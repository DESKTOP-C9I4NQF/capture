#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/syscall.h>


// process info struct contains information process id 
// of child process and also holds file descriptor that
// pipes which is used to communicate with child process
struct process_info
{
	// child process id
	pid_t child_pid;
	int talk_fd[2];


    // stores bytes as backup 
    size_t *bkup_bytes;

    // store required bytes which contains 0xcc 
    // to sigtrap give control to parent process
    // to execute
    size_t *rq_bytes;
};

void executeCLD(
    struct process_info* self,			// process info struct
    const char *prog_name,					// name of program to be executed
    void (*parent_exec_fcn)(struct process_info*) // parent function that should called 
    )
{
	int from_child[2], from_parent[2];

	// pipes for communicating with child
	pipe(from_child);
	pipe(from_parent);

	// fork a child process
	self->child_pid = fork();
	assert(self->child_pid >= 0);
	if (!self->child_pid)
	{
		// replace stdin and stdout with pipes
		dup2(from_parent[0], STDIN_FILENO);
		dup2(from_child[1], STDOUT_FILENO);

        // close unused file descriptors
		close(from_parent[0]);
		close(from_child[1]);

        // traced by the parent process
        ptrace(PTRACE_TRACEME, NULL, NULL, NULL);

        // execute  program 
		execl(prog_name, prog_name, NULL);
	}
	else
	{
		// close unused file descriptor
		close(from_child[1]);
		close(from_parent[0]);

		self->talk_fd[0] = from_child[0];
		self->talk_fd[1] = from_parent[1];

		parent_exec_fcn(self);

		close(from_child[0]);
		close(from_parent[1]);
	}
}
