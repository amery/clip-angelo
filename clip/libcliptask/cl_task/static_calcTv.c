static void
calcTv(struct timeval *tv, long msec)
{
   if (msec == 0)
      msec = 1;
   tv->tv_sec = msec / 1000;
   tv->tv_usec = (msec % 1000) * 1000;
}
