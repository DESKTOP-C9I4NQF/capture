package main
import (
	"fmt"
)

// we use lookup table for given index
var specs = []byte("ABCDEFGHIJKLMNOPQRSTUVWXYZ ,.")
func extendedEuclid(A, B int, d *int, x *int, y *int) {
	var temp int
	if B == 0 {
		*d = A
		*x = 1
		*y = 0
	} else {
		extendedEuclid(B, A % B, d, x, y)
		temp = *x
		*x = *y
		*y = temp - (A / B) * (*y)
	}
}

// find inverse of A with extended euclid algorithm
func modInv(A, M int) (int) {
	var d, x, y int
	extendedEuclid(A, M, &d, &x, &y)
	if x < 0 { x += M }
	return x
}

// The number comes from the fact there are 12 numbers that are coprime with 26
// possible keys 26 x 12 = 312
func encrypt(thisstring []byte, ai, bi, mod int) {
	for idx := 0; idx < len(thisstring); idx++ {
		thisstring[idx] = specs[(ai * (int(thisstring[idx])-0x41) + bi) % mod]
	}
}

// by formula y = a^-1 * (x - b)
func decrypt(thisstring []byte, ai, bi, mod int) {
	var ai_inverse int = modInv(ai, mod)
	bi = mod - bi	// range go in negation so we subtract it from 26

	for idx := 0; idx < len(thisstring); idx++ {
		thisstring[idx] = specs[(ai_inverse * ((int(thisstring[idx]) - 0x41) + bi)) % mod]
	}
}

// substitute byte with its index
func subbyte(thisstring []byte) {
	for i := 0; i < len(thisstring); i++ {
		switch thisstring[i] {
		case ' ':
			thisstring[i] = 0x41 + 26

		case ',':
			thisstring[i] = 0x41 + 27

		case '.':
			thisstring[i] = 0x41 + 28
		}
	} 
}



func main() {
	var ciphertext []byte = []byte("BOHHIKBI,OZ,REI,WZRIKZIR,EX.,BOHI,RO,KISU,XSHO.R,ICBSG.WYISU,OZ, WZXZBWXS,WZ.RWRGRWOZ.,.IKYWZP,X.RKG.RIT,REWKT,DXKRWI.,RO,DKOBI..,ISIBRKOZWB,DXUHIZR.F,NEWSI,REI,.U.RIH,NOKA.,NISS,IZOGPE, OKHO.R,RKXZ.XBRWOZ.Q,WR,.RWSS,.G  IK., KOH,REI,WZEIKIZR,NIXAZI..I.,O ,REI,RKG.R,MX.IT,HOTISF,BOHDSIRISU,ZOZKIYIK.WMSI,RKXZ.XBRWOZ.,XKI,ZOR,KIXSSU,DO..WMSIQ,.WZBI, WZXZBWXS,WZ.RWRGRWOZ.,BXZZORXYOWT,HITWXRWZP,TW.DGRI.F,REI,BO.R,O ,HITWXRWOZ,WZBKIX.I.,RKXZ.XBRWOZ,BO.R.Q,SWHWRWZP,REIHWZWHGH,DKXBRWBXS,RKXZ.XBRWOZ,.WJI,XZT,BGRRWZP,O  ,REI,DO..WMWSWRU, OK,.HXSS,BX.GXS,RKXZ.XBRWOZ.QXZT,REIKI,W.,X,MKOXTIK,BO.R,WZ,REI,SO..,O ,XMWSWRU,RO,HXAI,ZOZKIYIK.WMSI,DXUHIZR., OK,ZOZKIYIK.WMSI.IKYWBI.F,NWRE,REI,DO..WMWSWRU,O ,KIYIK.XSQ,REI,ZIIT, OK,RKG.R,.DKIXT.F,HIKBEXZR.,HG.RMI,NXKU,O ,REIWK,BG.ROHIK.Q,EX..SWZP,REIH, OK,HOKI,WZ OKHXRWOZ,REXZ,REIU,NOGST,OREIKNW.I,ZIITF,X,BIKRXWZ,DIKBIZRXPI,O , KXGT,W.,XBBIDRIT,X.,GZXYOWTXMSIF,REI.I,BO.R.,XZT,DXUHIZR,GZBIKRXWZRWI.BXZ,MI,XYOWTIT,WZ,DIK.OZ,MU,G.WZP,DEU.WBXS,BGKKIZBUQ,MGR,ZO,HIBEXZW.H,ICW.R.,RO,HXAI,DXUHIZR.OYIK,X,BOHHGZWBXRWOZ.,BEXZZIS,NWREOGR,X,RKG.RIT,DXKRUF")
	var thisstring = make([]byte, len(ciphertext))

	subbyte(ciphertext)
	for idy := 0; idy < 29; idy++ {
		for idx := 0; idx < 29; idx++ {
			// copy ciphertext for decrypting with different
			// keys
			copy(thisstring, ciphertext)
			decrypt(thisstring, idy, idx, 29)
			fmt.Printf("%s\n", thisstring)
		}
	}
}