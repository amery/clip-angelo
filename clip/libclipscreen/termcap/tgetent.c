int
tgetent(char *bp, char *t_name)
{
   char *tcap_addfile;

   tname = t_name;
   tcap_addfile = getenv("TERMCAP_ADDFILE");
   if (tcap_addfile)
   {
      strncpy(MYLCLTERMCAP, tcap_addfile, 255);
      MYLCLTERMCAP[255] = 0;
   }
   else
      strcpy(MYLCLTERMCAP, MY_LOCAL_TERMCAP);
   if (!envtermcap)
      envtermcap = getenv("TERMCAP");
   if (envtermcap && *envtermcap == '/')
   {
      tcapfile = envtermcap;
      envtermcap = 0;
   }
   if (!envtermcap)
      envtermcap = "";
   if (!tcapfile || access(tcapfile, 0) < 0)
      tcapfile = SHARETERMCAP;
   if (access(tcapfile, 0) < 0)
      tcapfile = TERMCAP;
   return (_tgetent(bp, tname, MYLCLTERMCAP) || _tgetent(bp, tname, MYTERMCAP) || _tgetent(bp, tname, tcapfile));
}
