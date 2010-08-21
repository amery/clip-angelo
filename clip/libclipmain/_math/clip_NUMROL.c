int
clip_NUMROL(ClipMachine * ClipMachineMemory)
{
   unsigned long ret = 0, tmp = 0;

   unsigned short data = _clip_parnl(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   int par = _clip_parinfo(ClipMachineMemory, 1);

   if (par != NUMERIC_type_of_ClipVarType && par != CHARACTER_type_of_ClipVarType)
   {
      _clip_retni(ClipMachineMemory, -1);
      return 0;
   }
   if (par == CHARACTER_type_of_ClipVarType)
      data = strtol(_clip_parc(ClipMachineMemory, 1), NULL, 16);

   if (_clip_parl(ClipMachineMemory, 3))
   {
      left = left % 8;
      ret = data & 0xFF;
      ret = ret << left;
      tmp = (ret & 0x0F00) >> 8;
      ret = (ret & 0x00FF) | tmp;
      _clip_retnl(ClipMachineMemory, ret | (data & 0xFF00));
      return 0;
   }
   else
   {
      left = left % 16;
      ret = data;
      ret = ret << left;
      tmp = (ret & 0x00FF0000) >> 16;
      ret = (ret & 0xFFFF) | tmp;
      _clip_retnl(ClipMachineMemory, ret);
      return 0;
   }
   return 0;
}
