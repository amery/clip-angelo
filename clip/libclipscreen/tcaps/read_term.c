int
read_term(int fnum, char **fnames, int tnum, Terminfo * infos, char *errbuf, int errbuflen)
{
   struct stat st;

   int r, i;

   for (i = 0; i < fnum; ++i)
   {
      r = stat(fnames[i], &st);
      if (!r)
      {
	 if (S_ISDIR(st.st_mode))
	    return read_tinfo(fnum, fnames, tnum, infos, errbuf, errbuflen);
	 else if (S_ISREG(st.st_mode))
	    return read_tcap(fnum, fnames, tnum, infos, errbuf, errbuflen);
      }
   }

   snprintf(errbuf, errbuflen, "no valid termcap/terminfo files:");
   for (i = 0; i < fnum; ++i)
   {
      r = strlen(errbuf);
      snprintf(errbuf + r, errbuflen - r, " %s", fnames[i]);
   }
   return -1;
}
