static long
calcWakeup(long msec)
{
   long n, ret;

   long clk_tck = sysconf(_SC_CLK_TCK);	//CLK_TCK;

   if (msec < 0)
   {				/*  one day will enought? */
      ret = (long) times(&tms_buf) + 60 * 60 * 24 * clk_tck;
      return ret;
   }
   n = msec / (1000 / clk_tck);
   if (n < 1)
      n = 1;
   ret = times(&tms_buf) + n;
   return ret;
}
