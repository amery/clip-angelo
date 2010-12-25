CLIP_DLLEXPORT struct ClipMachine *
new_ClipMachine(struct Screen *screen)
{
   ClipMachine *ret;

#ifdef MEMDEBUG
   {
      static int inited = 0;

      if (!inited)
      {
	 malloc_logpath = "_memdebug";
	 malloc_debug(MEMDEBUG);
	 inited = 1;
      }
   }
#endif

   ret = NEW(ClipMachine);
   if (all_functions)
      ret->functions = all_functions;
   else
      ret->functions = all_functions = new_HashTable();
   ++machineCount;

   ret->dbdrivers = &all_dbdrivers;
   ret->ndbdrivers = &all_ndbdrivers;
   ret->data_drivers = &all_data_drivers;
   ret->ndata_drivers = &all_ndata_drivers;
   ret->idx_drivers = &all_idx_drivers;
   ret->nidx_drivers = &all_nidx_drivers;
   ret->memo_drivers = &all_memo_drivers;
   ret->nmemo_drivers = &all_nmemo_drivers;
   ret->def_data_driver = all_def_data_driver;
   ret->def_idx_driver = all_def_idx_driver;
   ret->def_memo_driver = all_def_memo_driver;
   ret->def_db_driver = all_def_db_driver;

   ret->sqldrivers = &all_sqldrivers;
   ret->nsqldrivers = &all_nsqldrivers;

   if (all_tasklocks)
      ret->tasklocks = all_tasklocks;
   else
      ret->tasklocks = all_tasklocks = new_HashTable();

   if (all_fileopens)
      ret->fileopens = all_fileopens;
   else
      ret->fileopens = all_fileopens = new_HashTable();

   if (all_publics)
      ret->publics = all_publics;
   else
      ret->publics = all_publics = new_HashTable();
   ret->privates = new_HashTable();
   ret->spaces = new_HashTable();

/*
	if (all_aliases)
		ret->aliases = all_aliases;
	else
		ret->aliases = all_aliases = new_HashTable();
*/
   ret->aliases = new_HashTable();
/*
	if (all_fields)
		ret->fields = all_fields;
	else
		ret->fields = all_fields = new_HashTable();
*/
   ret->fields = new_HashTable();

   if (all_keys)
      ret->keys = all_keys;
   else
      ret->keys = all_keys = new_HashTable();

   if (all_store)
      ret->store = all_store;
   else
      ret->store = all_store = new_HashTable();

   ret->next = first_mp;
   first_mp = ret;
   ret->decimals = 2;
   ret->fileCreateMode = _clip_fileStrModeToNumMode("664");
   ret->dirCreateMode = _clip_fileStrModeToNumMode("753");
   ret->flags = CONSOLE_FLAG + INTENSITY_FLAG;
   ret->date_format = strdup(CLIP_DATEFORMAT_DEFAULT);
   ret->rootpath = NULL;
   if (CLIP_CENTURY_DEFAULT)
      ret->flags += CENTURY_FLAG;
#ifdef _WIN32
   ret->flags1 += DISPBOX_FLAG;
#endif
   ret->epoch = CLIP_EPOCH_DEFAULT;
   ret->path = strdup(".");
   ret->defaul = strdup(".");
   ret->typeahead = 32;

   if (all_kbdbuf)
      ret->kbdbuf = all_kbdbuf;
   else
      ret->kbdbuf = all_kbdbuf = (int *) calloc(ret->typeahead, sizeof(int));

   ret->kbdptr = &all_kbdptr;
   *(ret->kbdptr) = ret->kbdbuf;

   ret->cursor = SC_NORMAL;
   ret->altfile = 0;
   ret->out = stdout;
   ret->delimchars = strdup("  ");
   ret->eventmask = _CLIP_INKEY_KEYBOARD;
   if (screen)
      ret->screen = screen;
   else
   {
      ret->screen_base = NEW(ScreenBase);
		/*_clip_init_tty(ret);
		_clip_restore_tty(ret);*/
   }
   ret->mblocksize = 64;
   ret->attr.standard_of_ClipAttr = COLOR_WHITE | COLOR_BACK_BLACK;
   ret->attr.enhanced_of_ClipAttr = COLOR_BLACK | COLOR_BACK_WHITE;
   ret->attr.border_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.background_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.unselected_of_ClipAttr = COLOR_BLACK | COLOR_BACK_WHITE;
   ret->attr.u1_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.u2_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.u3_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.u4_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.u5_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->buf = (char *) malloc(64);
   ret->buflen = 64;
   ret->driver = init_struct->_dbdrivers_of_ClipInitStruct[0];
   ret->syserr[0] = 0;

#if 1
   ret->windows = NEW(ClipWindow);
#else
   if (all_windows)
      ret->windows = all_windows;
   else
      ret->windows = all_windows = NEW(ClipWindow);
#endif

   ret->wnum = 0;
   if (ret->screen)
   {
      ret->wboard.bottom_of_ClipRect = ret->windows->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect = ret->screen->base->Lines - 1;
      ret->wboard.right_of_ClipRect = ret->windows->ClipRect_rect_of_ClipWindow.right_of_ClipRect = ret->screen->base->Columns - 1;
      ret->windows->ClipRect_format_of_ClipWindow = ret->windows->ClipRect_format_of_ClipWindow;
   }
   ret->wshadow = -1;
#if 1
   if (all_container)
      ret->container = all_container;
   else
      ret->container = all_container = NEW(Container);
#else
   ret->container = NEW(Container);
#endif

   if (all_hashnames)
      ret->hashnames = all_hashnames;
   else
      ret->hashnames = all_hashnames = new_HashTable();
   ret->print_max_deep = CLIP_MAX_PRINT_DEEP;
   ret->print_deep = 0;

   {
      int i;

      for (i = 0; i < 256; i++)
      {
	 ret->prntbl[i] = i;
	 ret->prn_xlat[i] = i;
	 ret->term_xlat[i] = i;
      }
   }

/*
	if (all_areas)
	{
		ret->areas = all_areas;
		ret->areaStack = all_areaStack;
	}
	else
	{
		ret->areas = all_areas = NEW(ClipVect);
		ret->areaStack = all_areaStack = NEW(ClipVect);
	}
*/
   ret->areas = NEW(ClipVect);
   ret->areaStack = NEW(ClipVect);

   ret->index_buffer_limit = 32;

   ret->flags |= ESCAPE_FLAG;
   ret->flags |= EXCLUSIVE_FLAG;
   ret->flags |= TRANSLATE_FLAG;
   ret->flags1 |= OPTIMIZE_FLAG;

   /*
      #ifdef OS_LINUX
      if (ret->out && isatty(fileno(ret->out)))
      ret->flags1 |= FLUSHOUT_FLAG;
      #endif
    */
   ret->flags1 |= FLUSHOUT_FLAG;

   return ret;
}
