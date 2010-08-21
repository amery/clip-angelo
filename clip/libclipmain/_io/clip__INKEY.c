int
clip__INKEY(ClipMachine * ClipMachineMemory)
{
   int ckey;

   int mask = ClipMachineMemory->eventmask;

   dialog_init(ClipMachineMemory);
   sync_mp(ClipMachineMemory);

   if (ClipMachineMemory->argc > 0 && _clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
   {
      double t = _clip_parnd(ClipMachineMemory, 1);

      if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	 mask = _clip_parni(ClipMachineMemory, 2);

      if (t > 0)
	 ckey = _clip_key(ClipMachineMemory, t * 1000, mask);
      else if (t == 0)
	 ckey = _clip_key(ClipMachineMemory, -1, mask);
      else
	 ckey = _clip_key(ClipMachineMemory, 0, mask);
   }
   else
      ckey = _clip_key(ClipMachineMemory, 0, mask);

   _clip_retndp(ClipMachineMemory, ckey, 10, 0);

   return 0;
}
