struct tm *
_clip_sysdate()
{
   //#if !defined( __ZTC__ ) || !defined(__MSDOS__)
   struct tm *sysTime;

   time_t t = time(0);

   sysTime = malloc(sizeof(struct tm));
   memcpy(sysTime, localtime(&t), sizeof(struct tm));

   return sysTime;
   /*
      #else
      time_t t=time(0);
      struct tm sysTime;
      REGS regs;
      regs.h.ah=0x2a;
      intdos(&regs, &regs);
      sysTime->tm_year=regs.x.cx-1900;
      sysTime->tm_wday=regs.h.al;
      sysTime->tm_mon=regs.h.dh-1;
      sysTime->tm_mday=regs.h.dl;
      regs.h.ah=0x2c;
      intdos(&regs, &regs);
      sysTime->tm_hour=regs.h.ch;
      sysTime->tm_min=regs.h.cl;
      sysTime->tm_sec=regs.h.dh;
      sysTime->tm_yday=0;
      sysTime->tm_isdst=-1;
      mktime(sysTime);
      return &sysTime;
      #endif
    */
}
