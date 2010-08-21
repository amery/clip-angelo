int
clip_INIT_FILE_SETS(ClipMachine * ClipMachineMemory)
{
   C_FILE *cf = NULL;

   int fd;

   int *err;

   err = calloc(sizeof(int), 1);

   _clip_store_item(ClipMachineMemory, HASH_ferror, err);
   err = calloc(sizeof(int), 1);

#ifdef _WIN32
   setmode(0, O_BINARY);
   setmode(1, O_BINARY);
#endif

   /*setvbuf(stdout, (char *)NULL, _IOFBF, 0); */
   /* open stdin, stdout, stderr */
   for (fd = 0; fd < 3; fd++)
   {
      cf = (C_FILE *) calloc(1, sizeof(C_FILE));
      if (fd == 0)
	 cf->file_of_C_FILE = stdin;
      else if (fd == 1)
	 cf->file_of_C_FILE = stdout;
      else if (fd == 2)
	 cf->file_of_C_FILE = stderr;
      cf->fileno_of_C_FILE = fd;
      cf->pid_of_C_FILE = -1;
      cf->type_of_C_FILE = _clip_ftype(fd);
      cf->timeout_of_C_FILE = -1;
      cf->stat_of_C_FILE = 0;
      _clip_store_c_item(ClipMachineMemory, cf, _C_ITEM_TYPE_FILE, destroy_c_file);
   }
   return 0;
}
