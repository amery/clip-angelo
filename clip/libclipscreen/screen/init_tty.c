int
init_tty(ScreenBase * base, int fd, char **envp, int Clear_on_exit, ScreenPgChars * pg_table, char *errbuf, int errbuflen)
{

   ScreenData *dp;

   char *p;

#ifndef OS_MINGW
   struct winsize ws;
#endif
   int i, tfd;

   int translateGraphMode = 0;

   static int first = 1;

   char *tcap;

   int fnum;

   char **fnames, *fbuf;

   int col = 0, row = 0;

   int ret = 0;

   memset(base, 0, sizeof(ScreenBase));

   base->fd = fd;
   base->clear_on_exit = Clear_on_exit;

   if (!isatty(fd))
   {
      snprintf(errbuf, errbuflen, "can run only on terminal");
      return -1;
   }

   p = get_env(envp, "TERM");
   if (!p)
      base->terminalName = strdup("ansi");
   else
      base->terminalName = strdup(p);

   dp = (ScreenData *) calloc(sizeof(ScreenData), 1);
   base->data = dp;

   init_Terminfo(&dp->terminfo);

   dp->terminfo.name = base->terminalName;

#ifdef _WIN32
#ifndef OS_MINGW
   p = get_env(envp, "CLIP_W32CONSOLE");
   if (p && *p)
   {
      if (!strcasecmp(p, "yes"))
      {
#endif
	 CONSOLE_SCREEN_BUFFER_INFO info;

	 if (w32_hStdOut == INVALID_HANDLE_VALUE)
	    w32_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	 if (w32_hStdIn == INVALID_HANDLE_VALUE)
	    w32_hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	 if (w32_hStdOut == INVALID_HANDLE_VALUE || w32_hStdIn == INVALID_HANDLE_VALUE)
	 {
	    snprintf(errbuf, errbuflen, "cannot take w32 console");
	    return -1;
	 }
	 /* set up mouse and window input */
	 if (!SetConsoleMode(w32_hStdIn, ENABLE_MOUSE_INPUT))
	 {
	    snprintf(errbuf, errbuflen, "cannot set w32 console mode");
	    return -1;
	 }
	 GetConsoleScreenBufferInfo(w32_hStdOut, &info);
	 base->Columns = info.dwSize.X;
	 base->Lines = info.dwSize.Y;
	 col = info.dwCursorPosition.X;
	 row = info.dwCursorPosition.Y;
	 w32_console = 1;
	 scr_scan_mode = ScanIoctl;
#ifndef OS_MINGW
      }
   }
#endif

#endif

   tcap = get_env(envp, "TERMCAP");
   if (!tcap)
      tcap = get_env(envp, "TERMINFO");
   fnum = FNUM + 2;
   fnames = (char **) alloca(sizeof(char *) * fnum);

   for (i = 0; i < FSTDNUM; i++)
      fnames[i] = FNAMES[i];
   fnames[FSTDNUM] = (char *) alloca(256);
   snprintf(fnames[FSTDNUM], 256, "%s/etc/terminfo", CLIPROOT);
   fnames[FSTDNUM + 1] = (char *) alloca(256);
   snprintf(fnames[FSTDNUM + 1], 256, "%s/etc/termcap", CLIPROOT);
   for (; i < FNUM; i++)
      fnames[i + 2] = FNAMES[i];

   if (tcap && *tcap)
   {
      char *slp = strchr(tcap, '/');

      char *clp = strchr(tcap, ':');

      char *vlp = strchr(tcap, '|');

      if (slp && (!clp || clp > slp) && (!vlp || (vlp > clp && vlp > slp)))
      {
	 /* tcap is a list of files */
	 char *s;

	 int l;

	 fnum = 0;
	 for (s = tcap; *s;)
	 {
	    l = strcspn(s, ":");
	    fnum++;
	    s += l;
	    if (!*s)
	       break;
	    s++;
	    l = strspn(s, ":");
	    s += l;
	 }

	 fnames = (char **) alloca(sizeof(char *) * fnum);

	 fbuf = (char *) alloca((l = (strlen(tcap) + 1)));
	 memcpy(fbuf, tcap, l);

	 fnum = 0;
	 for (s = fbuf; *s;)
	 {
	    fnames[fnum] = s;
	    l = strcspn(s, ":");
	    fnum++;
	    s += l;
	    if (!*s)
	       break;
	    *s++ = 0;
	    l = strspn(s, ":");
	    s += l;
	 }
	 if (read_term(fnum, fnames, 1, &dp->terminfo, errbuf, errbuflen))
	 {
#ifdef _WIN32
	    if (!w32_console)
#endif
	       return -1;
	 }
      }
      else
      {
	 /* tcap is a termcap entry */
	 if (read_tcapbuf(tcap, &dp->terminfo, errbuf, errbuflen))
	 {
#ifdef _WIN32
	    if (!w32_console)
#endif
	       return -1;
	 }
      }
   }
   else if (read_term(fnum, fnames, 1, &dp->terminfo, errbuf, errbuflen))
   {
#ifdef _WIN32
      if (!w32_console)
#endif
	 return -1;
   }

   p = get_env(envp, "LINS");
   if (p && *p)
      base->Lines = atoi(p);
   p = get_env(envp, "LINES");
   if (p && *p)
      base->Lines = atoi(p);
   p = get_env(envp, "COLS");
   if (p && *p)
      base->Columns = atoi(p);
   p = get_env(envp, "COLUMNS");
   if (p && *p)
      base->Columns = atoi(p);
   dp->fd = fd;
   init_ScreenData(dp);
   dp->base = base;

#ifdef _WIN32
   if (!w32_console)
   {
#endif
      if (!dp->termcap_CM)
      {
	 snprintf(errbuf, errbuflen, "too dumb terminal '%s' (no cursor move capabilitie)", base->terminalName);
	 free(base->terminalName);
	 destroy_ScreenData(dp);
	 free(dp);
	 return -1;
      }
#ifdef _WIN32
   }
#endif

#ifndef OS_MINGW
   if (ioctl(fd, TIOCGWINSZ, &ws) != -1)
   {
      if (!base->Lines && ws.ws_row > 0)
	 base->Lines = ws.ws_row;
      if (!base->Columns && ws.ws_col > 0)
	 base->Columns = ws.ws_col;
   }
#endif

   if (!base->Lines && dp->termcap_lines > 0)
      base->Lines = dp->termcap_lines;

   if (!base->Columns && dp->termcap_columns > 0)
      base->Columns = dp->termcap_columns;

   if (!base->Lines && !base->Columns)
   {
      base->Lines = 25;
      base->Columns = 80;
   }

   if (!base->Lines)
      base->Lines = 25;
   if (!base->Columns)
      base->Columns = 80;

   if (base->Lines < 2 || base->Columns <= 10)
   {
      snprintf(errbuf, errbuflen, "too small screen: is: %dx%d need: 2x10", base->Lines, base->Columns);
      return -1;
   }

   if (pg_table)
      memcpy(dp->pgTable, pg_table, 256);

   memcpy(base->pg_chars, ascii_pg_chars, PG_SIZE);
   squeesh_if_need(dp->termcap_CM);

   dp->scrool = dp->termcap_AL && dp->termcap_DL;
   if (!(dp->rscrool = dp->termcap_SF && dp->termcap_SR))
      dp->termcap_SF = dp->termcap_SR = 0;

   if (dp->termcap_ME)
   {
      if (dp->termcap_SO)
	 dp->termcap_SO = skipDelay(dp->termcap_SO);
      else if (dp->termcap_MR)
	 dp->termcap_SO = skipDelay(dp->termcap_MR);
   }

   dp->termcap_Visuals = 0;
   if ((dp->termcap_NF > 0 && dp->termcap_NB > 0 && dp->termcap_CF && (dp->termcap_CB || dp->termcap_C2)) || (dp->termcap_AF && dp->termcap_AB) || (dp->termcap_SETF && dp->termcap_SETB))
   {
      if (!dp->termcap_NF)
	 dp->termcap_NF = 16;
      if (!dp->termcap_NB)
	 dp->termcap_NB = 16;
      dp->termcap_Visuals |= VisualColors;
      if (dp->termcap_CF /* && strstr(dp->termcap_CF,"%p1") */ )
	 squeesh_if_need(dp->termcap_CF);
      if (dp->termcap_CB)
	 squeesh_if_need(dp->termcap_CB);
      if (dp->termcap_AB)
	 squeesh_if_need(dp->termcap_AB);
      if (dp->termcap_AF)
	 squeesh_if_need(dp->termcap_AF);
      if (dp->termcap_SETF)
	 squeesh_if_need(dp->termcap_SETF);
      if (dp->termcap_SETB)
	 squeesh_if_need(dp->termcap_SETB);
   }
   if (dp->termcap_MH)
      dp->termcap_Visuals |= VisualDim;
   if (dp->termcap_MD)
      dp->termcap_Visuals |= VisualBold;
   if (dp->termcap_mb)
      dp->termcap_Visuals |= VisualBlink;
   if (dp->termcap_SO)
      dp->termcap_Visuals |= VisualInverse;
   if (dp->termcap_G1 || dp->termcap_G2 || dp->termcap_AC)
      dp->termcap_Visuals |= VisualGraph;

   if (dp->termcap_Visuals & VisualColors)
      setColorMap(dp, "0123456789ABCDEF", "0123456789ABCDEF");
   else
      dp->termcap_CE = 0;	/* Don't use clear to end of line. */

   p = get_env(envp, "CLIP_COLORMAP");
   if (p && *p)
      setColorMap(dp, p, p);

   for (i = 0; i < 256; ++i)
      dp->inputTable[i] = i /*+ 256 */ ;
   for (i = 0; i < 256; ++i)
      dp->outputTable[i] = i /*+ 256 */ ;
   /*dp->outputTable[127] = 0xdf; */
   for (i = 0; i < 256; ++i)
      dp->uniTable[i] = i /*+ 256 */ ;

   if (dp->termcap_Ct && (tfd = open(dp->termcap_Ct, O_RDONLY)) >= 0)
   {
      read(tfd, dp->outputTable + 128 + 64, 64);	/* koi8 alpha's out translation */
      read(tfd, dp->nationalTable + 32, 96);	/* 96-RED CYRILLIC 128 - NORMAL CYRILLIC TRANSLATION */
      read(tfd, dp->inputTable + 128, 128);
      if (read(tfd, dp->outputTable + 128, 64) == 64)	/* koi8 pseudograph out translation */
	 translateGraphMode = 1;

      close(tfd);
   }

   {
      char *p1, *p2, *pp;

      cons_CharsetEntry *cs1 = 0, *cs2 = 0;

      int len1 = 0, len2 = 0;

#ifdef _WIN32
      /*char pibuf[12]; */
      char pobuf[12];
#endif

      p1 = get_env(envp, "CLIP_HOSTCS");
      p2 = get_env(envp, "CLIP_CLIENTCS");

#ifdef _WIN32
      if (w32_console)
      {
	 int cp_num;

	 /*
	    cp_num = GetConsoleCP();
	    snprintf(pibuf, sizeof(pibuf), "cp%d", cp_num);
	    if (!load_charset_name(pibuf, &cs1, &len1))
	    p1 = pibuf;
	  */

	 cp_num = GetConsoleOutputCP();
	 snprintf(pobuf, sizeof(pobuf), "cp%d", cp_num);
	 if (!load_charset_name(pobuf, &cs2, &len2))
	    p2 = pobuf;
      }
#endif
      if (!p1 || !p2 || !*p1 || !*p2)
	 goto norm;

      if (!cs1 && load_charset_name(p1, &cs1, &len1))
      {
	 snprintf(errbuf, errbuflen, "cannot load charset file '%s': %s", p1, strerror(errno));
	 ret = 1;
	 goto norm;
      }

      make_uniTable(cs1, len1, dp->uniTable);
      if (strcasecmp(p2, "UTF-8") == 0 ||
	  ((pp = get_env(envp, "LANG")) && (strstr(pp, ".UTF-8") != 0)) ||
	  ((pp = get_env(envp, "LC_ALL")) && (strstr(pp, ".UTF-8") != 0)) || ((pp = get_env(envp, "LC_CTYPE")) && (strstr(pp, ".UTF-8") != 0)))
      {
	 /*
	    if ((pp = get_env(envp, "TERM")) && (strncasecmp(pp, "xterm",5) == 0))
	  */
	 _clip_logg(3, "UTF terminal detected");
	 dp->utf8_mode = 1;
      }

      if (!cs2 && load_charset_name(p2, &cs2, &len2))
      {
	 snprintf(errbuf, errbuflen, "cannot load charset file '%s': %s", p2, strerror(errno));
	 ret = 2;
	 goto norm;
      }

      _clip_logg(3, "set out translation: %s -> %s", p1, p2);

      make_translation(cs1, len1, cs2, len2, dp->outputTable);
      make_translation(cs2, len2, cs1, len1, dp->inputTable);

      free(cs1);
      free(cs2);

    norm:
      ;
   }

   if (dp->termcap_Visuals & VisualGraph)
   {
      char *g1 = dp->termcap_G1;

      char *g2 = dp->termcap_G2;

      char *p;

      p = get_env(envp, "CLIP_LINECHARS");
      if (p && *p)
	 g1 = read_oct(p);
      p = get_env(envp, "CLIP_DLINECHARS");
      if (p && *p)
	 g2 = read_oct(p);

      if (g2 && !g1)
	 g1 = g2;
      if (g1 && !g2)
	 g2 = g1;

      if (translateGraphMode)
      {
	 /* use RFC KOI8 standart:
	    :g1=\200\201\204\211\205\206\212\207\202\210\203:
	    :g2=\240\241\253\273\256\261\276\265\245\270\250:
	  */
	 g1 = "\200\201\204\211\205\206\212\207\202\210\203";
	 g2 = "\240\241\253\273\256\261\276\265\245\270\250";
      }
      if (g1)
      {
	 i = strlen(g1);
	 if (i > 0)
	    base->pg_chars[PG_HLINE] = g1[0];
	 if (i > 1)
	    base->pg_chars[PG_VLINE] = g1[1];
	 if (i > 2)
	    base->pg_chars[PG_LLCORNER] = g1[2];
	 if (i > 3)
	    base->pg_chars[PG_BTEE] = g1[3];
	 if (i > 4)
	    base->pg_chars[PG_LRCORNER] = g1[4];
	 if (i > 5)
	    base->pg_chars[PG_LTEE] = g1[5];
	 if (i > 6)
	    base->pg_chars[PG_PLUS] = g1[6];
	 if (i > 7)
	    base->pg_chars[PG_RTEE] = g1[7];
	 if (i > 8)
	    base->pg_chars[PG_ULCORNER] = g1[8];
	 if (i > 9)
	    base->pg_chars[PG_TTEE] = g1[9];
	 if (i > 10)
	    base->pg_chars[PG_URCORNER] = g1[10];

	 if (g2)
	 {
	    i = strlen(g2);
	    if (i > 0)
	       base->pg_chars[PG_HLINE2] = g2[0];
	    if (i > 1)
	       base->pg_chars[PG_VLINE2] = g2[1];
	    if (i > 2)
	       base->pg_chars[PG_LLCORNER2] = g2[2];
	    if (i > 3)
	       base->pg_chars[PG_BTEE2] = g2[3];
	    if (i > 4)
	       base->pg_chars[PG_LRCORNER2] = g2[4];
	    if (i > 5)
	       base->pg_chars[PG_LTEE2] = g2[5];
	    if (i > 6)
	       base->pg_chars[PG_PLUS2] = g2[6];
	    if (i > 7)
	       base->pg_chars[PG_RTEE2] = g2[7];
	    if (i > 8)
	       base->pg_chars[PG_ULCORNER2] = g2[8];
	    if (i > 9)
	       base->pg_chars[PG_TTEE2] = g2[9];
	    if (i > 10)
	       base->pg_chars[PG_URCORNER2] = g2[10];

	 }
      }
      else if (dp->termcap_AC)
      {
	 int l, i;

	 char *s = dp->termcap_AC;

	 unsigned char *pg_chars = base->pg_chars;

	 l = strlen(s);
	 if (l % 2)
	    l--;

	 for (i = 0; i < l; i += 2)
	 {
	    switch (s[i])
	    {
	    case '}':
	       pg_chars[PG_STERLING] = s[i + 1];
	       break;
	    case '.':
	       pg_chars[PG_DARROW] = s[i + 1];
	       break;
	    case ',':
	       pg_chars[PG_LARROW] = s[i + 1];
	       break;
	    case '+':
	       pg_chars[PG_RARROW] = s[i + 1];
	       break;
	    case '-':
	       pg_chars[PG_UARROW] = s[i + 1];
	       break;
	    case 'h':
	       pg_chars[PG_BOARD] = s[i + 1];
	       break;
	    case '~':
	       pg_chars[PG_BULLET] = s[i + 1];
	       break;
	    case 'a':
	       pg_chars[PG_CKBOARD] = s[i + 1];
	       break;
	    case 'f':
	       pg_chars[PG_DEGREE] = s[i + 1];
	       break;
	    case 'z':
	       pg_chars[PG_GEQUAL] = s[i + 1];
	       break;
	    case '{':
	       pg_chars[PG_PI] = s[i + 1];
	       break;
	    case 'q':
	       pg_chars[PG_HLINE2] = pg_chars[PG_HLINE] = s[i + 1];
	       break;
	    case 'i':
	       pg_chars[PG_LANTERN] = s[i + 1];
	       break;
	    case 'n':
	       pg_chars[PG_PLUS2] = pg_chars[PG_PLUS] = s[i + 1];
	       break;
	    case 'y':
	       pg_chars[PG_LEQUAL] = s[i + 1];
	       break;
	    case 'm':
	       pg_chars[PG_LLCORNER2] = pg_chars[PG_LLCORNER] = s[i + 1];
	       break;
	    case 'j':
	       pg_chars[PG_LRCORNER2] = pg_chars[PG_LRCORNER] = s[i + 1];
	       break;
	    case '|':
	       pg_chars[PG_NEQUAL] = s[i + 1];
	       break;
	    case 'g':
	       pg_chars[PG_PLMINUS] = s[i + 1];
	       break;
	    case 'o':
	       pg_chars[PG_S1] = s[i + 1];
	       break;
	    case 'p':
	       pg_chars[PG_S3] = s[i + 1];
	       break;
	    case 'r':
	       pg_chars[PG_S7] = s[i + 1];
	       break;
	    case 's':
	       pg_chars[PG_S9] = s[i + 1];
	       break;
	    case '0':
	       pg_chars[PG_BLOCK] = s[i + 1];
	       break;
	    case 'w':
	       pg_chars[PG_TTEE2] = pg_chars[PG_TTEE] = s[i + 1];
	       break;
	    case 'u':
	       pg_chars[PG_RTEE2] = pg_chars[PG_RTEE] = s[i + 1];
	       break;
	    case 't':
	       pg_chars[PG_LTEE2] = pg_chars[PG_LTEE] = s[i + 1];
	       break;
	    case 'v':
	       pg_chars[PG_BTEE2] = pg_chars[PG_BTEE] = s[i + 1];
	       break;
	    case 'l':
	       pg_chars[PG_ULCORNER2] = pg_chars[PG_ULCORNER] = s[i + 1];
	       break;
	    case 'k':
	       pg_chars[PG_URCORNER2] = pg_chars[PG_URCORNER] = s[i + 1];
	       break;
	    case 'x':
	       pg_chars[PG_VLINE2] = pg_chars[PG_VLINE] = s[i + 1];
	       break;
	    }
	 }
      }
   }

   p = get_env(envp, "CLIP_SCANSTART");
   if (p && *p)
      scan_start = read_oct(p);

   p = get_env(envp, "CLIP_SCANSTOP");
   if (p && *p)
      scan_stop = read_oct(p);

   p = get_env(envp, "CLIP_KEYMAP");
   if (p && *p)
   {
      char path[256];

      FILE *file;

      snprintf(path, sizeof(path), "%s/keymaps/%s", CLIPROOT, p);

      file = fopen(path, "rt");
      if (!file)
      {
	 snprintf(errbuf, errbuflen, "cannot read keymap file '%s': %s", path, strerror(errno));
	 return -1;
      }
      if (load_keymap(file, errbuf, errbuflen))
      {
	 fclose(file);
	 return -1;
      }
      fclose(file);
   }

   p = get_env(envp, "CLIP_SCANMODE");
   if (p && *p)
   {
      if (!strcasecmp(p, "ioctl"))
	 scr_scan_mode = ScanIoctl;
      else if (!strcasecmp(p, "terminal"))
	 scr_scan_mode = ScanTerminal;
   }

#ifdef _WIN32
   if (!w32_console)
#endif
      tcgetattr(dp->fd, &dp->start_mode);
   if (first)
   {
      start_scan_mode(0);

#ifdef _WIN32
      if (!w32_console)
#endif
	 tcgetattr(0, &start_mode);
      first = 0;
      screen_data = dp;
      atexit(exit_tty);
   }

   restart_tty(base);
   base->realScreen = new_Screen(base);
   base->realScreen->y = row;
   base->realScreen->x = col;

#ifdef _WIN32
   if (w32_console)
   {
      CHAR_INFO p;

      int i;

      w32_size.X = base->Columns;
      w32_size.Y = base->Lines;
      w32_scrsize = base->Lines * base->Columns;
      w32_screen = (CHAR_INFO *) malloc(w32_scrsize * sizeof(CHAR_INFO));

      p.Char.AsciiChar = ' ';
      p.Attributes = 7;
      for (i = 0; i < w32_scrsize; i++)
	 w32_screen[i] = p;
      w32_beg.X = 0;
      w32_beg.Y = 0;
      w32_end.X = w32_size.X - 1;
      w32_end.Y = w32_size.Y - 1;
      if (Clear_on_exit)
      {
	 w32_flush();
      }
   }
   else
#endif
   {
#if 0
      termcap_put_raw_str(dp, dp->termcap_IS);
#endif

      termcap_put_raw_str(dp, dp->termcap_TI);
      termcap_put_raw_str(dp, dp->termcap_VE);
      termcap_put_raw_str(dp, dp->termcap_KS);
      termcap_put_raw_str(dp, dp->termcap_EA);
      termcap_put_raw_str(dp, dp->termcap_ME);

      termcap_flush(dp);

      termcap_set_color(dp, COLOR_WHITE | COLOR_BACK_BLACK);
      if (Clear_on_exit)
      {
	 termcap_clear_screen(dp);
	 termcap_flush(dp);
      }
   }

   init_mouse(base, envp);

   return ret;
}
