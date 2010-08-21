static double
_clip_seconds()
{
   double ret;

#if !defined( __ZTC__ ) || !defined(__MSDOS__)
   struct timeval tv;

   struct timezone tz;

   gettimeofday(&tv, &tz);
#ifdef _WIN32
   ret = (tv.tv_sec) % 86400 + (double) tv.tv_usec / 1000000;
#else
   ret = (tv.tv_sec - tz.tz_minuteswest * 60) % 86400 + (double) tv.tv_usec / 1000000;
#endif
#else
   time_t t = time(0);

   struct tm sysTime;

   struct timeval tv;

   {
      REGS regs;

      regs.h.ah = 0x2a;
      intdos(&regs, &regs);
      sysTime.tm_year = regs.x.cx - 1900;
      sysTime.tm_wday = regs.h.al;
      sysTime.tm_mon = regs.h.dh - 1;
      sysTime.tm_mday = regs.h.dl;
      regs.h.ah = 0x2c;
      intdos(&regs, &regs);
      sysTime.tm_hour = regs.h.ch;
      sysTime.tm_min = regs.h.cl;
      sysTime.tm_sec = regs.h.dh;
      sysTime.tm_yday = 0;
      sysTime.tm_isdst = -1;
      mktime(&sysTime);
   }
   gettimeofday(&tv, 0);
   ret = (sysTime.tm_hour * 3600 + sysTime.tm_min * 60 + sysTime.tm_sec) * 1000;
   ret = (ret + tv.tv_usec / 1000) / 1000;
#endif
   return ret;
}
