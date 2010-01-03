int
clip_SQLSETLENDEC(ClipMachine * ClipMachineMemory)
{
   double    d = _clip_parnd(ClipMachineMemory, 1);

   int       len = _clip_parni(ClipMachineMemory, 2);

   int       dec = _clip_parni(ClipMachineMemory, 3);

   _clip_retndp(ClipMachineMemory, d, len, dec);
   return 0;
}
