import rust_demangler
while True:
	data = input()
	if data == 'exit':
		break
	print(rust_demangler.demangle(data))
