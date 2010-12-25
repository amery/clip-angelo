int
write_MemFile(const char *path, DbfLocale * tbl, DbfData * arr, char **names, int acount, char *errbuf, int errbuflen)
{
   int r = 0, i, line;

   FILE *file;

   MemHeader mh;

   DbfData *dp;

   file = fopen(path, "wb");
   if (!file)
      return mem_error(errbuf, errbuflen, __LINE__, "cannot open file '%s': %s", path, strerror(errno));

   for (i = 0; i < acount; ++i)
   {
      dp = arr + i;

      memset(&mh, 0, sizeof(mh));
      strncpy((char *) mh.name, names[i], 10);

      switch (dp->type)
      {
      case 'C':
	 {
	    int len;

	    len = dp->u.c.len + 1;
	    mh.type = 0xC3;
	    mh.len = len & 0xff;
	    mh.dec = (len >> 8) & 0xff;
	 }
	 break;
      case 'L':
	 mh.type = 0xCC;
	 mh.len = 1;
	 break;
      case 'N':
	 mh.type = 0xCE;
	 mh.len = dp->len;
	 mh.dec = dp->dec;
	 break;
      case 'D':
	 mh.type = 0xC4;
	 mh.len = 1;
	 break;
      default:
	 continue;
      }

      if (fwrite(&mh, sizeof(mh), 1, file) != 1)
      {
	 line = __LINE__;
       writerr:
	 r = mem_error(errbuf, errbuflen, line, "file '%s' write error: %s", strerror(errno));
	 goto ret;
      }

      switch (mh.type)
      {
      case 0xC3:		/* string */
	 {
	    char *sp;

	    int len;

	    int j;

	    len = dp->u.c.len + 1;
	    sp = dp->u.c.str;

	    for (j = 0; j < len; ++j, ++sp)
	    {
	       int s = *(unsigned char *) sp;

	       if (tbl && s > 127)
		  s = tbl->write[s - 128];
	       if (fputc(s, file) == EOF)
	       {
		  line = __LINE__;
		  goto writerr;
	       }
	    }
	 }
	 break;
      case 0xCC:		/* logic */
	 {
	    char ch;

	    ch = dp->u.l;
	    if (fputc(ch, file) == EOF)
	    {
	       line = __LINE__;
	       goto writerr;
	    }
	 }
	 break;
      case 0xCE:		/* numeric */
	 {
	    double d;

	    unsigned char buf[8];

	    int j;

	    d = dp->u.n;
	    for (j = 0; j < 8; ++j)
	       buf[j] = ((unsigned char *) &d)[j];
	    if (fwrite(buf, 8, 1, file) != 1)
	    {
	       line = __LINE__;
	       goto writerr;
	    }
	 }
	 break;
      case 0xC4:		/* date */
	 {
	    double d;

	    unsigned char buf[8];

	    int j;

	    d = dp->u.d;
	    for (j = 0; j < 8; ++j)
	       buf[j] = ((unsigned char *) &d)[j];
	    if (fwrite(buf, 8, 1, file) != 1)
	    {
	       line = __LINE__;
	       goto writerr;
	    }
	 }
	 break;
      }
   }

 ret:
   fclose(file);

   return r;
}
