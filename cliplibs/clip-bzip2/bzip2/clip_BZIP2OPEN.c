int
clip_BZIP2OPEN(ClipMachine * mp)
{
   char     *fname = _clip_parc(mp, 1);

   char     *mode = _clip_parc(mp, 2);

   int      *err = NULL, ret;

   char      buf[PATH_MAX];

   BZFILE   *gz;

   if (fname == NULL)
    {
       _clip_retni(mp, -1);
       return _clip_trap_err(mp, EG_ARG, 0, 0, __FILE__, __LINE__, "GZIPOPEN");
    }
   if (mode == NULL)
      mode = "rb";

   _clip_translate_path(mp, fname, buf, sizeof(buf));
   if (strcmp(buf, "in") == 0)
      gz = BZPREF(bzdopen) (fileno(stdin), "rb");
   else
    {
       if (strcmp(buf, "out") == 0)
	  gz = BZPREF(bzdopen) (fileno(stdout), "wb");
       else
	  gz = BZPREF(bzopen) (buf, mode);
    }
   if (!gz)
    {
       err = _clip_fetch_item(mp, HASH_ferror);
       *err = errno;
       _clip_retni(mp, -1);
       return 0;
    }
   ret = _clip_store_c_item(mp, gz, _C_ITEM_TYPE_FILE, destroy_bzip_file);
   _clip_retni(mp, ret);
   return 0;
}

