SQLLocale *
SQL_get_locale(ClipMachine * ClipMachineMemory, const char *sqlcs)
{
   char *SQLcs;

   SQLLocale *SQL_locale;
   static Coll locales = { 0, 0, 0, 0, 0, 0 };
   LocData ld, *lp;

   int ind;

   char *hostcs;

   cons_CharsetEntry *cs1 = 0, *cs2 = 0;

   int len1 = 0, len2 = 0;

   unsigned char *buf;

   if (sqlcs && sqlcs[0])
   {
      SQLcs = strdup(sqlcs);
   }
   else
   {
      SQLcs = (char *) _clip_fetch_item(ClipMachineMemory, _clip_hashstr("SQL_CHARSET"));
      if (!SQLcs)
      {
	 _clip_logg(2, "no item SQL_CHARSET, revert to KOI charset");
	 return &koi_locale;
      }
   }

   if (!locales.compare)
      init_Coll(&locales, 0, cmp_locale);

   ld.name = SQLcs;
   if (search_Coll(&locales, &ld, &ind))
   {
		lp = (LocData *) locales.items_of_Coll[ind];
      return lp->locale;
   }

   hostcs = _clip_getenv("CLIP_HOSTCS");
   if (!hostcs)
   {
      char *s;

      s = _clip_getenv("LC_ALL");
      if (s && *s)
      {
	 hostcs = strrchr(s, '.');
	 if (hostcs)
	    hostcs++;
	 else if (strcmp(s, "C") && strcmp(s, "POSIX"))
	    hostcs = s;
      }
   }
   if (!hostcs)
      hostcs = _clip_getenv("CLIP_LANG");
   if (hostcs == NULL)
      hostcs = _clip_getenv("LANG");

   if (!hostcs)
   {
      _clip_logg(0, "SQL: cannot determine host charset, revert to koi_locale");
      return &koi_locale;
   }

   _clip_logg(2, "load host charset '%s'", hostcs);
   if (load_charset_name(hostcs, &cs1, &len1))
   {
      _clip_logg(0, "SQL: cannot load charset '%s': %s, revert to koi_locale", hostcs, strerror(errno));
      return &koi_locale;
   }

   _clip_logg(2, "load SQL charset '%s'", SQLcs);
   if (load_charset_name(SQLcs, &cs2, &len2))
   {
      _clip_logg(0, "SQL: cannot load charset '%s': %s, revert to koi_locale", SQLcs, strerror(errno));
      return &koi_locale;
   }

   buf = (unsigned char *) calloc(256, 3);

   make_translation(cs1, len1, cs2, len2, buf);
   make_translation(cs2, len2, cs1, len1, buf + 256);

   free(cs1);
   free(cs2);

   SQL_locale = (SQLLocale *) calloc(1, sizeof(SQLLocale));
   lp = (LocData *) calloc(1, sizeof(LocData));
   lp->locale = SQL_locale;
   lp->name = strdup(SQLcs);

   memcpy(SQL_locale->read, buf + 256 + 128, 128);
   memcpy(SQL_locale->write, buf + 128, 128);

   memcpy(SQL_locale->cmp, _clip_cmptbl + 128, 128);
   memcpy(SQL_locale->upper, _clip_uptbl + 128, 128);
   memcpy(SQL_locale->lower, _clip_lowtbl + 128, 128);

   free(buf);
   insert_Coll(&locales, lp);
   return SQL_locale;
}
