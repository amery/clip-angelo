CLIP_DLLEXPORT void
_clip_logg(int level, const char *fmt, ...)
{
   va_list ap;

   time_t tt;

   struct tm *tp;

   if (level > log_level)
      return;

   _clip_log_init(NULL);
   if (!logg)
      goto ret;
   tt = time(0);
   tp = localtime(&tt);

   fprintf(logg, "%02d:%02d:%02d %s:%lu:%d ", tp->tm_hour, tp->tm_min, tp->tm_sec, _clip_progname, (unsigned long) getpid(), level);
   va_start(ap, fmt);
   vfprintf(logg, _clip_gettext(fmt), ap);
   va_end(ap);
   fputc('\n', logg);
   fflush(logg);

 ret:
   ;
}
