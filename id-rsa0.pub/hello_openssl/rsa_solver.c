#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>

RSA * createRSA(unsigned char *key)
{
	RSA *rsa= NULL;
	BIO *keybio;

	keybio = BIO_new_mem_buf(key, -1);
	if (keybio==NULL)
	{
		printf( "Failed to create key BIO");
		return 0;
	}

	// extract required stuff for RSA key
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
	if(rsa == NULL)
		printf( "Failed to create RSA");

	return rsa;
}

int private_decrypt(unsigned char * enc_data,int data_len,unsigned char *key, unsigned char *decrypted)
{
	RSA * rsa = createRSA(key);
	return RSA_private_decrypt(data_len,enc_data, decrypted, rsa, RSA_NO_PADDING);
}

int main(int argc, char* argv[])
{

	char privateKey[] = "-----BEGIN RSA PRIVATE KEY-----\n"
		"MIGtAgEAAiEA5tygpSZdOZUMfuO3oTGWR4cALBtWui5UzrQw2/8JlZ0CAwEAAQIh\n"
		"AI9n4Yp1KFfKlHaF8d15tgUONQXn+e3aI+beFKoi2XipAhEA/ZkHPmcDwXIqloGr\n"
		"minb1wIRAOkMdv7emMGd08gwwOQ6i6sCEQC0pjcXx9BQFCCsWDDCwAC/AhEAxYcn\n"
		"JQeO+izH4JpSJB/rWQIRAOO9m6JHEWgzLYD+fe003vw=\n"
		"-----END RSA PRIVATE KEY-----\n";

	char *cipher = "6794893f3c47247262e95fbed846e1a623fc67b1dd96e13c7f9fc3b880642e42";
	unsigned char decrypted[4098] = { 0 };
	unsigned char plainText[256] = { 0 };
	size_t len, idx = 0;
	size_t decrypted_len;

	for (len = 0; *cipher != '\0'; cipher += 2, len++)
		sscanf(cipher, "%02hhx", &plainText[len]);

	// decrypt with private key
	decrypted_len = private_decrypt(plainText, len, privateKey, decrypted);

	// skip zeros from big number
	while (decrypted[idx] == '\0' && ++idx < decrypted_len);

	// real meat is here
	for (idx; idx < decrypted_len; idx++)
		printf("%.02x", decrypted[idx]);
	printf("\n");

	return 0;
}
