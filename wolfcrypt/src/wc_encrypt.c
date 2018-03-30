/* wc_encrypt.c
 *
 * Copyright (C) 2006-2017 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */


#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/des3.h>
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/wc_encrypt.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/asn.h>
#include <wolfssl/wolfcrypt/coding.h>
#include <wolfssl/wolfcrypt/pwdbased.h>

#ifdef NO_INLINE
    #include <wolfssl/wolfcrypt/misc.h>
#else
    #define WOLFSSL_MISC_INCLUDED
    #include <wolfcrypt/src/misc.c>
#endif

#if !defined(NO_AES) && defined(HAVE_AES_CBC)
#ifdef HAVE_AES_DECRYPT
int wc_AesCbcDecryptWithKey(byte* out, const byte* in, word32 inSz,
                                  const byte* key, word32 keySz, const byte* iv)
{
    int  ret = 0;
#ifdef WOLFSSL_SMALL_STACK
    Aes* aes = NULL;
#else
    Aes  aes[1];
#endif

    if (out == NULL || in == NULL || key == NULL || iv == NULL) {
        return BAD_FUNC_ARG;
    }

#ifdef WOLFSSL_SMALL_STACK
    aes = (Aes*)XMALLOC(sizeof(Aes), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (aes == NULL)
        return MEMORY_E;
#endif

    ret = wc_AesInit(aes, NULL, INVALID_DEVID);
    if (ret == 0) {
        ret = wc_AesSetKey(aes, key, keySz, iv, AES_DECRYPTION);
        if (ret == 0)
            ret = wc_AesCbcDecrypt(aes, out, in, inSz);

        wc_AesFree(aes);
    }

#ifdef WOLFSSL_SMALL_STACK
    XFREE(aes, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}
#endif /* HAVE_AES_DECRYPT */

int wc_AesCbcEncryptWithKey(byte* out, const byte* in, word32 inSz,
                            const byte* key, word32 keySz, const byte* iv)
{
    int  ret = 0;
#ifdef WOLFSSL_SMALL_STACK
    Aes* aes = NULL;
#else
    Aes  aes[1];
#endif

#ifdef WOLFSSL_SMALL_STACK
    aes = (Aes*)XMALLOC(sizeof(Aes), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (aes == NULL)
        return MEMORY_E;
#endif

    ret = wc_AesInit(aes, NULL, INVALID_DEVID);
    if (ret == 0) {
        ret = wc_AesSetKey(aes, key, keySz, iv, AES_ENCRYPTION);
        if (ret == 0)
            ret = wc_AesCbcEncrypt(aes, out, in, inSz);

        wc_AesFree(aes);
    }

#ifdef WOLFSSL_SMALL_STACK
    XFREE(aes, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}
#endif /* !NO_AES && HAVE_AES_CBC */


#ifndef NO_DES3
int wc_Des_CbcEncryptWithKey(byte* out, const byte* in, word32 sz,
                             const byte* key, const byte* iv)
{
    int ret  = 0;
#ifdef WOLFSSL_SMALL_STACK
    Des* des = NULL;
#else
    Des  des[1];
#endif

#ifdef WOLFSSL_SMALL_STACK
    des = (Des*)XMALLOC(sizeof(Des), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (des == NULL)
        return MEMORY_E;
#endif

    ret = wc_Des_SetKey(des, key, iv, DES_ENCRYPTION);
    if (ret == 0)
        ret = wc_Des_CbcEncrypt(des, out, in, sz);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(des, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}

int wc_Des_CbcDecryptWithKey(byte* out, const byte* in, word32 sz,
                             const byte* key, const byte* iv)
{
    int ret  = 0;
#ifdef WOLFSSL_SMALL_STACK
    Des* des = NULL;
#else
    Des  des[1];
#endif

#ifdef WOLFSSL_SMALL_STACK
    des = (Des*)XMALLOC(sizeof(Des), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (des == NULL)
        return MEMORY_E;
#endif

    ret = wc_Des_SetKey(des, key, iv, DES_DECRYPTION);
    if (ret == 0)
        ret = wc_Des_CbcDecrypt(des, out, in, sz);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(des, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}


int wc_Des3_CbcEncryptWithKey(byte* out, const byte* in, word32 sz,
                              const byte* key, const byte* iv)
{
    int ret    = 0;
#ifdef WOLFSSL_SMALL_STACK
    Des3* des3 = NULL;
#else
    Des3  des3[1];
#endif

#ifdef WOLFSSL_SMALL_STACK
    des3 = (Des3*)XMALLOC(sizeof(Des3), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (des3 == NULL)
        return MEMORY_E;
#endif

    ret = wc_Des3Init(des3, NULL, INVALID_DEVID);
    if (ret == 0) {
        ret = wc_Des3_SetKey(des3, key, iv, DES_ENCRYPTION);
        if (ret == 0)
            ret = wc_Des3_CbcEncrypt(des3, out, in, sz);
        wc_Des3Free(des3);
    }

#ifdef WOLFSSL_SMALL_STACK
    XFREE(des3, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}


int wc_Des3_CbcDecryptWithKey(byte* out, const byte* in, word32 sz,
                              const byte* key, const byte* iv)
{
    int ret    = 0;
#ifdef WOLFSSL_SMALL_STACK
    Des3* des3 = NULL;
#else
    Des3  des3[1];
#endif

#ifdef WOLFSSL_SMALL_STACK
    des3 = (Des3*)XMALLOC(sizeof(Des3), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (des3 == NULL)
        return MEMORY_E;
#endif

    ret = wc_Des3Init(des3, NULL, INVALID_DEVID);
    if (ret == 0) {
        ret = wc_Des3_SetKey(des3, key, iv, DES_DECRYPTION);
        if (ret == 0)
            ret = wc_Des3_CbcDecrypt(des3, out, in, sz);
        wc_Des3Free(des3);
    }

#ifdef WOLFSSL_SMALL_STACK
    XFREE(des3, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return ret;
}

#endif /* !NO_DES3 */


#ifdef WOLFSSL_ENCRYPTED_KEYS

int wc_BufferKeyDecrypt(EncryptedInfo* info, byte* der, word32 derSz,
    const byte* password, int passwordSz, int hashType)
{
    int ret;
#ifdef WOLFSSL_SMALL_STACK
    byte* key      = NULL;
#else
    byte  key[AES_MAX_KEY_SIZE];
#endif

    (void)derSz;

    if (der == NULL || password == NULL || info == NULL || info->keySz == 0) {
        return BAD_FUNC_ARG;
    }

    /* use file's salt for key derivation, hex decode first */
    if (Base16_Decode(info->iv, info->ivSz, info->iv, &info->ivSz) != 0) {
        return BUFFER_E;
    }

#ifdef WOLFSSL_SMALL_STACK
    key = (byte*)XMALLOC(AES_MAX_KEY_SIZE, NULL, DYNAMIC_TYPE_SYMETRIC_KEY);
    if (key == NULL) {
        return MEMORY_E;
    }
#endif

    if ((ret = wc_PBKDF1(key, password, passwordSz, info->iv, info->ivSz, 1,
                                        info->keySz, hashType)) != 0) {
#ifdef WOLFSSL_SMALL_STACK
        XFREE(key, NULL, DYNAMIC_TYPE_SYMETRIC_KEY);
#endif
        return ret;
    }

    ret = NOT_COMPILED_IN; /* set error in case no cipher is enabled/matched */
#ifndef NO_DES3
    if (info->cipherType == WC_CIPHER_DES)
        ret = wc_Des_CbcDecryptWithKey(der, der, derSz, key, info->iv);
    if (info->cipherType == WC_CIPHER_DES3)
        ret = wc_Des3_CbcDecryptWithKey(der, der, derSz, key, info->iv);
#endif /* NO_DES3 */
#if !defined(NO_AES) && defined(HAVE_AES_CBC) && defined(HAVE_AES_DECRYPT)
    if (info->cipherType == WC_CIPHER_AES_CBC)
        ret = wc_AesCbcDecryptWithKey(der, der, derSz, key, info->keySz,
            info->iv);
#endif /* !NO_AES && HAVE_AES_CBC && HAVE_AES_DECRYPT */

#ifdef WOLFSSL_SMALL_STACK
    XFREE(key, NULL, DYNAMIC_TYPE_SYMETRIC_KEY);
#endif

    return ret;
}

int wc_BufferKeyEncrypt(EncryptedInfo* info, byte* der, word32 derSz,
    const byte* password, int passwordSz, int hashType)
{
    int ret;
#ifdef WOLFSSL_SMALL_STACK
    byte* key      = NULL;
#else
    byte  key[AES_MAX_KEY_SIZE];
#endif

    (void)derSz;

    if (der == NULL || password == NULL || info == NULL || info->keySz == 0 ||
            info->ivSz == 0) {
        return BAD_FUNC_ARG;
    }

#ifdef WOLFSSL_SMALL_STACK
    key = (byte*)XMALLOC(AES_MAX_KEY_SIZE, NULL, DYNAMIC_TYPE_SYMETRIC_KEY);
    if (key == NULL) {
        return MEMORY_E;
    }
#endif /* WOLFSSL_SMALL_STACK */

    if ((ret = wc_PBKDF1(key, password, passwordSz, info->iv, info->ivSz, 1,
                                        info->keySz, hashType)) != 0) {
#ifdef WOLFSSL_SMALL_STACK
        XFREE(key, NULL, DYNAMIC_TYPE_SYMETRIC_KEY);
#endif
        return ret;
    }

    ret = NOT_COMPILED_IN; /* set error in case no cipher is enabled/matched */
#ifndef NO_DES3
    if (info->cipherType == WC_CIPHER_DES)
        ret = wc_Des_CbcEncryptWithKey(der, der, derSz, key, info->iv);
    if (info->cipherType == WC_CIPHER_DES3)
        ret = wc_Des3_CbcEncryptWithKey(der, der, derSz, key, info->iv);
#endif /* NO_DES3 */
#ifndef NO_AES
    if (info->cipherType == WC_CIPHER_AES_CBC)
        ret = wc_AesCbcEncryptWithKey(der, der, derSz, key, info->keySz,
            info->iv);
#endif /* NO_AES */

#ifdef WOLFSSL_SMALL_STACK
    XFREE(key, NULL, DYNAMIC_TYPE_SYMETRIC_KEY);
#endif

    return ret;
}

#endif /* WOLFSSL_ENCRYPTED_KEYS */
