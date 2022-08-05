#!/usr/bin/env	python3
import hashlib
required_str = b"id0-rsa.pub"

sha256_version_of_string = hashlib.sha256(required_str).hexdigest().encode()
md5version_of_hexdigest = hashlib.md5(sha256_version_of_string).hexdigest()

# required flag 
print(md5version_of_hexdigest)
