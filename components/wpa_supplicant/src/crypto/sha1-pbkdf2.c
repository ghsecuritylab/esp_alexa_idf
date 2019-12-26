/*
 * SHA1-based key derivation function (PBKDF2) for IEEE 802.11i
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include "crypto/includes.h"
#include "crypto/common.h"
#include "crypto/sha1.h"
#include "crypto/md5.h"
#include "crypto/crypto.h"

#ifdef CONFIG_WPA_PBKDF_RUN_IN_LOW_PRIORITY_TASK
#include "wpa/wpa_debug.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#endif

static int 
pbkdf2_sha1_f(const char *passphrase, const char *ssid,
			 size_t ssid_len, int iterations, unsigned int count,
			 u8 *digest)
{
	unsigned char tmp[SHA1_MAC_LEN], tmp2[SHA1_MAC_LEN];
	int i, j;
	unsigned char count_buf[4];
	const u8 *addr[2];
	size_t len[2];
	size_t passphrase_len = os_strlen(passphrase);

	addr[0] = (u8 *) ssid;
	len[0] = ssid_len;
	addr[1] = count_buf;
	len[1] = 4;

	/* F(P, S, c, i) = U1 xor U2 xor ... Uc
	 * U1 = PRF(P, S || i)
	 * U2 = PRF(P, U1)
	 * Uc = PRF(P, Uc-1)
	 */

	count_buf[0] = (count >> 24) & 0xff;
	count_buf[1] = (count >> 16) & 0xff;
	count_buf[2] = (count >> 8) & 0xff;
	count_buf[3] = count & 0xff;
	if (hmac_sha1_vector((u8 *) passphrase, passphrase_len, 2, addr, len,
			     tmp))
		return -1;
	os_memcpy(digest, tmp, SHA1_MAC_LEN);

	for (i = 1; i < iterations; i++) {
		if (hmac_sha1((u8 *) passphrase, passphrase_len, tmp,
			      SHA1_MAC_LEN, tmp2))
			return -1;
		os_memcpy(tmp, tmp2, SHA1_MAC_LEN);
		for (j = 0; j < SHA1_MAC_LEN; j++)
			digest[j] ^= tmp2[j];
	}

	return 0;
}


/**
 * pbkdf2_sha1_internal - SHA1-based key derivation function (PBKDF2) for IEEE 802.11i
 * @passphrase: ASCII passphrase
 * @ssid: SSID
 * @ssid_len: SSID length in bytes
 * @iterations: Number of iterations to run
 * @buf: Buffer for the generated key
 * @buflen: Length of the buffer in bytes
 * Returns: 0 on success, -1 of failure
 *
 * This function is used to derive PSK for WPA-PSK. For this protocol,
 * iterations is set to 4096 and buflen to 32. This function is described in
 * IEEE Std 802.11-2004, Clause H.4. The main construction is from PKCS#5 v2.0.
 */
static int 
pbkdf2_sha1_internal(const char *passphrase, const char *ssid, size_t ssid_len,
		int iterations, u8 *buf, size_t buflen)
{
	unsigned int count = 0;
	unsigned char *pos = buf;
	size_t left = buflen, plen;
	unsigned char digest[SHA1_MAC_LEN];

	while (left > 0) {
		count++;
		if (pbkdf2_sha1_f(passphrase, ssid, ssid_len, iterations,
				  count, digest))
			return -1;
		plen = left > SHA1_MAC_LEN ? SHA1_MAC_LEN : left;
		os_memcpy(pos, digest, plen);
		pos += plen;
		left -= plen;
	}

	return 0;
}

#ifdef CONFIG_WPA_PBKDF_RUN_IN_LOW_PRIORITY_TASK

typedef struct {
    char *passphrase;
    char *ssid;
    size_t ssid_len;
    int iterations;
    u8 *buf;
    size_t buflen;
    SemaphoreHandle_t sem;
    int ret;
} pbkdf_arg_list_t;

static uint32_t s_pbkdf_task_priority = CONFIG_WPA_PBKDF_TASK_PRIORITY;

static void 
pbkdf_task(void *arg)
{
    pbkdf_arg_list_t *arg_list = (pbkdf_arg_list_t*)arg;

    if (!arg_list) {
        wpa_printf(MSG_ERROR, "pbkdf_task: null arg");
    } else {
        arg_list->ret = pbkdf2_sha1_internal(arg_list->passphrase, arg_list->ssid, 
                            arg_list->ssid_len, arg_list->iterations, arg_list->buf, arg_list->buflen);
        xSemaphoreGive(arg_list->sem);
    }

    vTaskDelete(NULL);
}
#endif

int 
pbkdf2_sha1(const char *passphrase, const char *ssid, size_t ssid_len,
		int iterations, u8 *buf, size_t buflen)
{
#ifdef CONFIG_WPA_PBKDF_RUN_IN_LOW_PRIORITY_TASK
    uint32_t my_priority = uxTaskPriorityGet(NULL);

    if (my_priority > s_pbkdf_task_priority) {
        SemaphoreHandle_t sem;
        pbkdf_arg_list_t arg;

        sem = xSemaphoreCreateCounting(1, 0);
        if (!sem) {
            wpa_printf(MSG_ERROR, "pbkdf2_sha1: failed to create sync sem\n");
            return ESP_ERR_NO_MEM;
        }

        arg.passphrase = (char*)passphrase;
        arg.ssid = (char*)ssid;
        arg.ssid_len = ssid_len;
        arg.iterations = iterations;
        arg.buf = buf;
        arg.buflen = buflen;
        arg.sem = sem;
        arg.ret = 0;

        wpa_printf(MSG_DEBUG, "pbkdf2_sha1: my_priority=%d s_pbkdf_task_priority=%d\n", my_priority, s_pbkdf_task_priority);
        if (pdPASS != xTaskCreate(pbkdf_task, "pbkdf_task", 2048, (void*)&arg, s_pbkdf_task_priority, NULL)) {
            wpa_printf(MSG_ERROR, "pbkdf2_sha1: failed to create pbkdf task");
            vSemaphoreDelete(sem);
            return ESP_ERR_NO_MEM;
        }

        xSemaphoreTake(arg.sem, portMAX_DELAY);
        vSemaphoreDelete(sem);
        return arg.ret;
    } else {
        return pbkdf2_sha1_internal(passphrase, ssid, ssid_len, iterations, buf, buflen);
    }
#else
    return pbkdf2_sha1_internal(passphrase, ssid, ssid_len, iterations, buf, buflen);
#endif
}
