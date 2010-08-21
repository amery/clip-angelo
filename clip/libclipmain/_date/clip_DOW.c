int
clip_DOW(ClipMachine * ClipMachineMemory)
{
   int week;

   long d = _clip_pardj(ClipMachineMemory, 1);

   char *buf = _clip_fetch_item(ClipMachineMemory, HASH_first_day_week);

   week = (d + (*buf)) % 7 + 1;
   if (d == 0)
      _clip_retndp(ClipMachineMemory, 0, 1, 0);
   else
      _clip_retndp(ClipMachineMemory, week, 1, 0);
   return 0;
}
