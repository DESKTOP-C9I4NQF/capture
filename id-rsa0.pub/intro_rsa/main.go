package main
import (
	"fmt"
	"math/big"
)

/*
	d := e^-1 mod phi(N), d is private key
	where phi (N) = (p - 1) * (q - 1)

	(e, N) is public key
	where N = p * q, e is such that gcd (e, phi(N)) == 1

	enc: c = m ^ e mod N
	dec: m = c ^ d mod N
*/
func main() {
	exponent := new(big.Int)
	N := new(big.Int)
	message := new(big.Int)

	cipher := new(big.Int)
	decryption_exponent := new(big.Int)

	//  we are going to using some modulo powering inorder get our plain
	// text from cipher of rsa
	exponent.SetString("3", 16)

	cipher.SetString("599f55a1b0520a19233c169b8c339f10695f9e61c92bd8fd3c17c8bba0d5677e", 16)
	decryption_exponent.SetString("431d844bdcd801460488c4d17487d9a5ccc95698301d6ab2e218e4b575d52ea3", 16)
	N.SetString("64ac4671cb4401e906cd273a2ecbc679f55b879f0ecb25eefcb377ac724ee3b1", 16)

	cipher.Exp(cipher, decryption_exponent, N)
	fmt.Printf("%x\n", cipher)
	message.Mod(message, N)
}
