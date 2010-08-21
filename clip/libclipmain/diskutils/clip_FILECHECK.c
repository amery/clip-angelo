/*
 * Calculates/computes/determines a checksum for a file
 */
int
clip_FILECHECK(ClipMachine * ClipMachineMemory)
{
   char *uname = NULL;

   FILE *f = NULL;

   int c = 0, checksum = 0, fd = -1;

   if (ClipMachineMemory->argc < 1)
   {
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf( ClipMachineMemory, __FILE__, __LINE__, "invalid argument" );*/
      return 1;
   }
   if ((uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1))) == NULL || (fd = open(uname, O_RDONLY)) == -1 || !_set_lock(fd, F_RDLCK) || (f = fdopen(fd, "r")) == NULL)
   {
      _clip_retni(ClipMachineMemory, -1);
      goto end;
   }

#define ROTATE_RIGHT(c) if ((c) & 01) (c) = ((c) >>1) + 0x8000; else (c) >>= 1;
   while ((c = getc(f)) != EOF)
   {
      ROTATE_RIGHT(checksum);
      checksum += c;
      checksum &= 0xffff;
   }
#undef ROTATE_RIGHT

 end:
   _clip_retni(ClipMachineMemory, checksum);
   if (uname != NULL)
      free(uname);
   if (f != NULL)
      fclose(f);
   return 0;
}
