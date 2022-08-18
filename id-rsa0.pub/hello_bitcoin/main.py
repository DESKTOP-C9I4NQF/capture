import ecdsa, hashlib, base58
from binascii import unhexlify

def long_to_bytes(val, endianness="big"):
	# one (1) hex digit per four bits
	width = val.bit_length()

	# unhexlify wants an even multiple of eight (8) bits, but we on't
	# want more digits than we need (hence the ternary-ish 'or')
	width += 8 - ((width % 8) or 8)

	# format width specifier: four (4) bits per hex digit
	fmt = '%%0%dx' % (width // 4)

	# prepend zero (0) to the width, to zero-pad the output
	s = unhexlify(fmt % val)

	if endianness == 'little':
		s = s[::-1]

	return s

Private_key = long_to_bytes(94176137926187438630526725483965175646602324181311814940191841477114099191175)

# Private key to public key (ecdsa transformation)
signing_key = ecdsa.SigningKey.from_string(Private_key, curve = ecdsa.SECP256k1)
verifying_key = signing_key.get_verifying_key()
public_key = bytes.fromhex("04") + verifying_key.to_string()

# hash sha256 of pubkey
sha256_1 = hashlib.sha256(public_key)

# hash ripemd of sha of pubkey
ripemd160 = hashlib.new("ripemd160")
ripemd160.update(sha256_1.digest())

# checksum
hashed_public_key = bytes.fromhex("00") + ripemd160.digest()
checksum_full = hashlib.sha256(hashed_public_key).digest()
checksum = checksum_full[:4]
bin_addr = hashed_public_key + checksum

# encode address to base58 and print
result_address = base58.b58encode(bin_addr)
print("Bitcoin addr {}".format(result_address))
