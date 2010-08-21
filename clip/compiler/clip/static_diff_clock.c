static char *
diff_clock(clock_t beg, clock_t end)
{
   static char buf[32];

   clock_t diff = end - beg;

   int /*m, */ s, ms;

   long clk_tck;

   clk_tck = sysconf(_SC_CLK_TCK);	/*CLK_TCK; */

   s = diff / clk_tck;
   ms = (diff % clk_tck) * 100 / clk_tck;
   /*m=s/60;
      s=s%60; */
   sprintf(buf, "%d.%02ds", s, ms);
   return buf;
}
