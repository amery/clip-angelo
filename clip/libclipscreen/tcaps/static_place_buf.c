static int
place_buf(Terminfo * info, int need)
{
   if ((info->bufsize - info->bufpos) < need)
   {
      int delta, osize;

      delta = info->bufsize / 3;
      if (delta < need)
	 delta = need;
      if (delta < 8)
	 delta = 8;
      osize = info->bufsize;
      info->bufsize += delta;
      info->buf = (char *) realloc(info->buf, info->bufsize);
      memset(info->buf + osize, 0, delta);
   }

   return info->bufsize - info->bufpos;
}
