static void
start_pipe(char *op, char *cmd)
{
   char buf[4096];

   if (pipe_flag)
      return;

   //if (!tmpnam(pipe_name))
   if (!mkstemp(pipe_name))
      return;

   pipe_tmp = fopen(pipe_name, "w");

   if (!pipe_tmp)
      return;

   pipe_flag = 1;

   snprintf(buf, sizeof(buf), "cat %s %s %s", pipe_name, op, cmd);

   free(pipe_command);
   pipe_command = strdup(buf);
}
