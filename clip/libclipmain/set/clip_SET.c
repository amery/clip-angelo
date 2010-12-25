int
clip_SET(ClipMachine * ClipMachineMemory)
{
   int opt = _clip_parni(ClipMachineMemory, 1);

   int lp;

   int argc = ClipMachineMemory->argc;

   char *sp;

   switch (opt)
   {
   case _SET_LOGLEVEL:
      if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	 _clip_retni(ClipMachineMemory, _clip_loglevel(_clip_parni(ClipMachineMemory, 2)));
      else
	 _clip_retni(ClipMachineMemory, _clip_loglevel(-1));
      break;
   case _SET_LOGFILE:
      if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      {
	 sp = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2));
	 _clip_log_init(sp);
	 free(sp);
	 _clip_retl(ClipMachineMemory, 1);
      }
      else
	 _clip_retl(ClipMachineMemory, 0);

      break;
   case _SET_EXACT:
      set_flag(ClipMachineMemory, EXACT_FLAG);
      break;
   case _SET_FIXED:
      set_flag(ClipMachineMemory, FIXED_FLAG);
      break;
   case _SET_DECIMALS:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->decimals);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 ClipMachineMemory->decimals = lp;
      }
      break;
   case _SET_FILECREATEMODE:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->fileCreateMode);
      lp = -1;
      if (argc > 1)
      {
	 if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
	    lp = _clip_fileStrModeToNumMode(_clip_parc(ClipMachineMemory, 2));
	 if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	    lp = _clip_parni(ClipMachineMemory, 2);
	 if (lp > 0)
	    ClipMachineMemory->fileCreateMode = lp;
      }
      break;
   case _SET_DIRCREATEMODE:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->dirCreateMode);
      lp = -1;
      if (argc > 1)
      {
	 if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
	    lp = _clip_fileStrModeToNumMode(_clip_parc(ClipMachineMemory, 2));
	 if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	    lp = _clip_parni(ClipMachineMemory, 2);
	 if (lp > 0)
	    ClipMachineMemory->dirCreateMode = lp;
      }
      break;
   case _SET_DATEFORMAT:
      _clip_retc(ClipMachineMemory, ClipMachineMemory->date_format);
      if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
      {
	 int i, ycount = 0;

	 free(ClipMachineMemory->date_format);
	 ClipMachineMemory->date_format = strdup(sp);
	 for (i = 0; sp[i] != 0; i++)
	 {
	    if (sp[i] == 'y' || sp[i] == 'Y')
	       ycount++;
	 }
	 if (ycount > 3)
	    ClipMachineMemory->flags |= CENTURY_FLAG;
	 else
	    ClipMachineMemory->flags &= ~CENTURY_FLAG;
      }
      break;
   case _SET_EPOCH:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->epoch);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 ClipMachineMemory->epoch = lp;
      }
      break;
   case _SET_PATH:
      _clip_retc(ClipMachineMemory, ClipMachineMemory->path);
      if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
      {
	 free(ClipMachineMemory->path);
	 ClipMachineMemory->path = strdup(sp);
      }
      break;
   case _SET_DEFAULT:
      _clip_retc(ClipMachineMemory, ClipMachineMemory->defaul);
      if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
      {
	 if (ClipMachineMemory->defaul)
	    free(ClipMachineMemory->defaul);
	 if (sp[0])
	    ClipMachineMemory->defaul = strdup(sp);
	 else
	    ClipMachineMemory->defaul = strdup(".");
      }
      break;
   case _SET_EXCLUSIVE:
      set_flag(ClipMachineMemory, EXCLUSIVE_FLAG);
      break;
   case _SET_SOFTSEEK:
      set_flag(ClipMachineMemory, SOFTSEEK_FLAG);
      break;
   case _SET_UNIQUE:
      set_flag(ClipMachineMemory, UNIQUE_FLAG);
      break;
   case _SET_DELETED:
      set_flag(ClipMachineMemory, DELETED_FLAG);
      _clip_set_deleted(ClipMachineMemory, !(ClipMachineMemory->flags & DELETED_FLAG));
      break;
   case _SET_CANCEL:
      return clip_SETCANCEL(ClipMachineMemory);
      break;
   case _SET_DEBUG:
      set_flag(ClipMachineMemory, DEBUG_FLAG);
      break;
   case _SET_TYPEAHEAD:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->typeahead);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 if (lp < 16)
	    lp = 16;
	 ClipMachineMemory->typeahead = lp;
	 ClipMachineMemory->kbdbuf = (int *) realloc(ClipMachineMemory->kbdbuf, lp * sizeof(int));

	 *ClipMachineMemory->kbdptr = ClipMachineMemory->kbdbuf;
      }
      break;

   case _SET_COLOR:
      {
	 char buf[64];

	 get_color(ClipMachineMemory, buf, sizeof(buf), 0);
	 _clip_retc(ClipMachineMemory, buf);
	 if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	 {
	    if (*sp)
	       set_color(ClipMachineMemory, sp);
	    else
	       set_color(ClipMachineMemory, DEFAULT_COLOR);
	 }
      }
      break;
   case _SET_CURSOR:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->cursor);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 ClipMachineMemory->cursor = lp;
      }
      break;
   case _SET_CONSOLE:
      set_flag(ClipMachineMemory, CONSOLE_FLAG);
      break;
   case _SET_ALTERNATE:
      set_flag(ClipMachineMemory, ALTERNATE_FLAG);
      break;
   case _SET_ALTFILE:
      _clip_retc(ClipMachineMemory, ClipMachineMemory->altfile);
      if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
      {
	 FILE *alt = (FILE *) ClipMachineMemory->alt;

	 char *mode;

	 char buf[PATH_MAX];

	 if (_clip_parl(ClipMachineMemory, 3))
	    mode = "a";
	 else
	    mode = "w";
	 if (alt && alt != stdout)
	    fclose(alt);
	 if (sp && sp[0])
	 {
	    if (_clip_path(ClipMachineMemory, sp, buf, sizeof(buf), 1))
	    {
	       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "%s: '%s'", strerror(errno), buf);
	       return 1;
	    }

	    _clip_addExtToFile(buf, PATH_MAX, "txt");
	    sp = strdup(buf);

	    if (access(sp, F_OK) != 0)
	       close(creat(sp, ClipMachineMemory->fileCreateMode));
	    alt = fopen(sp, mode);
	    if (!alt)
	    {
	       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "%s: '%s'", strerror(errno), sp);
	       return 1;
	    }
#ifdef _WIN32
	    setmode(fileno(alt), O_BINARY);
#endif
	    if (ClipMachineMemory->altfile != NULL)
	       free(ClipMachineMemory->altfile);
	    ClipMachineMemory->altfile = sp;
	    ClipMachineMemory->alt = alt;
	 }
	 else
	 {
	    if (ClipMachineMemory->altfile != NULL)
	       free(ClipMachineMemory->altfile);
	    ClipMachineMemory->alt = 0;
	    ClipMachineMemory->altfile = 0;
	 }
      }
      break;
   case _SET_DEVICE:
      _clip_retc(ClipMachineMemory, (ClipMachineMemory->flags & DEVICE_FLAG) ? "PRINTER" : "SCREEN");
      if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
      {
	 if (!strcasecmp(sp, "PRINTER"))
	    ClipMachineMemory->flags |= DEVICE_FLAG;
	 else if (!strcasecmp(sp, "PRINT"))
	    ClipMachineMemory->flags |= DEVICE_FLAG;
	 else
	    ClipMachineMemory->flags &= ~DEVICE_FLAG;
      }
      break;
   case _SET_EXTRA:
      set_flag(ClipMachineMemory, EXTRA_FLAG);
      break;
   case _SET_EXTRAFILE:
      _clip_retc(ClipMachineMemory, ClipMachineMemory->extrafile);
      if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
      {
	 FILE *extra = (FILE *) ClipMachineMemory->extra;

	 char *mode;

	 char buf[PATH_MAX];

	 if (_clip_parl(ClipMachineMemory, 3))
	    mode = "a";
	 else
	    mode = "w";
	 if (extra && extra != stdout)
	    fclose(extra);
	 if (sp && sp[0])
	 {
	    if (_clip_path(ClipMachineMemory, sp, buf, sizeof(buf), 1))
	    {
	       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "%s: '%s'", strerror(errno), buf);
	       return 1;
	    }

	    _clip_addExtToFile(buf, PATH_MAX, "txt");
	    sp = strdup(buf);

	    if (access(sp, F_OK) != 0)
	       close(creat(sp, ClipMachineMemory->fileCreateMode));
	    extra = fopen(sp, mode);
	    if (!extra)
	    {
	       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "%s: '%s'", strerror(errno), sp);
	       return 1;
	    }
	    if (ClipMachineMemory->extrafile != NULL)
	       free(ClipMachineMemory->extrafile);
#ifdef _WIN32
	    setmode(fileno(extra), O_BINARY);
#endif
	    ClipMachineMemory->extrafile = sp;
	    ClipMachineMemory->extra = extra;
	 }
	 else
	 {
	    if (ClipMachineMemory->extrafile != NULL)
	       free(ClipMachineMemory->extrafile);
	    ClipMachineMemory->extra = 0;
	    ClipMachineMemory->extrafile = 0;
	 }
      }
      break;
   case _SET_PRINTER:
      set_flag(ClipMachineMemory, PRINTER_FLAG);
      if (!(ClipMachineMemory->flags & PRINTER_FLAG) && ClipMachineMemory->printer)
	 fflush((FILE *) ClipMachineMemory->printer);
      break;
   case _SET_PRINTFILE:
      _clip_retc(ClipMachineMemory, ClipMachineMemory->prfile);
      if (argc <= 1)
	 break;

      _clip_close_printer(ClipMachineMemory);

      if (_clip_parinfo(ClipMachineMemory, 2) != CHARACTER_type_of_ClipVarType)
	 break;

      sp = _clip_parc(ClipMachineMemory, 2);
      if (!sp || !(*sp))
	 break;

      if (ClipMachineMemory->pbuf)
      {
	 destroy_Buf(ClipMachineMemory->pbuf);
	 free(ClipMachineMemory->pbuf);
	 ClipMachineMemory->pbuf = 0;
      }
      if (strncmp(sp, "MEMBUF", 6) == 0)
      {
	 ClipMachineMemory->pbuf = calloc(1, sizeof(OutBuf));
	 init_Buf(ClipMachineMemory->pbuf);
      }
      else
      {
	 /* create file for printer output */
	 FILE *printer;

	 char *mode;

	 if (_clip_parl(ClipMachineMemory, 3))
	    mode = "a";
	 else
	    mode = "w";

	 if (!strcasecmp(sp, "prn") || !strcasecmp(sp, "prn:")
#if 0
	     || !strcasecmp(sp, "lpt1") || !strcasecmp(sp, "lpt1:") || !strcasecmp(sp, "lpt2") || !strcasecmp(sp, "lpt2:") || !strcasecmp(sp, "lpt3") || !strcasecmp(sp, "lpt3:")
#else
	     || (!strncasecmp(sp, "lpt", 3) && strlen(sp) > 3 && sp[3] >= '1' && sp[3] <= '9')
#endif
	    )
	 {
	    char buf[256];

	    snprintf(buf, sizeof(buf), "prn-%s-%lx%lx", _clip_progname, (long) getpid(), (long) random());
	    ClipMachineMemory->real_prfile = strdup(buf);
	    ClipMachineMemory->prfile = strdup(sp);
	 }
	 else
	 {
	    char buf[PATH_MAX];

	    _clip_translate_path(ClipMachineMemory, sp, buf, sizeof(buf));
	    _clip_addExtToFile(buf, PATH_MAX, "prn");
	    ClipMachineMemory->real_prfile = ClipMachineMemory->prfile = strdup(buf);
	 }

	 if (access(ClipMachineMemory->real_prfile, F_OK) != 0)
	    close(creat(ClipMachineMemory->real_prfile, ClipMachineMemory->fileCreateMode));
	 printer = fopen(ClipMachineMemory->real_prfile, mode);
	 _clip_logg(2, "set print to:%s,%s", ClipMachineMemory->real_prfile, ClipMachineMemory->prfile);

	 if (!printer)
	 {
	    _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "cannot open printer file '%s': %s", sp, strerror(errno));
	    if (ClipMachineMemory->real_prfile != ClipMachineMemory->prfile)
	       free(ClipMachineMemory->real_prfile);
	    free(ClipMachineMemory->prfile);
	    ClipMachineMemory->real_prfile = 0;
	    ClipMachineMemory->prfile = 0;
	    return 1;
	 }
#ifdef _WIN32
	 setmode(fileno(printer), O_BINARY);
#endif
	 ClipMachineMemory->printer = printer;
	 set_printer_charset(ClipMachineMemory);
      }
      break;
   case _SET_MARGIN:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->margin);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 if (lp < 0)
	    lp = 0;
	 ClipMachineMemory->margin = lp;
      }
      break;

   case _SET_BELL:
      set_flag(ClipMachineMemory, BELL_FLAG);
      break;
   case _SET_CONFIRM:
      set_flag(ClipMachineMemory, CONFIRM_FLAG);
      break;
   case _SET_ESCAPE:
      set_flag(ClipMachineMemory, ESCAPE_FLAG);
      break;
   case _SET_INSERT:
      set_flag(ClipMachineMemory, INSERT_FLAG);
      break;
   case _SET_EXIT:
      set_flag(ClipMachineMemory, EXIT_FLAG);
      break;
   case _SET_INTENSITY:
      set_flag(ClipMachineMemory, INTENSITY_FLAG);
      break;
   case _SET_SCOREBOARD:
      set_flag(ClipMachineMemory, SCOREBOARD_FLAG);
      break;
   case _SET_DELIMITERS:
      set_flag(ClipMachineMemory, DELIMITERS_FLAG);
      break;
   case _SET_DELIMCHARS:
      _clip_retc(ClipMachineMemory, ClipMachineMemory->delimchars);
      if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
      {
	 free(ClipMachineMemory->delimchars);
	 ClipMachineMemory->delimchars = strdup(sp);
      }
      break;

   case _SET_WRAP:
      set_flag(ClipMachineMemory, WRAP_FLAG);
      break;
   case _SET_MESSAGE:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->msgline);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 if (lp < 0)
	    lp = 0;
	 ClipMachineMemory->msgline = lp;
      }
      break;
   case _SET_MCENTER:
      set_flag(ClipMachineMemory, MCENTER_FLAG);
      break;
   case _SET_SCROLLBREAK:
      set_flag(ClipMachineMemory, SCROLLBREAK_FLAG);
      break;
   case _SET_RATIONAL:
      set_flag(ClipMachineMemory, RATIONAL_FLAG);
      break;
   case _SET_TRANSLATE_PATH:
      set_flag(ClipMachineMemory, TRANSLATE_FLAG);
      break;
   case _SET_AUTOPEN:
      set_flag1(ClipMachineMemory, AUTOPEN_FLAG, 1, 0);
      break;
   case _SET_AUTORDER:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->autorder);
      ClipMachineMemory->autorder = _clip_parni(ClipMachineMemory, 2);
      break;
   case _SET_MBLOCKSIZE:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->mblocksize);
      ClipMachineMemory->mblocksize = _clip_parni(ClipMachineMemory, 2);
      break;
   case _SET_MAPERR:
      set_flag1(ClipMachineMemory, MAPERR_FLAG, 1, 0);
      break;
   case _SET_UTF8TERM:
      set_flag1(ClipMachineMemory, UTF8TERM_FLAG, 1, 0);
      break;
   case _SET_IDLE_INKEY:
      set_flag1(ClipMachineMemory, IDLEINKEY_FLAG, 1, 0);
      break;
   case _SET_DISPBOX:
      set_flag1(ClipMachineMemory, DISPBOX_FLAG, 1, 0);
      if (ClipMachineMemory->fullscreen)
      {
	 _clip_fullscreen(ClipMachineMemory);
	 setPgMode_Screen(ClipMachineMemory->screen, !(ClipMachineMemory->flags1 & DISPBOX_FLAG));
      }
      break;
   case _SET_OPTIMIZE:
      set_flag1(ClipMachineMemory, OPTIMIZE_FLAG, 1, 0);
      break;
   case _SET_MACRO_IN_STRING:
      set_flag1(ClipMachineMemory, NOEXPAND_MACRO_FLAG, 1, 1);
      break;
   case _SET_FOPENMODE:
      set_flag1(ClipMachineMemory, FOPENMODE_FLAG, 1, 0);
      break;
   case _SET_BUFFERING:
      set_flag1(ClipMachineMemory, BUFFERING_FLAG, 1, 0);
      break;
   case _SET_MAP_FILE:
      set_flag1(ClipMachineMemory, MAP_FILE_FLAG, 1, 0);
      break;
   case _SET_MULTILOCKS:
      set_flag1(ClipMachineMemory, MULTILOCKS_FLAG, 1, 0);
      break;
   case _SET_FLUSHOUT:
      set_flag1(ClipMachineMemory, FLUSHOUT_FLAG, 1, 0);
      break;
   case _SET_OPTIMIZELEVEL:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->optimizelevel + 1);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 if (lp == 1 || lp == 2)
	    ClipMachineMemory->optimizelevel = lp - 1;
      }
      break;
   case _SET_LOCKSTYLE:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->lockstyle);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 ClipMachineMemory->lockstyle = lp;
      }
      break;
   case _SET_HOURS:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->hours);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 ClipMachineMemory->hours = lp;
      }
      break;
   case _SET_SECONDS:
      _clip_retl(ClipMachineMemory, ClipMachineMemory->seconds);
      if (argc > 1)
      {
	 lp = 0;
	 if (_clip_parinfo(ClipMachineMemory, 2) == LOGICAL_type_of_ClipVarType)
	 {
	    lp = _clip_parl(ClipMachineMemory, 2);
	    //printf("\nset=logical,lp=%d\n",lp);
	 }
	 if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
	 {
	    if (strncasecmp("ON", _clip_parc(ClipMachineMemory, 2), 2) == 0)
	       lp = 1;
	    //printf("\nset=%s,lp=%d\n",_clip_parc(ClipMachineMemory,2),lp);
	 }
	 ClipMachineMemory->seconds = lp;
      }
      break;
   case _SET_EVENTMASK:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->eventmask);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 if (lp < 0)
	    lp = 0;
	 ClipMachineMemory->eventmask = lp;
      }
      break;
   case _SET_INDEX_BUFFER_LIMIT:
      _clip_retni(ClipMachineMemory, ClipMachineMemory->index_buffer_limit);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 if (lp < 0)
	    lp = 0;
	 ClipMachineMemory->index_buffer_limit = lp;
      }
      break;
   case _SET_ESC_DELAY:
      _clip_retni(ClipMachineMemory, esc_delay_Screen);
      if (argc > 1)
      {
	 lp = _clip_parni(ClipMachineMemory, 2);
	 if (lp > 0)
	    esc_delay_Screen = lp;
      }
      break;

   case _SET_ROOTPATH:
      _clip_retc(ClipMachineMemory, ClipMachineMemory->rootpath);
      if (ClipMachineMemory->rootpath == NULL && argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
      {
	 int len;

	 len = strlen(sp);
	 ClipMachineMemory->rootpath = malloc(len + 2);
	 strcpy(ClipMachineMemory->rootpath, sp);
	 if (sp[len - 1] == '/' || sp[len - 1] == '\\')
	    ;
	 else
	 {
#ifdef _WIN32
	    ClipMachineMemory->rootpath[len] = '\\';
#else
	    ClipMachineMemory->rootpath[len] = '/';
#endif
	    ClipMachineMemory->rootpath[len + 1] = 0;
	 }
      }
      break;
   default:
      {
	 char *name = _clip_parc(ClipMachineMemory, 1);

	 if (name)
	 {
	    char *val;

	    long hash;

	    hash = _clip_hashstr(name);
	    val = _clip_fetch_item(ClipMachineMemory, hash);
	    if (val)
	       _clip_retc(ClipMachineMemory, val);
	    else
	       _clip_retc(ClipMachineMemory, "");

	    if (argc > 1)
	    {
	       int len;

	       val = _clip_parcl(ClipMachineMemory, 2, &len);
	       if (val)
		  _clip_store_item(ClipMachineMemory, hash, _clip_memdup(val, len));
	    }

	    if (strlen(name) == 2 && name[1] == ':' && name[0] >= 'A' && name[0] <= 'Z')
	    {
	       int clip_INIT__CTOOLS_DISKFUNC(ClipMachine * ClipMachineMemory);

	       clip_INIT__CTOOLS_DISKFUNC(ClipMachineMemory);
	    }
	 }
      }
   }

   return 0;
}
