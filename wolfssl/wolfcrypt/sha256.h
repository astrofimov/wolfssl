/* sha256.h
 *
 * This file is part of wolfSSL. (formerly known as CyaSSL)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */


/* code submitted by raphael.huck@efixo.com */


#ifndef NO_SHA256

#ifndef WOLF_CRYPT_SHA256_H
#define WOLF_CRYPT_SHA256_H

#ifdef HAVE_FIPS
/* for fips */
#include <cyassl/ctaocrypt/sha256.h>
#endif

#include <wolfssl/wolfcrypt/types.h>

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef HAVE_FIPS
#define CTAO_CRYPT_SHA256_H
#ifdef WOLFSSL_PIC32MZ_HASH
#include "port/pic32/pic32mz-crypt.h"
#endif


/* in bytes */
enum {
    SHA256              =  2,   /* hash type unique */
    SHA256_BLOCK_SIZE   = 64,
    SHA256_DIGEST_SIZE  = 32,
    SHA256_PAD_SIZE     = 56
};


/* Sha256 digest */
typedef struct Sha256 {
    word32  buffLen;   /* in bytes          */
    word32  loLen;     /* length in bytes   */
    word32  hiLen;     /* length in bytes   */
    word32  digest[SHA256_DIGEST_SIZE / sizeof(word32)];
    word32  buffer[SHA256_BLOCK_SIZE  / sizeof(word32)];
    #ifdef WOLFSSL_PIC32MZ_HASH
        pic32mz_desc desc ; /* Crypt Engine descripter */
    #endif
} Sha256;

#endif /* HAVE_FIPS */

WOLFSSL_API int wc_InitSha256(Sha256*);
WOLFSSL_API int wc_Sha256Update(Sha256*, const byte*, word32);
WOLFSSL_API int wc_Sha256Final(Sha256*, byte*);
WOLFSSL_API int wc_Sha256Hash(const byte*, word32, byte*);
 
#ifdef __cplusplus
    } /* extern "C" */
#endif
//#else
//#define wc_InitSha256 wc_InitSha256Sha256*);
//#define int wc_Sha256Update(Swc_Sha256Updateha256*, const byte*, word32);
//#define int wc_Sha256Final(wc_Sha256FinalSha256*, byte*);
//#define int wc_Sha256Hash(wc_Sha256Hashconst byte*, word32, byte*);
//
//#endif /* HAVE_FIPS */

#endif /* WOLF_CRYPT_SHA256_H */
#endif /* NO_SHA256 */

