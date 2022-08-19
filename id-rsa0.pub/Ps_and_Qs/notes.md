# Ps and Qs
> RSA keys generated with one critical weakness
In this level challenge author described that **p** and **q** 
are generated with weak random number generator that means there
might be possiblity that two keys collected may have same one prime factor

So we will take advantage of this weakness in order get plain text

Big Plans:
1. Extract either p or q by using GCD 
2. Decrypt Plain Text from by calculating from p by dividing with N.

```go
	// extract p and q from publickeys
	p.GCD(nil, nil, publicKeyData1.N, publicKeyData2.N)
	q.Div(publicKeyData1.N, p)
```
flag: f6a1df363229c6ec 
