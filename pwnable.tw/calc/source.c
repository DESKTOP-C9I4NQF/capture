#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>


/* Read data from stdin */
void get_expr(uint8_t* str, uint32_t length)
{
	uint8_t byte;
	uint32_t count = 0;

	while (count < length && read(0, &byte, 1) > 0)
	{
		switch (byte)
		{
			case '+': case '*': case '-': case '/': case '%':
				*str++ = byte;
				break;
			case '\n':
				goto end;

			default:
				if (byte >= '0' && byte <= '9')
					*str++ = byte;
				break;
		}
	}
end:
	*str = '\0';
}

#define ADD(num1, num2) (num1 + num2)
#define SUB(num1, num2) (num1 - num2)
#define MUL(num1, num2) (num1 * num2)
#define DIV(num1, num2) (num1 / num2)

int32_t *eval(int32_t *arg1, char arg2)
{
	switch (arg2)
	{
		case '+': arg1[*arg1-1] = ADD(arg1[*arg1-1], arg1[*arg1]); break;
		case '*': arg1[*arg1-1] = MUL(arg1[*arg1-1], arg1[*arg1]); break;
		case '/': arg1[*arg1-1] = DIV(arg1[*arg1-1], arg1[*arg1]); break;
		case '-': arg1[*arg1-1] = SUB(arg1[*arg1-1], arg1[*arg1]); break;
	}
	(*arg1)--;
	return arg1;
}

void parse_expr(uint8_t* str, uint8_t* integer)
{
	uint32_t _operator_counter = 0;
	uint8_t _operators[0x64];
	uint32_t cur_idx = 0;
	int32_t *casted_integer = (int32_t *)integer;

	// clean before use
	memset(_operators, 0x0, 0x64);

	while (1)
	{
		if (str[curidx]-0x30 > 9)
		{
			uint32_t size = &str[cur_idx]-&str[0];
			uint8_t* ptr = malloc(size+1);
			int32_t eax_24;

			memcpy(ptr, &str[cur_idx], size);
			ptr[size] = '\0';

			if (!strcmp(ptr, "0"))
			{
				puts("prevent division by zero");
				fflush(stdout);
			}
			int32_t num1 = atoi(ptr);
			if (num1 > 0)
			{
				(*casted_integer)++;
				casted_integer[*casted_integer] = num1;
			}
			if (str[cur_idx] == 0 || (str[cur_idx] != 0 && str[cur_idx+1] - 0x30 <= 9))
			{
				uint8_t *newptr = &str[cur_idx+1];
				if (_operators[operator_counter] == 0)
					_operators[operator_counter] = str[cur_idx];
				else
				{
					int32_t eax_52 = (int32_t)(str[cur_idx] - 0x25);
					if (eax_52 > 0xa || eax_52 == 1 || eax_52 == 2 || eax_52 == 3 || eax_52 == 7 || eax_52 == 9)
						eval(integer, _operators[_operator_counter--]);

					if (eax_52 == 6 || eax_52 == 8)
					{
						eval(integer, _operators[_operator_counter]);
						_operators[_operator_counter] = str[cur_idx];;
					}
					if (eax_52 == 0 || eax_52 == 5 || eax_52 == 0xa)
					{
						if (_operators[_operator_counter] == 0x2b || _operators[_operator_counter] != 0x2d)
							_operators[++_operator_counter] = str[cur_idx];
						if (_operators[_operator_counter] != 0x2b && _operators[_operator_counter] != 0x2d)
						{		
							eval(integer, _operatros[_operator_counter++]);
							_operators[_operator_counter] = str[cur_idx];
						}
					}
				}
				if (str[cur_idx] == 0)
					for(; _operator_counter >= 0; _operator_counter--)
						eval(integer, str[cur_idx]);
					eax_24 = 1;
			}

		}
	}



}

void calc(void)
{
	uint8_t input_str[0x400];
	int32_t integer[108];

	while (1) {
		// bzero
		memset(input_str, 0, 0x400);

		// init_pool
		memset(&integer, 0, 108 * sizeof(uint32_t));

		get_expr(input_str, 0x400);
		parse_expr(input_str, &integer);

		printf("%s\n", input_str);
		fflush(stdout);
	}
}

int main(int argc, char *argv[])
{
	calc();
	return 0;
}
