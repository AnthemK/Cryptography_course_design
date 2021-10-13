#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream> 
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/err.h>
//#include <openssl/sm3.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
//#include <openssl/sm2.h>
#include <openssl/pkcs7.h>

//#pragma comment(lib, "libcrypto.a")
//#pragma comment(lib, "libssl.a")

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
	BIO *bio_out,*mem;
	PKCS7 *p7;
	X509 *cert;
  	X509_STORE *ca;
	size_t len;
	char buf[70],ans[5000];
	string str="";
	char *out;
	int i,flags,num_signer;
	EVP_PKEY* pKey = getpkey(pkeyB);
	if(pKey==NULL){
		printf("key_ERROR\n");
		return;
	}
 	//freopen("C:\\Users\\asus\\Desktop\\test\\5.in","r",stdin);
 	ca=X509_STORE_new();
  	X509_STORE_add_cert(ca,getX509(cacert));
  
 	while(fgets(buf,sizeof(buf),stdin)){
 		str+=buf;	
	}
 	str+='\n';
 	const char *s=str.c_str();
	mem=BIO_new_mem_buf(s,strlen(s));
	p7 = PKCS7_new();
	//PKCS7_set_type(p7,NID_pkcs7_signedAndEnveloped);
	p7= PEM_read_bio_PKCS7(mem, NULL, NULL, NULL);
	if(p7==NULL){
		printf("ERROR\n");
		return;
	}
	//PKCS7_set_type(p7,NID_pkcs7_signedAndEnveloped);
	bio_out=PKCS7_dataDecode(p7,pKey,NULL,NULL);
	len=BIO_read(bio_out,ans,sizeof(ans));
	if(len==-2){
		printf("ERROR\n");
		return;
	}
	STACK_OF(PKCS7_SIGNER_INFO) *so=PKCS7_get_signer_info(p7);
	if(so==NULL){
		printf("ERROR\n");
		return;
	}
	num_signer=sk_PKCS7_SIGNER_INFO_num(so);
	PKCS7_SIGNER_INFO *si;
	for(i=0;i<num_signer;i++){
		si=sk_PKCS7_SIGNER_INFO_value(so,i);
		X509_STORE_CTX* ct=X509_STORE_CTX_new();
		if(PKCS7_dataVerify(ca,ct,bio_out,p7,si)!=1){
			printf("ERROR\n");
			return;
		}
	}
	for(i=0;i<len;i++){
		printf("%c",ans[i]);
	}
	return;
}


int main(int argc, char **argv)
{
	//ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	gen_pkcs7();
	return 0;
}
