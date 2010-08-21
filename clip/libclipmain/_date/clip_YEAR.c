int
clip_YEAR(ClipMachine * ClipMachineMemory)
{
   int dd, mm, yy, ww;

   if (_clip_pardj(ClipMachineMemory, 1) == 0)
   {
      _clip_retndp(ClipMachineMemory, 0, 5, 0);
      return 0;
   }

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   _clip_retndp(ClipMachineMemory, yy, 5, 0);
   return 0;
}
