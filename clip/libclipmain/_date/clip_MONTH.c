int
clip_MONTH(ClipMachine * ClipMachineMemory)
{
   int dd, mm, yy, ww;

   if (_clip_pardj(ClipMachineMemory, 1) == 0)
   {
      _clip_retndp(ClipMachineMemory, 0, 3, 0);
      return 0;
   }

   _clip_pardc(ClipMachineMemory, 1, &yy, &mm, &dd, &ww);
   _clip_retndp(ClipMachineMemory, mm, 3, 0);
   return 0;
}
