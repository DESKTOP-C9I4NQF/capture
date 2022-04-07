#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define AUTHER "KUSHAL"


int main(int argc, char **argv, char **envp)
{
	if (argc < 2) {
		fprintf(stderr, "Use file or folder name as\
 argmuent to get full path\n");
		exit(1);
	}

	char *pwd = getenv("PWD");

	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '/') {			

		/* check whether file name start '/' character
			 if there is '/' character directly jump below
			without executing anything */

			if(!strstr(argv[i], " ")) { 

				/* check whether there is no ' ' character */
					
				printf("%s\n", argv[i]);
			}
			else {
				printf("\"%s\"\n", argv[i]);
			}
		}
		else {
			if(!strstr(argv[i], " ") && !strstr(pwd, " ")) { 
				printf("%s/%s\n", pwd, argv[i]);
			}
			else {
				printf("\"%s/%s\"\n", pwd, argv[i]);
			}
		}
	}
	return 0;
}
