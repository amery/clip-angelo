CLIP_DLLEXPORT void *
_clip_memdup(const void *src, int len)
{
   void *ret = malloc(len + 1);

   memcpy(ret, src, len);
   ((char *) ret)[len] = 0;
   return ret;
}
