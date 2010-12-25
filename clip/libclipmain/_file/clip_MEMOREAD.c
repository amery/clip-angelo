int
clip_MEMOREAD(ClipMachine * ClipMachineMemory)
{
   unsigned mode = 0;

   char buf[PATH_MAX];

   int fh, *err;

   char *ret;

   long len;

   char *fname = _clip_parc(ClipMachineMemory, 1);

   if (fname == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MEMOREAD");
   }
   _clip_translate_path(ClipMachineMemory, fname, buf, sizeof(buf));
   mode |= O_RDONLY;
   fh = _clip_open(ClipMachineMemory, buf, mode, 0, mode != O_RDONLY);

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);
   if (fh < 0)
   {
      *err = errno;
      close(fh);
      fh = -1;
   }
   else
   {
      *err = 0;
   }
   if (fh < 0)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   len = lseek(fh, 0, SEEK_END);
   lseek(fh, 0, SEEK_SET);
   ret = malloc(len + 1);
   len = read(fh, ret, len);

   if (len < 0)
      *err = errno;
   else
      *err = 0;
   _clip_close(ClipMachineMemory, _clip_hashstr(buf), fh);
   if (len <= 0)
   {
      free(ret);
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   if (len > 1 && ret[len - 1] == 0x1A)	/* Ctrl+Z */
      len--;
   ret[len] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, len);
   return 0;
}
