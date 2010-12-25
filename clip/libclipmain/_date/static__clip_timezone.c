static int
_clip_timezone()
{
   int ret;

#if !defined( __ZTC__ ) || !defined(__MSDOS__)
   struct timeval tv;

   struct timezone tz;

   gettimeofday(&tv, &tz);
   ret = tz.tz_minuteswest;
#else
   ret = 0;
#endif
   return ret;
}
