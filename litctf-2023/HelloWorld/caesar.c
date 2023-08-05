#include <stdio.h>
#include <stdlib.h>


void caesar_cipher(char *inp, char *out, int key)
{
	for (; *inp != '\0'; inp++, out++)
	{
		if (*inp >= 'A' && *inp <= 'Z')
			*out = ((*inp)-'A' + key) % 26 + 'A';

		else if (*inp >= 'a' && *inp <= 'z')
			*out = ((*inp)-'a' + key) % 26 + 'a';

		else { *out = *inp; }
	}
	*out = '\0';
}

int main(int argc, char *argv[])
{
	char inp[] = "YVGPGS{J3yp0z3_G0_Y1GPGS}";
	char out[sizeof(inp)]; 
	for (int idx = 1; idx < 27; idx++)
	{
		caesar_cipher(inp, out, idx);
		printf("%s:%02d\n", out, idx);
	}
	return 0;
}
