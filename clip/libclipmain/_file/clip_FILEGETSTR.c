int
clip_FILEGETSTR(ClipMachine * ClipMachineMemory)
{
#if 0
   char *buf, *e;

   int *err;

   int h = _clip_parni(ClipMachineMemory, 1);

   int kol = _clip_parni(ClipMachineMemory, 2);

   int oldmesto = lseek(h, 0, SEEK_CUR);

   if (kol <= 0)
      kol = 256;

   buf = malloc(kol + 1);
   kol = read(h, buf, kol);

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   if (kol < 0)
      *err = errno;
   else
      *err = 0;

   if (kol < 0)
   {
      _clip_retc(ClipMachineMemory, "");
      lseek(h, oldmesto, SEEK_SET);
      return 0;
   }
   for (e = buf; e < buf + kol && *e != '\n'; e++);
   if (e == buf + kol)
   {
      _clip_retcn_m(ClipMachineMemory, buf, kol);
      return 0;
   }
   if (*(e - 1) == '\r')
   {
      *(e - 1) = 0;
      _clip_retcn_m(ClipMachineMemory, buf, e - buf - 1);
      lseek(h, oldmesto + e - buf + 2, SEEK_SET);
      return 0;
   }
   *e = 0;
   _clip_retcn_m(ClipMachineMemory, buf, e - buf);
   lseek(h, oldmesto + e - buf + 1, SEEK_SET);
#else
   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   char *buf = NULL;

   int fd = _clip_parni(ClipMachineMemory, 1);

   int i, c = 0, n = _clip_parni(ClipMachineMemory, 2);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   if (cf == NULL)
   {
      *err = EBADF;
      _clip_retc(ClipMachineMemory, "");
   }
   else
   {
      *err = 0;
      if (n <= 0)
	 n = 256;
      buf = calloc(n + 1, 1);
      if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
      {
	 if (cf->type_of_C_FILE == FT_FILE)
	 {
	    int j;

	    j = _clip_read(cf, buf, n);
	    if (j < n)
	       n = j;
	    for (i = j = 0; j < n;)
	    {
	       c = *(buf + j++);
	       if (c == '\n')
		  break;
	       if (c != '\r')
		  *(buf + i++) = c;
	    }
	    if (n > 0 && j < n)
	       lseek(cf->fileno_of_C_FILE, j - n, SEEK_CUR);
	    else if (n == -1)
	       *err = errno;
	 }
	 else
	 {
	    for (i = 0; i < n;)
	    {
	       if (_clip_read(cf, buf + i, 1) <= 0)
		  break;
	       c = *(buf + i);
	       if (c == '\n')
		  break;
	       if (c != '\r')
		  ++i;
	    }
	 }
      }
      else
      {
/*
			if (fgets(buf, n + 1, cf->f) != NULL)
			{
				for (i = strlen(buf); i; )
				{
					c = *(buf + --i);
					if (c != EOF && c != '\n' && c != '\r')
						break;
				}
				*(buf + i) = '\0';
			}
*/
	 for (i = 0; i < n;)
	 {
	    c = fgetc(cf->file_of_C_FILE);
	    if (c == EOF || c == '\n')
	       break;
	    if (c != '\r')
	       buf[i++] = c;
	 }
      }
      if (i > 0)
      {
	 buf = (char *) realloc(buf, i + 1);
	 _clip_retcn_m(ClipMachineMemory, buf, i);
      }
      else
      {
	 free(buf);
	 _clip_retc(ClipMachineMemory, "");
      }
   }
#endif
   return 0;
}
