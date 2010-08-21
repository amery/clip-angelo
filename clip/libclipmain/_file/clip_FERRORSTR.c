int
clip_FERRORSTR(ClipMachine * ClipMachineMemory)
{
   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
   {
      _clip_retc(ClipMachineMemory, strerror(_clip_parni(ClipMachineMemory, 1)));
   }
   else
   {
      int *err;

      err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);
      _clip_retc(ClipMachineMemory, strerror(*err));
   }
   return 0;
}
