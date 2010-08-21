/*
 *      FILESTR(<cFile>, [<nLength>], [<nOffset>],
 *	  [<lCtrl-Z>]) --> cCharacterstring
 */
int
clip_FILESTR(ClipMachine * ClipMachineMemory)	/* Reads a portion of a file into a string */
{
   char *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   char *ret = NULL;

   long nlen = _clip_parnl(ClipMachineMemory, 2);

   long noff = _clip_parnl(ClipMachineMemory, 3);

   long n = 0;

   int c = 0, ctrl_z = _clip_parl(ClipMachineMemory, 4);

   int fd = -1;

   FILE *f = NULL;

   if (uname == NULL || noff < 0 || nlen < 0)
      goto end;

   if (nlen == 0)
      nlen = 0xFFFF;

   if ((fd = open(uname, O_RDONLY)) == -1 || !_set_lock(fd, F_RDLCK) || (f = fdopen(fd, "r")) == NULL || fseek(f, noff, SEEK_SET) || (ret = (char *) malloc(nlen)) == NULL)
      goto end;

   if (!ctrl_z)
   {
      n = fread(ret, 1, nlen, f);
   }
   else
      while ((c = fgetc(f)) != EOF)
      {
	 if (n >= nlen || c == 26 /* ctrl_z */ )
	    break;
	 ret[n++] = c;
      }

   fclose(f);
 end:
   if (uname != NULL)
      free(uname);

   _clip_retcn_m(ClipMachineMemory, ret, (ret ? n : 0));

   return 0;
}
