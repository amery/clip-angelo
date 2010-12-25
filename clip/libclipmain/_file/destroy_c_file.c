void
destroy_c_file(void *item)
{
   C_FILE *cf = (C_FILE *) item;

   if (cf == NULL)
      return;

   if (cf->file_of_C_FILE != NULL)
   {
      fflush(cf->file_of_C_FILE);
      fclose(cf->file_of_C_FILE);
   }
   else if (cf->type_of_C_FILE == FT_FILE)
      _clip_close(cf->ClipMachineMemory_of_C_FILE, cf->hash_of_C_FILE, cf->fileno_of_C_FILE);
   else
      close(cf->fileno_of_C_FILE);

#ifndef OS_MINGW
   if (cf->pid_of_C_FILE > 0)
      waitpid(cf->pid_of_C_FILE, NULL, 0);
#endif
   /* TODO? it can hang up main clip process.
      maybe some simple signal handler for caching children PIDs?
      Under Linux you can set SIGCHLD to SIG_IGN and this signal
      will be redirected to pid=1 process (init) or maybe we should
      use timeout value from C_FILE, too?
    */

   free(cf);
}
