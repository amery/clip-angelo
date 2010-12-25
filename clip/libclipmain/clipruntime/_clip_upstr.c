CLIP_DLLEXPORT void
_clip_upstr(char *s, int len)
{
   unsigned char *us = (unsigned char *) s;

   for (; len; us++, len--)
   {
      int c = *us;

      *us = _clip_uptbl[c];
   }
}
