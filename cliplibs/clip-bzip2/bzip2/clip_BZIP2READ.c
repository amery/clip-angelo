int
clip_BZIP2READ(ClipMachine * mp)
{
   BZFILE   *gz;

   char     *buf;

   int      *err = NULL, fd, nb = 0;

   long      ret = -1;

   fd = _clip_parni(mp, 1);
   buf = _clip_parcl(mp, 2, &nb);
   gz = _clip_fetch_c_item(mp, fd, _C_ITEM_TYPE_FILE);

   if (mp->argc > 2)
      nb = _clip_parnl(mp, 3);

   if (gz == NULL || !buf)
    {
       _clip_retnl(mp, -1);
       return 0;
    }

   buf = (char *) malloc(nb);

   ret = BZPREF(bzread) (gz, buf, nb);
   if (ret >= 0)
      _clip_storclen(mp, buf, ret, 2, 0);
   else
    {
       err = _clip_fetch_item(mp, HASH_ferror);
       *err = errno;
    }

   _clip_retnl(mp, ret);
   free(buf);

   return 0;
}

