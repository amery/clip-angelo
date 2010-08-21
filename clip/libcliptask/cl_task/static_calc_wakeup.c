static long
calc_wakeup(struct timeval *tv)
{
   long n;

   long clk_tck = sysconf(_SC_CLK_TCK);	//CLK_TCK;

   if (!tv)
      return times(&tms_buf) + 60 * 60 * 24 * clk_tck;
   n = tv->tv_sec * clk_tck + tv->tv_usec / (1000000 / clk_tck);
   if (!n)
      n = 1;
   return times(&tms_buf) + n;
}
