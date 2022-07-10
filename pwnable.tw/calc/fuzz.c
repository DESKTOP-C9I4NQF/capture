#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <assert.h>

#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#include <openssl/md5.h>

#define NUM_TIMES 10000

void free_array(void **array)
{
	size_t idx = 0;
	while (array[idx] != NULL)
		free(array[idx++]);
	free(array);
}

// Generate md5 from specfied string
void md5_hash_from_string(char* str, char* out)
{
	unsigned char hash[MD5_DIGEST_LENGTH];
	MD5_CTX c;

	// Initialize md5 struct with inittial  values
	MD5_Init(&c);
	MD5_Update(&c, str, strlen(str));		// uupdate initial value with string
	MD5_Final(hash, &c);				// finalize md5 struct and get hash of 32 bytes

	for (uint8_t idx = 0; idx < MD5_DIGEST_LENGTH; idx++) 
		out += sprintf(out, "%.02x", hash[idx]);
}


void **allocate_array(size_t length, size_t size)
{
	void **array;
	size_t idx;

	array = malloc(sizeof(void *) * (length + 1));
	assert(array != NULL);

	for (idx = 0; idx < length; idx++)
	{
		array[idx] = malloc(size);
		assert(array[idx] != NULL);
	}
	array[idx] = NULL;
	return array;
}

struct operation_info {
	uint32_t 	num1;
	uint8_t 	operator;
	uint32_t 	num2;
};

void fuzz()
{
	int sock_pair[2];
	uint8_t operators[] = {
		'+', '/', '-', '%', '*'
	};
	int wstatus, fild;
	uint32_t case_array[NUM_TIMES];
	size_t case_array_counter = 0;		// counter for case_array
	pid_t child_pid;

	for (size_t num_time = 0; num_time < NUM_TIMES; num_time++)
	{
		socketpair(AF_UNIX, SOCK_STREAM, 0, sock_pair);
		child_pid = fork();
		if (child_pid == 0)
		{
			char* argv[] = { "./calc", NULL };

			fild = open("/dev/null", O_WRONLY);

			dup2(sock_pair[0], STDIN_FILENO);
			dup2(fild, STDOUT_FILENO);
			dup2(fild, STDERR_FILENO);

			// close unused file descriptor
			close(sock_pair[1]);

			ptrace(PTRACE_TRACEME, 0, 0, 0);
			execve(argv[0], argv, NULL);
		}
		struct operation_info **operations;
		size_t counter;
		char str[4096], *ptr;

		// continue child process 
		wait(0); 
		ptrace(PTRACE_O_TRACEEXIT, child_pid, 0, 0);
		ptrace(PTRACE_CONT, child_pid, 0, 0);

		// clean before use
		memset(str, 0, 4096);

		// get allocate random number integers calculation
		// struct operation_info
		operations = (struct operation_info **)allocate_array((rand() % 1024) / 3 + 3, 
					sizeof(struct operation_info));
		counter = 0;

		time_t timer;
		time(&timer);

		// generate new seed with respect to time
		srand(timer);

		ptr = &str[0];
		while (operations[counter] != NULL)
		{
			operations[counter]->num1 = (uint32_t)rand() % 10;
			operations[counter]->num2 = (uint32_t)rand() % 10;
			operations[counter]->operator = operators[counter % sizeof(operators)];

			// append string each time.
			ptr += sprintf(ptr, "%d%c%d", 
					operations[counter]->num1, 
					operations[counter]->operator, 
					operations[counter]->num2);
			counter++;
		}
		dprintf(sock_pair[1], "%s\n\n", str);

		close(sock_pair[1]);
		close(sock_pair[0]);

		// wait for the file to exit with signaled 11 that is
		// SEGFAULT
		wait(&wstatus);
		wstatus >>= 8;
		if (wstatus != SIGKILL)
		{
			char out[2 * MD5_DIGEST_LENGTH +1];
			char tmp_file_str[496];
			uint32_t tmp;

			// get eip of child process
			tmp = (uint32_t)ptrace(PTRACE_PEEKUSER, child_pid, 13 * 8, 0);

			// loop through collection of eip that whether child process
			// id exists in the list 
//			for (size_t idx = 0; idx < case_array_counter; idx++)
//				if (tmp == case_array[idx])
//					goto end;
//
			case_array[case_array_counter++] = tmp;

			// get md5 hash from input string
			md5_hash_from_string(str, out);
			sprintf(tmp_file_str, "sample_test/tmp_%s", out);

			fild = open(tmp_file_str, O_WRONLY|O_CREAT, 0755);
			assert(fild >= 0);

			// store input string into file
			dprintf(fild, "%s\n\n", str);
			close(fild);
		}
end:
		free_array((void **)operations);
	}
}

int main(int argc, char* argv[])
{
	fuzz();
	return 0;
}
