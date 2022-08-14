package main
import (
	"fmt"
	"bufio"
	_ "strings"
	"bytes"
	"os"
)

func toencrypt(plainText []byte, key byte) []byte {
	for i := 0; i < len(plainText); i++ {
		plainText[i] = ((plainText[i] - 0x41) + (key - 0x41)) % 26 + 0x41
	}
	return plainText
}

func todecrypt(cipherText []byte, key byte) []byte {
	return toencrypt(cipherText, 26-(key - 0x41) + 0x41)	// unsigned subtraction issue so we directly used already existing
								// code and it is good programming practice
}

func main() {
	rl := bufio.NewReader(os.Stdin)

	fmt.Printf("Get Me Text :> ")
	data,_, err := rl.ReadLine()
	if err != nil {
		panic(err)
	}

	// After analysing `THECAESARCIPHERISAREALLYOLDCIPHERWHICHISREALLYWEAKIMNOTSUREHOWOLDBECAUSEIMTOOLAZYTOLOOKATTHEWIKIPAGEATTHEMOMENTBUTIFIGUREITSGOTTOBEATLEASTLIKEFIFTYYEARSOLDORWHATEVERVAJDUXFCPVUSETHOSELASTTENCHARACTERSASTHESOLUTION'
	// flag : VAJDUXFCPV
	for i := 0; i < 26; i++ {
		fmt.Printf("string for key: %c, value: `%s'\n", 0x41 + i, todecrypt(bytes.ToUpper(data), 0x41 + byte(i)))
	}
}
