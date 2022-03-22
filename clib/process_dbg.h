struct process_info
{
  // child process id
  pid_t child_pid;
  int talk_fd[2];
};

void executeCLD(
    struct process_info* self,      // process info struct
    const char *prog_name,          // name of program to be executed
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
  if (!self->child)
  {
    // replace stdin and stdout with pipes
    dup2(from_parent[0], STDIN_FILENO);
    dup2(from_child[1], STDOUT_FILENO);

    close(from_parent[0]);
    close(from_child[1]);

    execl(prog_name, prog_name, NULL);
  }
  else
  {
    // close unused file descriptor
    close(from_child[1]);
    close(from_parent[0]);

    talk_fd[0] = from_child[0];
    talk_fd[1] = from_parent[1];

    parent_exec_fcn(self);

    close(from_child[0]);
    close(from_parent[1]);
  }
}
