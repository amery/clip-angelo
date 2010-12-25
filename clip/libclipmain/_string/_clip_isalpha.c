int
_clip_isalpha(unsigned char ch)
{
#if 0
   int ret = 0;

   if (ch >= 'A' && ch <= 'Z')
      ret = 1;
   if (ch >= 'a' && ch <= 'z')
      ret = 1;
   if (ch > 127)
      ret = _clip_isalpha_tbl[ch - 128];
   return ret;
#else
   return _clip_isalpha_tbl[ch];
#endif
}
