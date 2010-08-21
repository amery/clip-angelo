CLIP_DLLEXPORT int
_clip_strcmp(const char *str1, const char *str2)
{
   int l1 = strlen(str1);

   int l2 = strlen(str2);

   return _clip_strnncmp(str1, str2, l1, l2);
}
