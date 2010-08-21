int
clip_SECONDSCPU(ClipMachine * ClipMachineMemory)
{
   int n = _clip_parni(ClipMachineMemory, 1);

   double d = 0;

   struct tms tm;

   times(&tm);

   if ((n < 1 || n > 3) && (n < 11 || n > 13))
      n = 3;

   if (n > 10)
   {
      n -= 10;
      if (n & 1)
	 d += tm.tms_cutime;
      if (n & 2)
	 d += tm.tms_cstime;
   }
   if (n & 1)
      d += tm.tms_utime;
   if (n & 2)
      d += tm.tms_stime;

   d /= sysconf(_SC_CLK_TCK);
   _clip_retndp(ClipMachineMemory, d, 10, 2);
   return 0;
}
