int
clip_BZIP2CLOSE(ClipMachine * mp)
{
   BZFILE   *gz;

   int       fd = _clip_parni(mp, 1);

   int       ret = -1, *err = _clip_fetch_item(mp, HASH_ferror);

   gz = (BZFILE *) _clip_fetch_c_item(mp, fd, _C_ITEM_TYPE_FILE);

   if (gz == NULL)
    {
       *err = EBADF;
    }

   else if (_clip_destroy_c_item(mp, fd, _C_ITEM_TYPE_FILE))
    {
       ret = 0;
       *err = 0;
    }

   _clip_retl(mp, (ret == 0));
   return 0;
}

