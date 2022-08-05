package main
import (
	"fmt"
	"crypto/sha256"
	"crypto/md5"
)


func main() {
	data := []byte("id0-rsa.pub")
	hash := sha256.Sum256(data)
	my_bytes_for_converting2md5 := []byte(fmt.Sprintf("%x", hash[:]))

	md5_shit := md5.Sum(my_bytes_for_converting2md5)

	/// lets fuck around with flag
	fmt.Printf("%x\n", md5_shit[:])
}
