int
read_MemFile(const char *path, DbfLocale * tbl, DbfData ** resp, char ***names, int *rescount, char *errbuf, int errbuflen)
{
   int r = 0, i, line;

   FILE *file;

   MemHeader mh;

   DbfData *dp;

   file = fopen(path, "rb");
   if (!file)
      return mem_error(errbuf, errbuflen, __LINE__, "cannot open file '%s': %s", path, strerror(errno));

   for (i = 0;; ++i)
   {
      if (fread(&mh, sizeof(mh), 1, file) != 1)
      {
	 if (feof(file))
	    break;
	 line = __LINE__;
       readerr:
	 r = mem_error(errbuf, errbuflen, line, "file '%s' read error: %s", strerror(errno));
	 goto ret;
      }

      *resp = (DbfData *) realloc(*resp, ((*rescount) + 1) * sizeof(DbfData));
      *names = (char **) realloc(*names, ((*rescount) + 1) * sizeof(char *));

      dp = (*resp) + (*rescount);
      memset(dp, 0, sizeof(DbfData));
      (*names)[*rescount] = strdup((const char *) mh.name);
      ++(*rescount);

      //printf("\ntype=%d,%x,len=%d,name=%s\n",mh.type,mh.type,mh.len,mh.name);
      switch (mh.type)
      {
      case 'C':		/* string */
      case 0xC3:		/* string */
	 {
	    char *sp;

	    int len;

	    len = mh.len | (mh.dec << 8);

	    sp = (char *) malloc(len);
	    if (fread(sp, len, 1, file) != 1)
	    {
	       line = __LINE__;
	       free(sp);
	       goto readerr;
	    }

	    dp->type = 'C';
	    dp->len = mh.len;
	    dp->dec = mh.dec;
	    dp->flags = 0;
	    dp->u.c.str = sp;
	    dp->u.c.len = --len;

	    while (len)
	    {
	       int s = *(unsigned char *) sp;

	       if (tbl && s > 127)
		  *sp = tbl->read[s - 128];
	       --len;
	       ++sp;
	    }
	 }
	 break;
      case 'L':		/* logic */
      case 0xCC:		/* logic */
	 {
	    char ch;

	    if (fread(&ch, 1, 1, file) != 1)
	    {
	       line = __LINE__;
	       goto readerr;
	    }
	    dp->type = 'L';
	    dp->len = mh.len;
	    dp->dec = mh.dec;
	    dp->flags = 0;
	    dp->u.l = ch;
	 }
	 break;
      case 'N':		/* numeric */
      case 0xCE:		/* numeric */
	 {
	    double d = 0;

	    unsigned char buf[8];

	    int j;

	    if (fread(buf, 8, 1, file) != 1)
	    {
	       line = __LINE__;
	       goto readerr;
	    }
	    for (j = 0; j < 8; ++j)
	       ((unsigned char *) &d)[j] = buf[j];

	    dp->type = 'N';
	    dp->len = mh.len;
	    dp->dec = mh.dec;
	    dp->flags = 0;
	    dp->u.n = d;
	 }
	 break;
      case 'D':		/* date */
      case 0xC4:		/* date */
	 {
	    double d = 0;

	    unsigned char buf[8];

	    int j;

	    if (fread(buf, 8, 1, file) != 1)
	    {
	       line = __LINE__;
	       goto readerr;
	    }
	    for (j = 0; j < 8; ++j)
	       ((unsigned char *) &d)[j] = buf[j];

	    dp->type = 'D';
	    dp->len = mh.len;
	    dp->dec = mh.dec;
	    dp->flags = 0;
	    dp->u.d = d;
	 }
	 break;

      default:
	 r = mem_error(errbuf, errbuflen, __LINE__, "invalid mem field type : 0x%0x", mh.type);
	 goto ret;
      }

   }

 ret:
   fclose(file);

   return r;
}
