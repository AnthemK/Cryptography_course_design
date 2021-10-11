#include <iostream>
 #include <cstring>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/pkcs7.h>
#include <openssl/x509.h>
#include <openssl/sm2.h>
#include <openssl/rand.h>
#include <openssl/objects.h>
#include <openssl/x509v3.h>
 
 using namespace std;

const char cacert[] = "\
-----BEGIN CERTIFICATE----- \n\
MIIB/zCCAaagAwIBAgIJAKKa0PAt9M1FMAoGCCqBHM9VAYN1MFsxCzAJBgNVBAYT \n\
AkNOMQ4wDAYDVQQIDAVIdUJlaTEOMAwGA1UEBwwFV3VIYW4xDTALBgNVBAoMBEhV \n\
U1QxDDAKBgNVBAsMA0NTRTEPMA0GA1UEAwwGY2Fyb290MB4XDTIwMDkyMDIwNTkx \n\
OVoXDTMwMDkxODIwNTkxOVowWzELMAkGA1UEBhMCQ04xDjAMBgNVBAgMBUh1QmVp \n\
MQ4wDAYDVQQHDAVXdUhhbjENMAsGA1UECgwESFVTVDEMMAoGA1UECwwDQ1NFMQ8w \n\
DQYDVQQDDAZjYXJvb3QwWTATBgcqhkjOPQIBBggqgRzPVQGCLQNCAASJ8mm28JJR \n\
bZKLr6DCo1+KWimpKEsiTfZM19Zi5ao7Au6YLosyN71256MWmjwkwXxJeLa0lCfm \n\
kF/YWCX6qGQ0o1MwUTAdBgNVHQ4EFgQUAL5hW3RUzqvsiTzIc1gUHeK5uzQwHwYD \n\
VR0jBBgwFoAUAL5hW3RUzqvsiTzIc1gUHeK5uzQwDwYDVR0TAQH/BAUwAwEB/zAK \n\
BggqgRzPVQGDdQNHADBEAiAaZMmvE5zzXHx/TBgdUhjtpRH3Jpd6OZ+SOAfMtKxD \n\
LAIgdKq/v2Jkmn37Y9U8FHYDfFqk5I0qlQOAmuvbVUi3yvM= \n\
-----END CERTIFICATE----- \n\
";

const char certB[] = "\
-----BEGIN CERTIFICATE----- \n\
MIICHDCCAcKgAwIBAgIBAzAKBggqgRzPVQGDdTBbMQswCQYDVQQGEwJDTjEOMAwG \n\
A1UECAwFSHVCZWkxDjAMBgNVBAcMBVd1SGFuMQ0wCwYDVQQKDARIVVNUMQwwCgYD \n\
VQQLDANDU0UxDzANBgNVBAMMBmNhcm9vdDAeFw0yMDA5MjAyMTIxMDNaFw0zMDA5 \n\
MTgyMTIxMDNaMEoxCzAJBgNVBAYTAkNOMQ4wDAYDVQQIDAVIdUJlaTENMAsGA1UE \n\
CgwESFVTVDEMMAoGA1UECwwDQ1NFMQ4wDAYDVQQDDAVVc2VyQjBZMBMGByqGSM49 \n\
AgEGCCqBHM9VAYItA0IABB+6yy4hQEnt0lkrBm8RVLXz2P/6V6i74DOkTsomIS5D \n\
8RLpcK03vJL+UfPzQO5Ekr3LBiEPRD1oa+h5wlC6ZJejgYcwgYQwCQYDVR0TBAIw \n\
ADALBgNVHQ8EBAMCBSAwKgYJYIZIAYb4QgENBB0WG0dtU1NMIEdlbmVyYXRlZCBD \n\
ZXJ0aWZpY2F0ZTAdBgNVHQ4EFgQUAhr5rk22vn/vEACBoMYN1FMPmGAwHwYDVR0j \n\
BBgwFoAUAL5hW3RUzqvsiTzIc1gUHeK5uzQwCgYIKoEcz1UBg3UDSAAwRQIhANkY \n\
3oWdgG8jriJXuRhX5le4QjE19EnKB4rWCQcKvORfAiAR4hnLYT+dXgFCxVgPkjGO \n\
PDXj9mXJk6crvhRRxZfWNg== \n\
-----END CERTIFICATE----- \n\
";

const char certA[] = "\
-----BEGIN CERTIFICATE----- \n\
MIICGzCCAcKgAwIBAgIBAjAKBggqgRzPVQGDdTBbMQswCQYDVQQGEwJDTjEOMAwG \n\
A1UECAwFSHVCZWkxDjAMBgNVBAcMBVd1SGFuMQ0wCwYDVQQKDARIVVNUMQwwCgYD \n\
VQQLDANDU0UxDzANBgNVBAMMBmNhcm9vdDAeFw0yMDA5MjAyMTEyMTZaFw0zMDA5 \n\
MTgyMTEyMTZaMEoxCzAJBgNVBAYTAkNOMQ4wDAYDVQQIDAVIdUJlaTENMAsGA1UE \n\
CgwESFVTVDEMMAoGA1UECwwDQ1NFMQ4wDAYDVQQDDAVVc2VyQTBZMBMGByqGSM49 \n\
AgEGCCqBHM9VAYItA0IABHvafDoCgn+mlwfMr89kPlC96XsSJACun0xNLEFOnb46 \n\
kZSeKUfRtLUcgpDoiwOVV7LiileewgJQqKFi5vy7wICjgYcwgYQwCQYDVR0TBAIw \n\
ADALBgNVHQ8EBAMCBSAwKgYJYIZIAYb4QgENBB0WG0dtU1NMIEdlbmVyYXRlZCBD \n\
ZXJ0aWZpY2F0ZTAdBgNVHQ4EFgQUIePApg8IOc4nvX/kMH0Cja0Mf4gwHwYDVR0j \n\
BBgwFoAUAL5hW3RUzqvsiTzIc1gUHeK5uzQwCgYIKoEcz1UBg3UDRwAwRAIgI3Q0 \n\
qndxJPTgvC8sPNarf7pJwdsIrU0Ajmlv4PdatU4CIHU8Pyt6rT9BaHrf8ppJduPe \n\
GWCy2i4+1au/zyp42b0r \n\
-----END CERTIFICATE----- \n\
";

const char certC[] = "\
-----BEGIN CERTIFICATE----- \n\
MIICPjCCAeWgAwIBAgIBBDAKBggqgRzPVQGDdTBbMQswCQYDVQQGEwJDTjEOMAwG \n\
A1UECAwFSHVCZWkxDjAMBgNVBAcMBVd1SGFuMQ0wCwYDVQQKDARIVVNUMQwwCgYD \n\
VQQLDANDU0UxDzANBgNVBAMMBmNhcm9vdDAeFw0yMDA5MjcxNDIwMjFaFw0zMDA5 \n\
MjUxNDIwMjFaMG0xCzAJBgNVBAYTAkNOMQ4wDAYDVQQIDAVIdUJlaTENMAsGA1UE \n\
CgwESFVTVDEMMAoGA1UECwwDQ1NFMQ4wDAYDVQQDDAVVc2VyQzEhMB8GCSqGSIb3 \n\
DQEJARYSeG10YW5nQGh1c3QuZWR1LmNuMFkwEwYHKoZIzj0CAQYIKoEcz1UBgi0D \n\
QgAEHSrf7RNs2+Rf1TrPeJREGSy+6YTC9t4rtp1vY7Akuct504We3iAiALcIarFt \n\
QxQOr5Zf4l7Ybt3NocM8fLxkNqOBhzCBhDAJBgNVHRMEAjAAMAsGA1UdDwQEAwIF \n\
4DAqBglghkgBhvhCAQ0EHRYbR21TU0wgR2VuZXJhdGVkIENlcnRpZmljYXRlMB0G \n\
A1UdDgQWBBQvMLtMYb/dD9cNF0+s/cZNxHUBGjAfBgNVHSMEGDAWgBQAvmFbdFTO \n\
q+yJPMhzWBQd4rm7NDAKBggqgRzPVQGDdQNHADBEAiBrTVuYbSUlOX4TYV4q094S \n\
XgfnobZWYPeRjGmBNAEIMwIgfHrQR9RfLd62bg0iZBpfgwvtGuaCE3DX24xapZ6U \n\
fIc= \n\
-----END CERTIFICATE----- \n\
";

const char pkeyA[] = "\
-----BEGIN EC PARAMETERS----- \n\
BggqgRzPVQGCLQ== \n\
-----END EC PARAMETERS----- \n\
-----BEGIN EC PRIVATE KEY----- \n\
MHcCAQEEIErrb3NV+HqWKfoINmBAFyNAe/knxHHXt5tXi2YCqgmtoAoGCCqBHM9V \n\
AYItoUQDQgAEe9p8OgKCf6aXB8yvz2Q+UL3pexIkAK6fTE0sQU6dvjqRlJ4pR9G0 \n\
tRyCkOiLA5VXsuKKV57CAlCooWLm/LvAgA== \n\
-----END EC PRIVATE KEY----- \n\
";
const char pkeyC[] = "\
-----BEGIN EC PARAMETERS----- \n\
BggqgRzPVQGCLQ== \n\
-----END EC PARAMETERS----- \n\
-----BEGIN EC PRIVATE KEY----- \n\
MHcCAQEEIIyXY38MKYLhXXRu/YC6+KxAYJ2rwUIlWAFwOmQKGhDboAoGCCqBHM9V \n\
AYItoUQDQgAEHSrf7RNs2+Rf1TrPeJREGSy+6YTC9t4rtp1vY7Akuct504We3iAi \n\
ALcIarFtQxQOr5Zf4l7Ybt3NocM8fLxkNg== \n\
-----END EC PRIVATE KEY----- \n\
";
const char pkeyB[] = "\
-----BEGIN EC PARAMETERS----- \n\
BggqgRzPVQGCLQ== \n\
-----END EC PARAMETERS----- \n\
-----BEGIN EC PRIVATE KEY----- \n\
MHcCAQEEINQhCKslrI3tKt6cK4Kxkor/LBvM8PSv699Xea7kTXTToAoGCCqBHM9V \n\
AYItoUQDQgAEH7rLLiFASe3SWSsGbxFUtfPY//pXqLvgM6ROyiYhLkPxEulwrTe8 \n\
kv5R8/NA7kSSvcsGIQ9EPWhr6HnCULpklw== \n\
-----END EC PRIVATE KEY----- \n\
";
const char message[] = "A0000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999aaaaaaaaaabbbbbbbbbbccccccccccddddddddddeeeeeeeeeeffffffffff";
 
X509 *getX509(const char *cert)
{
	BIO *bio;
	bio = BIO_new(BIO_s_mem());
	BIO_puts(bio, cert);
    return  PEM_read_bio_X509(bio, NULL, NULL, NULL);
}
EVP_PKEY *getpkey(const char *private_key)
{
	BIO *bio_pkey = BIO_new_mem_buf((char *)private_key, strlen(private_key));
	if (bio_pkey == NULL)
 		return NULL;
 	return  PEM_read_bio_PrivateKey(bio_pkey, NULL, NULL, NULL);
}

 
void gen_pkcs7()
{
	BIO *p7bio;
	const EVP_CIPHER *cipher;
	EVP_PKEY *pkey;
	PKCS7*p7;
	X509 *cert;
	size_t len;
	char *out;
	int flags;
 
	p7 = PKCS7_new();
	PKCS7_set_type(p7,NID_pkcs7_signedAndEnveloped);
	cipher=EVP_aes_128_cbc();
	PKCS7_set_cipher(p7,cipher);
	PKCS7_SIGNER_INFO* si=PKCS7_add_signature(p7,getX509(certC),getpkey(pkeyC),EVP_sha1());
	PKCS7_add_signed_attribute(si, NID_pkcs9_contentType, V_ASN1_OBJECT,OBJ_nid2obj(NID_pkcs7_data));

	PKCS7_add_recipient(p7,getX509(certB));
	PKCS7_add_certificate(p7,getX509(certC));
	PKCS7_add_certificate(p7,getX509(cacert));
	p7bio=PKCS7_dataInit(p7,NULL);
	BIO_write(p7bio,message,strlen(message));
	BIO_flush(p7bio);
	PKCS7_dataFinal(p7,p7bio);
	BIO_free(p7bio);
	BIO* bio_out=BIO_new_fd(fileno(stdout), BIO_NOCLOSE);
	PEM_write_bio_PKCS7(bio_out, p7);
	BIO_flush(bio_out);
	BIO_free(bio_out); 
	PKCS7_free(p7);
}


int
main(int argc, char **argv)
{
	int flag=0;
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	gen_pkcs7();
	return 0;
}