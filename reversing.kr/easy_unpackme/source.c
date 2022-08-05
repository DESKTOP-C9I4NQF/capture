#define PAGE_READWRITE 0x4
#define __IMPORT_DESCRIPTOR_kernel32 0x405464

void _start ()
{
	// order of allocation doesn't matter right now
	HMODULE kernel_lib_module;
	FARPROC FreeLibrary;
	HMODULE cur_process_hmodule;
	FARPROC VirtualProtect;
	lpflOldProtect flOldProtect;
	char *ptr, *req_int_ptr;

	kernel_lib_module = LoadLibraryA("kernel32.dll");

	// Retrieves a module handle for the specified module. 
	// The module must have been loaded by the calling process.
	FreeLibrary = GetProcAddress(kernel_lib_module, "FreeLibrary");
	cur_process_hmodule = GetProcAddress(0);


	ptr = 0x409000;
	req_int_ptr = 0x4094EE;

	// changing each char in memory with by particular 
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
		*(int *)req_int_ptr++ = *(int *)ptr++;
	} while (*ptr != 0xab);

	if (*++ptr != 0xcd)
		goto repeat;

	if (*++ptr != 0xef)
		goto repeat;
	ptr++;


	req_int_ptr = 0x409129;
	int* loaded_library;

	while (1)
	{
		if (*req_int_ptr == 0xac)
			goto task;

		if (*(req_int_ptr+1) == 0xdf)
			goto task;

		req_int_ptr += 2;
		loaded_library = LoadLibraryA(req_int_ptr);

		while (*req_int_ptr++);
task:
		// loaded function
		function = GetProcAddress(loaded_library, *(int *)req_int_ptr++);

		while(*req_int_ptr++);
		if (req_int_ptr == 0x4094EC)
			break;
	}

	VirtualProtect(0x401000, 0x4000, PAGE_READWRITE, &flOldProtect);

	ptr = 0x401000;
	req_int_ptr = 0x405000;
	req_int_ptr = 0x4094EE;

	// changing each char in memory with by particular 
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

	// We are not interested in things
	do_something_of_nointerest();
	return 0;
}
