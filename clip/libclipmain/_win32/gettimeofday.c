int
gettimeofday(struct timeval *tv, struct timezone *tz)
{
   SYSTEMTIME st;

   if (tv != NULL)
   {
      GetLocalTime(&st);
      tv->tv_sec = st.wMilliseconds / 1000;
      tv->tv_usec = st.wMilliseconds * 1000;
   }

   return 0;
}
