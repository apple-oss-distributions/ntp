/*
 * ntp_md5.h: deal with md5.h headers
 *
 * Use the system MD5 if available, otherwise libisc's.
 */
#ifndef NTP_MD5_H
#define NTP_MD5_H

#ifdef OPENSSL
# include "openssl/evp.h"
# include "libssl_compat.h"
#else	/* !OPENSSL follows */
/*
 * Provide OpenSSL-alike MD5 API if we're not using OpenSSL
 */
# if defined HAVE_MD5_H && defined HAVE_MD5INIT
#  include <md5.h>
#elif defined(__APPLE__)
#include <CommonCrypto/CommonDigest.h>
# define MD5_CTX      CC_MD5_CTX
# define MD5Init      CC_MD5_Init
# define MD5Update    CC_MD5_Update
# define MD5Final     CC_MD5_Final
# define EVP_MD_CTX_new()   calloc(1, sizeof(EVP_MD_CTX))
# define EVP_MD_CTX_free(x) free((x))
# else
#  include "isc/md5.h"
   typedef isc_md5_t		MD5_CTX;
#  define MD5Init(c)		isc_md5_init(c)
#  define MD5Update(c, p, s)	isc_md5_update(c, p, s)
#  define MD5Final(d, c)	isc_md5_final((c), (d))	/* swapped */
# endif

  typedef MD5_CTX			EVP_MD_CTX;

# define EVP_MD_CTX_free(c)		free(c)
# define EVP_MD_CTX_new()		calloc(1, sizeof(MD5_CTX))
# define EVP_get_digestbynid(t)		(t)
# define EVP_md5()			NULL
# define EVP_MD_CTX_init(c)
# define EVP_MD_CTX_set_flags(c, f)
# define EVP_DigestInit(c, dt)		(MD5Init(c), (dt))
# define EVP_DigestInit_ex(c, dt, i)	(MD5Init(c), 1)
# define EVP_DigestUpdate(c, p, s)	MD5Update(c, (const void *)(p), \
						  s)
# define EVP_DigestFinal(c, d, pdl)	\
	do {				\
		MD5Final((d), (c));	\
		*(pdl) = 16;		\
	} while (0)
# endif	/* !OPENSSL */
#endif	/* NTP_MD5_H */
