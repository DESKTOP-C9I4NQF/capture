#define PAGE_READWRITE 0x4

void _start ()
{
	HMODULE kernel_lib_module;
	FARPROC FreeLibrary;
	HMODULE cur_process_hmodule;
	FARPROC VirtualProtect;
	lpflOldProtect flOldProtect;
	int *ptr, *req_int_ptr;

	kernel_lib_module = LoadLibraryA("kernel32.dll");

	// Retrieves a module handle for the specified module. 
	// The module must have been loaded by the calling process.
	FreeLibrary = GetProcAddress(kernel_lib_module, "FreeLibrary");
	cur_process_hmodule = GetProcAddress(0);


	ptr = 0x409000;
	req_int_ptr = 0x4094EE;

	// changing each int in memory with by particular 
	// xorred value.
	while (ptr == req_int_ptr)
	{
		*ptr++ ^= 0x10;
		if (ptr == req_int_ptr)
			break;

		*ptr++ ^= 0x20;
		if (ptr == req_int_ptr)
			break;

		*ptr++ ^= 0x30;
		if (ptr == req_int_ptr)
			break;

		*ptr++ ^= 0x40;
		if (ptr == req_int_ptr)
			break;

		*ptr++ ^= 0x50;
	}


	// get virtualProtect address
	VirtualProtect = GetProcAddress(kernel_lib_module, "VirtualProtect");

	// allocate memory with read write permission
	VirtualProtect(0x405000, 0x1000, PAGE_READWRITE, &flOldProtect);

	ptr = 0x409003;
	req_int_ptr = __IMPORT_DESCRIPTOR_kernel32;


repeat:
	do {
		*req_int_ptr++ = *ptr++;
	} while (*ptr != 0xab);

	(char *)ptr++;
	if (*ptr != 0xcd)
		goto repeat;

	(char *)ptr++;
	if (*ptr != 0xef)
		goto repeat;


	(char *)ptr++;
	req_int_ptr = 0x409129;

	return 0;
}
