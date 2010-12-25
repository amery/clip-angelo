int
_clip_charsort(const void *a1, const void *a2)
{
   int ret = 0, *set, i;

   set = _clip_fetch_item(_clip_charsort_mp, HASH_charsort_param);
   for (i = set[2]; i < set[3] && ret == 0; i++)
      ret = (((unsigned char *) a1)[i] >= ((unsigned char *) a2)[i]);
   if (set[0])
      return !ret;
   else
      return ret;
}
