DbfLocale *
dbf_get_locale(ClipMachine * ClipMachineMemory)
{
#if 1
   char *dbfcs;

   DbfLocale *dbf_locale;
   static Coll locales = { 0, 0, 0, 0, 0, 0 };
   LocData ld, *lp;

   int ind;

   char *hostcs;

   cons_CharsetEntry *cs1 = 0, *cs2 = 0;

   int len1 = 0, len2 = 0;

   unsigned char *buf;

   dbfcs = (char *) _clip_fetch_item(ClipMachineMemory, _clip_hashstr("DBF_CHARSET"));
   if (!dbfcs)
   {
      _clip_logg(2, "no item DBF_CHARSET, revert to KOI charset");
      return &koi_locale;
   }

   if (!locales.compare)
      init_Coll(&locales, 0, cmp_locale);

   ld.name = dbfcs;
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
      _clip_logg(0, "dbf: cannot determine host charset, revert to koi_locale");
      return &koi_locale;
   }

   _clip_logg(2, "load host charset '%s'", hostcs);
   if (load_charset_name(hostcs, &cs1, &len1))
   {
      _clip_logg(0, "dbf: cannot load charset '%s': %s, revert to koi_locale", hostcs, strerror(errno));
      return &koi_locale;
   }

   _clip_logg(2, "load dbf charset '%s'", dbfcs);
   if (load_charset_name(dbfcs, &cs2, &len2))
   {
      _clip_logg(0, "dbf: cannot load charset '%s': %s, revert to koi_locale", dbfcs, strerror(errno));
      return &koi_locale;
   }

   buf = (unsigned char *) calloc(256, 3);

   make_translation(cs1, len1, cs2, len2, buf);
   make_translation(cs2, len2, cs1, len1, buf + 256);

   free(cs1);
   free(cs2);

   dbf_locale = (DbfLocale *) calloc(1, sizeof(DbfLocale));
   lp = (LocData *) calloc(1, sizeof(LocData));
   lp->locale = dbf_locale;
   lp->name = strdup(dbfcs);

   memcpy(dbf_locale->read, buf + 256 + 128, 128);
   memcpy(dbf_locale->write, buf + 128, 128);

#if 1
   memcpy(dbf_locale->cmp, _clip_cmptbl + 128, 128);
   memcpy(dbf_locale->upper, _clip_uptbl + 128, 128);
   memcpy(dbf_locale->lower, _clip_lowtbl + 128, 128);
#else
   load_charset_tables(dbfcs, buf, buf + 256, buf + 512, 0, 0);
   memcpy(dbf_locale->cmp, buf + 128, 128);
   memcpy(dbf_locale->upper, buf + 256 + 128, 128);
   memcpy(dbf_locale->lower, buf + 512 + 128, 128);
#endif

   free(buf);
   insert_Coll(&locales, lp);
   return dbf_locale;

#else
   char *s;

   DbfLocale *dbf_locale = &koi_locale;

   s = (char *) _clip_fetch_item(ClipMachineMemory, _clip_hashstr("DBF_CHARSET"));
   if (s && (!strcasecmp(s, "alt") || !strcasecmp(s, "866")))
      dbf_locale = &alt_locale;
   if (s && (!strcasecmp(s, "win") || !strcasecmp(s, "1251")))
      dbf_locale = &win_locale;
   if (s && (!strcasecmp(s, "main") || !strcasecmp(s, "459")))
      dbf_locale = &main_locale;

   return dbf_locale;
#endif
}
