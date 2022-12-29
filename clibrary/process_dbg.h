#ifndef __PROCESS_DBG__
#define __PROCESS_DBG__ 1

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>

#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/syscall.h>



// process info struct contains information process id 
// of child process and also holds file descriptor that
// pipes which is used to communicate with child process
struct process_info
{
	bool ptrace_enabled;
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

struct MemFileStruct {
	void *base;
	char fname[0xfff];
};

void GetFileInfo(struct process_info* pinfo, struct MemFileStruct *minfo, char* fname)
{
	char *line = NULL, proc_maps_file[256];
	size_t size = 0;
	FILE* fp;

	snprintf(proc_maps_file, 256, "/proc/%d/maps", pinfo->child_pid);
	fp = fopen(proc_maps_file, "r");
	if (fp == NULL) {
		fprintf(stderr, "Unable to open %s :(\n", proc_maps_file);
		exit(EXIT_FAILURE);
	}

	while (getline(&line, &size, fp) != -1)
	{
		if (strstr(line, fname) != NULL)
		{
			size_t from, to, major, minor, ino, flags, pageoff;
			sscanf(line, "%lx-%lx %4c %lx %lx:%lx %lu %s", 
					&from,
					&to,
					(char*)&flags,
					&pageoff,
					&minor,
					&major,
					&ino,
					minfo->fname);

			minfo->base = (void*)from;
			goto out;
		}
	}
	minfo->fname[0] = '\0';
	minfo->base = NULL;
out:
	// close open file descriptor
	fclose(fp);
	free(line);
}

// do giant read 
size_t GiantRead(struct process_info* pinfo, void* addr) 
{
	return ptrace(PTRACE_PEEKTEXT, pinfo->child_pid, addr, NULL);
}

// do Giant write
void GiantWrite(struct process_info *pinfo, void* addr, void* data)
{
	ptrace(PTRACE_PEEKTEXT, pinfo->child_pid, addr, &pinfo->bkup_bytes);
	ptrace(PTRACE_POKETEXT, pinfo->child_pid, addr, data);
}

void executeCLD(
    struct process_info* self,			// process info struct
    char** argv,
    char** envp,

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
        if (self->ptrace_enabled) ptrace(PTRACE_TRACEME, NULL, NULL, NULL);

        // execute  program 
	execve(argv[0], argv, envp);
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

#endif
