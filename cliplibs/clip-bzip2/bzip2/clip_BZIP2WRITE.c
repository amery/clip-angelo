int
clip_BZIP2WRITE(ClipMachine * mp)
{
   BZFILE   *gz;

   int       len = 0, *err = NULL;

   int       fd = _clip_parni(mp, 1);

   char     *str = _clip_parcl(mp, 2, &len);

   long      ret = -1, n = _clip_parnl(mp, 3);

   gz = _clip_fetch_c_item(mp, fd, _C_ITEM_TYPE_FILE);

   if (gz == NULL || !str)
    {
       _clip_retnl(mp, -1);
       return 0;
    }
   if (n <= 0 || n >= len)
      n = len;
   ret = BZPREF(bzwrite) (gz, (void *) str, n);
   err = _clip_fetch_item(mp, HASH_ferror);
   *err = ret < 0 ? errno : 0;
   _clip_retnl(mp, ret);
   return 0;
}

