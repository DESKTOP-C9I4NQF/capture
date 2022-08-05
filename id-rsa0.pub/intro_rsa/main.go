package main
import (
	"fmt"
	"math/big"
)

/*
Generation:
	Pick two prime numbers of the same bit size, call them p and q
	let N = p * q and let mul(N) = (p-1) * (q-1)
	Choose an integer e such that gcd(e, mul(N)) = 1 (i.e. such that e and mul(N) share no common factors)
	let d = e^-1 mod mul(N)
	Output (e, N) as the public key and d as the private key.
*/

/* 
	Enc
	To encrypt integer m with public key (e, N) calculate c = m^e mod N
	Output c as the encrypted message.

	Dec 
	To decrypt integer c with private key d calculate m = c^d mod N.
	Output m as the decrypted message.

	if you're
*/

func main() {
	p := new(big.Int)
	q := new(big.Int)

	p_minus_1 := new(big.Int)
	q_minus_1 := new(big.Int)

	g := new(big.Int)

	// after all without knowing a fucking shit we have written a piece of shit
	p.SetString("431d844bdcd801460488c4d17487d9a5ccc95698301d6ab2e218e4b575d52ea3", 16)
	q.SetString("599f55a1b0520a19233c169b8c339f10695f9e61c92bd8fd3c17c8bba0d5677e", 16)

	p_minus_1.Sub(p, big.NewInt(1))
	q_minus_1.Sub(q, big.NewInt(1))

	g.GCD(nil, nil, p, q)
}
