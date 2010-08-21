int
clip_RANDOM(ClipMachine * ClipMachineMemory)
{
   long ret = 0;

   long d = _clip_parnl(ClipMachineMemory, 1);

   long l = _clip_parl(ClipMachineMemory, 1);

   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      d = 0xFFFF;

   if (d != 0)
      ret = random() % d;
   else
      ret = random();
   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType && !l)
      ret -= d / 2;
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
