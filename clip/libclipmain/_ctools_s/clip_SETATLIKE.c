int
clip_SETATLIKE(ClipMachine * ClipMachineMemory)
{
   char *atlike;

   atlike = _clip_fetch_item(ClipMachineMemory, HASH_setatlike);
   _clip_retni(ClipMachineMemory, atlike[0]);
   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
      atlike[0] = _clip_parni(ClipMachineMemory, 1);
   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      atlike[0] = _clip_parl(ClipMachineMemory, 1);
   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      atlike[1] = _clip_parni(ClipMachineMemory, 2);
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      atlike[1] = *(_clip_parc(ClipMachineMemory, 2));
   return 0;
}
