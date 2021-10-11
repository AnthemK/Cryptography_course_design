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
#include <openssl/sm3.h> 
#include <openssl/bio.h> 
#include <openssl/pem.h> 
#include <openssl/x509.h> 
#include <openssl/sm2.h> 
#include <openssl/pkcs7.h> 
using namespace std;

const char* CACert = "\
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

char ans[10000]; 
const char PKeyB[] = "\
-----BEGIN EC PARAMETERS----- \n\
BggqgRzPVQGCLQ== \n\
-----END EC PARAMETERS----- \n\
-----BEGIN EC PRIVATE KEY----- \n\
MHcCAQEEINQhCKslrI3tKt6cK4Kxkor/LBvM8PSv699Xea7kTXTToAoGCCqBHM9V \n\
AYItoUQDQgAEH7rLLiFASe3SWSsGbxFUtfPY//pXqLvgM6ROyiYhLkPxEulwrTe8 \n\
kv5R8/NA7kSSvcsGIQ9EPWhr6HnCULpklw== \n\
-----END EC PRIVATE KEY----- \n\
";

X509 *getX509(const char *cert)
{
	BIO *bio;
	bio = BIO_new(BIO_s_mem());
	BIO_puts(bio, cert);
    return  PEM_read_bio_X509(bio, NULL, NULL, NULL);
}
EVP_PKEY *getpkey(const char *private_key)
{
	BIO *bio_pkey = BIO_new_mem_buf((char *)private_key, strlen(private_key));  //字符串压成BIO 
	if (bio_pkey == NULL)
 		return NULL;
 	return  PEM_read_bio_PrivateKey(bio_pkey, NULL, NULL, NULL);
}

void Encode_pkcs7(void)
{
	BIO* mem=BIO_new_fd(fileno(stdin), BIO_NOCLOSE);       //读数据 
	
	//解码 
	EVP_PKEY* pKey = getpkey(PKeyB);  //获得私钥 
	PKCS7* p7 = PEM_read_bio_PKCS7(mem, NULL, NULL, NULL);  //读密文 
	free(mem);
	BIO* bio_out = PKCS7_dataDecode(p7, pKey, NULL, NULL);//解析出原始数据
	int len = BIO_read(bio_out, ans, 10000);//从BIO中读取原始数据
	if (len <=0)   //比如返回NULL 
	{
		printf("ERROR\n");
		return;
	}
	//验证签名 
	STACK_OF(PKCS7_SIGNER_INFO)* sk = PKCS7_get_signer_info(p7);//获得签名者信息stack
	if(!sk)
	{
		printf("ERROR\n");
		return;
	}
	int n = sk_PKCS7_SIGNER_INFO_num(sk);//获得签名者个数
	PKCS7_SIGNER_INFO* signInfo;
	X509_STORE* ca = X509_STORE_new();
	X509_STORE_CTX* ct = X509_STORE_CTX_new();
	X509_STORE_add_cert(ca, getX509(CACert));   //加入证书 
	for (int j = 0; j < n; j++) 
	{
		signInfo = sk_PKCS7_SIGNER_INFO_value(sk, j);//获得签名者信息
		if (PKCS7_dataVerify(ca, ct, bio_out, p7, signInfo) <= 0)   //验证签名 
		{
			printf("ERROR\n");
			return;
		}
	}
	
	for (int i = 0; i < len; i++) 
	{
		if (isprint(ans[i]) == 0) 
		{
			printf("ERROR\n");
			return;
		}
	}
	
	ans[len]=0;printf("%s",ans); 
	return;
}

void init()
{
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	return;
}

int main(void) 
{ 
	init();
	Encode_pkcs7();
	return 0;
}
