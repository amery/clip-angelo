int
clip___KSETSTATE(ClipMachine * ClipMachineMemory, int locktype)
{
   int newvalue = _clip_parl(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);

   if (_clip_parinfo(ClipMachineMemory, 1) != LOGICAL_type_of_ClipVarType)
      newvalue = -1;
   _clip_retl(ClipMachineMemory, setState_Key(ClipMachineMemory->screen->base, newvalue, locktype));

   return 0;
}
