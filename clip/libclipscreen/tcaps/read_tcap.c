int
read_tcap(int Fnum, char **Fnames, int tnum, Terminfo * infos, char *Errbuf, int Errbuflen)
{
   int i, r = 0;

   char buf[TBUFSIZE];

   Terminfo *info;

   fnum = Fnum;
   fnames = Fnames;
   errbuf = Errbuf;
   errbuflen = Errbuflen;
   errcode = 0;

   for (i = 0; i < tnum; ++i)
   {
      info = infos + i;

      match_count = 0;
      r = tgetent(buf, info->name, 0);
      if (r < 0 && errcode)
	 return -1;

      if (!r || !match_count)
      {
	 snprintf(Errbuf, Errbuflen, "no termcap entry for terminal '%s'", info->name);
	 return -1;
      }

      split_entry(info, buf);

   }

   return 0;
}
