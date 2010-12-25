clock_t
times(struct tms *buf)
{
   static clock_t pt = 0;

   clock_t ct;

   if (pt == 0)
      pt = clock();

   ct = clock();

   buf->tms_utime = ct - pt;
   buf->tms_stime = pt;
   buf->tms_cutime = ct - pt;
   buf->tms_cstime = pt;

   return ct;
}
