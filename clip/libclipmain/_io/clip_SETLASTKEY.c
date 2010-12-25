int
clip_SETLASTKEY(ClipMachine * ClipMachineMemory)
{
   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
      ClipMachineMemory->lastkey = _clip_parni(ClipMachineMemory, 1);
   _clip_retc(ClipMachineMemory, "");
   return 0;
}
