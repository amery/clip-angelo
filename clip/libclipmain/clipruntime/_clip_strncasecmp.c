CLIP_DLLEXPORT int
_clip_strncasecmp(const char *str1, const char *str2, int len)
{
   int l1 = strlen(str1);

   int l2 = strlen(str2);

   if (l1 > len)
      l1 = len;
   if (l2 > len)
      l2 = len;
   return _clip_strnncasecmp(str1, str2, l1, l2);
}
