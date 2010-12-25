int
_clip_dt_normalize(ClipDateTime * dt)
{
   int ii, jj, ww;

   long d;

   ii = dt->msec % 1000;
   jj = dt->msec / 1000;
   if (ii < 0)
   {
      jj--;
      ii = 1000 + ii;
   }
   dt->msec = ii;
   dt->sec += jj;

   ii = dt->sec % 60;
   jj = dt->sec / 60;
   if (ii < 0)
   {
      jj--;
      ii = 60 + ii;
   }
   dt->sec = ii;
   dt->min += jj;

   ii = dt->min % 60;
   jj = dt->min / 60;
   if (ii < 0)
   {
      jj--;
      ii = 60 + ii;
   }
   dt->min = ii;
   dt->hour += jj;

   ii = dt->hour % 24;
   jj = dt->hour / 24;
   if (ii < 0)
   {
      jj--;
      ii = 24 + ii;
   }
   dt->hour = ii;
   dt->day += jj;

   d = _clip_jdate(dt->day, dt->month, dt->year);
   _clip_cdate(d, &(dt->day), &(dt->month), &(dt->year), &ww);
   return 0;
}
