int
clip_NUMLOW(ClipMachine * ClipMachineMemory)
{
   unsigned short data = _clip_parnl(ClipMachineMemory, 1);

   int par = _clip_parinfo(ClipMachineMemory, 1);

   if (par != NUMERIC_type_of_ClipVarType && par != CHARACTER_type_of_ClipVarType)
   {
      _clip_retni(ClipMachineMemory, -1);
      return 0;
   }
   if (par == CHARACTER_type_of_ClipVarType)
      data = strtol(_clip_parc(ClipMachineMemory, 1), NULL, 16);
   _clip_retnl(ClipMachineMemory, data & 0x00FF);
   return 0;
}
