static int
load_pobj(ClipMachine * ClipMachineMemory, struct ClipFile *file, const char *name, int mallocMem)
{
   struct stat st;

   int fd;

   long len;

   char *sp, *end, *modbeg;

   memset(file, 0, sizeof(*file));

   if (stat(name, &st) != 0)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "cannot stat file '%s': %s", name, strerror(errno));
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   if (!S_ISREG(st.st_mode))
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "is not regular file '%s'", name);
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   len = st.st_size;
   if (len < 38)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "is not pobj file '%s'", name);
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   fd = open(name, O_RDONLY);
   if (fd < 0)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "open file '%s' error: %s", name, strerror(errno));
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   file->bodyMem_of_ClipFile = mallocMem;
   file->bodySize_of_ClipFile = len;
#ifdef HAVE_MMAN_H
   if (file->bodyMem_of_ClipFile == 0)
   {
      file->body_of_ClipFile = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
#ifndef OS_CYGWIN
      close(fd);
      fd = -1;
#endif
      file->fd_of_ClipFile = fd;
      if (file->body_of_ClipFile == (char *) -1)
      {
	 _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "mmap file '%s' error: %s", name, strerror(errno));
	 return _clip_call_errblock(ClipMachineMemory, -1);
      }
   }
   else
#endif
   {
      char *s;

      int readed, r;

      file->bodyMem_of_ClipFile = 1;
      file->body_of_ClipFile = (char *) malloc(len);
      for (s = file->body_of_ClipFile, readed = 0; readed < len; readed += r, s += r)
      {
	 int portion = 4096;

	 if ((len - readed) < portion)
	    portion = len - readed;
	 r = read(fd, s, portion);
	 if (r < portion)
	 {
	    close(fd);
	    free(file->body_of_ClipFile);
	    _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "read file '%s' error: %s", name, strerror(errno));
	    return _clip_call_errblock(ClipMachineMemory, -1);
	 }
	 close(fd);
	 file->fd_of_ClipFile = -1;
      }

   }

   sp = file->mem_of_ClipFile = file->body_of_ClipFile;
   end = sp + len;

   if (sp[0] == '#' && sp[1] == '!')
   {
      for (sp += 2; sp < end; sp++)
	 if (*sp == '\n')
	 {
	    sp++;
	    break;
	 }
   }

   file->body_of_ClipFile = sp;

   if (memcmp(sp, "!<pobj>\n", 8))
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, " '%s' is not valid pobj file: invalid magic", name);
      destroy_ClipFile(ClipMachineMemory, file);
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   sp += 8;
   sp += sizeof(long);

   /*
      if (GETLONG( sp != (len - 8 - 2 * sizeof(long))))
      {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, " '%s' is not valid pobj file: invalid length", name);
      destroy_ClipFile(ClipMachineMemory, file);
      return 1;
      }
    */
   sp += sizeof(long);

   modbeg = sp;
   sp += sizeof(long);

   file->nstatics_of_ClipFile = GETLONG(sp);

   file->ClipVar_statics_of_ClipFile = (ClipVar *) calloc(sizeof(ClipVar), file->nstatics_of_ClipFile + 1);
   file->staticsMem_of_ClipFile = 1;

   sp += sizeof(short) * 7 + sizeof(long) * 4;

   /*++sp; */
   file->name_of_ClipFile = strdup(sp);

   _clip_hash_buckets(ClipMachineMemory, file);

   return 0;
}
