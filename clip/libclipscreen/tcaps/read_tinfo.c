int
read_tinfo(int fnum, char **fnames, int tnum, Terminfo * infos, char *Errbuf, int Errbuflen)
{
   int r = 0;

   int i, j, found;

   Terminfo *info;

   errbuf = Errbuf;
   errbuflen = Errbuflen;
   errcode = 0;

   for (i = 0; i < tnum; ++i)
   {
      found = 0;
      info = infos + i;
      r = 0;

      for (j = 0; j < fnum; ++j)
      {
	 char path[PATH_MAX];

	 snprintf(path, PATH_MAX, "%s" DELIMSTR "%c" DELIMSTR "%s", fnames[j], info->name[0], info->name);

	 r = read_info(info, path);
	 if (!r)
	    ++found;
      }

      if (!found)
      {
	 snprintf(Errbuf, Errbuflen, "no terminfo entry for terminal '%s'", info->name);
	 return -1;
      }

   }

   return 0;
}
