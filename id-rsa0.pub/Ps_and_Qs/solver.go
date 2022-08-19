package main
import (
	"fmt"
	"crypto/x509"
	"encoding/pem"
	"io/ioutil"
	"strings"
	"log"
	"crypto/rsa"
	"math/big"
)

func parseRSAPublicKey(publicKey string) (*rsa.PublicKey) {
	r := strings.NewReader(publicKey)
	pemBytes, err := ioutil.ReadAll(r)
	if err != nil {
		log.Fatal(err)
	}

	block, _ := pem.Decode(pemBytes)
	if block == nil {
		log.Println("error")
	}

	// extract all required information for public key
	publicKeyData, _ := x509.ParsePKIXPublicKey(block.Bytes)
	if publicKeyData == nil {
		log.Println("Unable to extract public Key")
	}

	return publicKeyData.(*rsa.PublicKey)
}

func RSAPrivateKeyDecrypt(cipherText, p, q, e *big.Int) (*big.Int) {
	p_minus_1 := new(big.Int)
	q_minus_1 := new(big.Int)
	phi_N := new(big.Int)
	N := new(big.Int)
	decryption_exp := new(big.Int)

	p_minus_1.Sub(p, big.NewInt(1))
	q_minus_1.Sub(q, big.NewInt(1))

	N.Mul(p, q)
	phi_N.Mul(p_minus_1, q_minus_1)

	decryption_exp.Exp(e, big.NewInt(-1), phi_N)
	return decryption_exp.Exp(cipherText, decryption_exp, N)
}


func main() {
	var publickey1, publickey2 string

	publickey1 = "-----BEGIN PUBLIC KEY-----\n" +
"MFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBAKzl5VggSXb/Jm2oqkPeRQwtpmGlLnJT" +
"Nre4LKx3VUljtLzYWj4xoG+aHBouwJT7DyeibpasCH8Yderr4zIGTNUCAwEAAQ==" +
"\n-----END PUBLIC KEY-----"

	publickey2 = "-----BEGIN PUBLIC KEY-----\n" +
"MF0wDQYJKoZIhvcNAQEBBQADTAAwSQJCAPsrpwx56OTlKtGAWn24bo5HUg3xYtnz" +
"nTj1X/8Hq7pLYNIVE57Yxoyr3zTOOBJufgTNzdKS0Rc5Ti4zZUkCkQvpAgMBAAE=" +
"\n-----END PUBLIC KEY-----"

	cipherText := new(big.Int)
	p := new(big.Int)
	q := new(big.Int)

	cipherText.SetString("f5ed9da29d8d260f22657e091f34eb930bc42f26f1e023f863ba13bee39071d1ea988ca62b9ad59d4f234fa7d682e22ce3194bbe5b801df3bd976db06b944da", 16)


	// first public key is by which message is encrypted
	publicKeyData1 := parseRSAPublicKey(publickey1)
	// second public key
	publicKeyData2 := parseRSAPublicKey(publickey2)

	// extract p and q from publickeys
	p.GCD(nil, nil, publicKeyData1.N, publicKeyData2.N)
	q.Div(publicKeyData1.N, p)

	// extract plain text 
	plainText := RSAPrivateKeyDecrypt(cipherText, p, q, big.NewInt(int64(publicKeyData1.E)))
	fmt.Printf("%x\n", plainText)
}
