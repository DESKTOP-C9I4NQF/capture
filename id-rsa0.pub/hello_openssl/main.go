package main
import (
	"fmt"
	"crypto/x509"
	"encoding/pem"
	"io/ioutil"
	"strings"
	"log"
	"encoding/hex"
	"math/big"
)


func main() {
	var privk = "-----BEGIN RSA PRIVATE KEY-----\n"+
"MIGtAgEAAiEA5tygpSZdOZUMfuO3oTGWR4cALBtWui5UzrQw2/8JlZ0CAwEAAQIh"+
"AI9n4Yp1KFfKlHaF8d15tgUONQXn+e3aI+beFKoi2XipAhEA/ZkHPmcDwXIqloGr"+
"minb1wIRAOkMdv7emMGd08gwwOQ6i6sCEQC0pjcXx9BQFCCsWDDCwAC/AhEAxYcn"+
"JQeO+izH4JpSJB/rWQIRAOO9m6JHEWgzLYD+fe003vw="+
"\n-----END RSA PRIVATE KEY-----"

	r := strings.NewReader(privk)
	pemBytes, err := ioutil.ReadAll(r)
	if err != nil {
		log.Fatal(err)
	}

	block, _ := pem.Decode(pemBytes)
	if block == nil {
		log.Println("error")
	}

	// extract all required information for private key
	priv, _ := x509.ParsePKCS1PrivateKey(block.Bytes)

	// extract cipher text
	cipherText, _ := hex.DecodeString("6794893f3c47247262e95fbed846e1a623fc67b1dd96e13c7f9fc3b880642e42")

	c := new(big.Int).SetBytes(cipherText)
	plainText := c.Exp(c, priv.D, priv.N).Bytes()

	fmt.Printf("%x\n", plainText)
}
