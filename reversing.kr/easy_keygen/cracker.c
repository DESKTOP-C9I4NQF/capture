#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define errExit(...) do {		\
	fprintf(stderr, __VA_ARGS__);	\
	exit(EXIT_FAILURE);		\
} while (0)

char lookup_table[] = {
	0x10, 0x20, 0x30
};
int counter;

char *name2serial(char *name)
{
	size_t name_len = strlen(name);
	char* input_serial_number = malloc(name_len * 2 + 1);
	char *serial_copy = input_serial_number;

	for (counter = 0; *name != '\0'; name++, counter %= 3)
	{
		sprintf(input_serial_number, "%02X", *name ^ lookup_table[counter++]);
		input_serial_number += 2;
	}
	input_serial_number = '\0';
	return serial_copy;
}

char *serial2name(char *serial)
{
	char *flag_str, *input_serial_number, *copy_str;
	size_t input_serial_number_length;

	input_serial_number_length = strlen(serial);

	if (input_serial_number_length % 2 != 0)
		errExit("Invalid serial number %s :(\n", serial);

	flag_str = malloc(input_serial_number_length / 2 + 1);
	copy_str = flag_str;

	for (counter = 0; *serial != '\0'; serial += 2, counter %= 3)
	{
		sscanf(serial, "%02X", (int *)flag_str);
		*flag_str++ ^= lookup_table[counter++];
	}
	*flag_str = '\0';
// flag 
	return copy_str;
}


int main(int argc, char *argv[])
{
	char *str;
	if (argc != 3)
		 errExit("hex string is required in order to get Input Name :(\n");

	if (!strcmp(argv[1], "name"))
	{
		str = name2serial(argv[2]);
	}
	else if (!strcmp(argv[1], "serial"))
	{
		str = serial2name(argv[2]);
	}
	else 
	{
		exit(EXIT_FAILURE);
	}
	printf("%s\n", str);
	free(str);

	return 0;
}
