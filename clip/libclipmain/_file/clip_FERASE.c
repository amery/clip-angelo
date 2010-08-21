int
clip_FERASE(ClipMachine * ClipMachineMemory)
{
   int ret, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   char buf[PATH_MAX];

   char *fname = _clip_parc(ClipMachineMemory, 1);

   if (fname == NULL)
   {
      _clip_retni(ClipMachineMemory, -1);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "FERASE");
   }
   _clip_translate_path(ClipMachineMemory, fname, buf, sizeof(buf));

#if 1
   /* I think I know what you wanted to do but this is very
      dangerous. POSIX defined that if process close a file
      all locks set by this process are removed. Kernel recognize
      locks by pair INODE & PID not by file handle. It means that
      if you run FERASE() on file which is opened database by
      the same process you remove all locks made by RDD :-(.
      The same happened if you try to remove any link
      (soft or hard) to this file.
    */
   {
      int fd;

      fd = _clip_open(ClipMachineMemory, buf, O_RDWR, 0, 1);
      if (fd < 0)
      {
	 *err = errno;
	 _clip_retni(ClipMachineMemory, -1);
	 return 0;
      }
      _clip_close(ClipMachineMemory, _clip_hashstr(buf), fd);
   }
#endif

   ret = unlink(buf);

   if (ret == -1)
      *err = errno;
   else
      *err = 0;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
