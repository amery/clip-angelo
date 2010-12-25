int
clip_NUMMIRR(ClipMachine * ClipMachineMemory)
{
   int i;

   unsigned short ret = 0, tmp = 0;

   unsigned char bdata, bret = 0, btmp = 0;

   unsigned short data = _clip_parnl(ClipMachineMemory, 1);

   int par = _clip_parinfo(ClipMachineMemory, 1);

   if (par != NUMERIC_type_of_ClipVarType && par != CHARACTER_type_of_ClipVarType)
   {
      _clip_retni(ClipMachineMemory, -1);
      return 0;
   }
   if (par == CHARACTER_type_of_ClipVarType)
      data = strtol(_clip_parc(ClipMachineMemory, 1), NULL, 16);
   if (_clip_parl(ClipMachineMemory, 2))
   {
      bdata = data & 0xFF;
      for (i = 0; i < 4; i++)
      {
	 btmp = (1 << i) & bdata;
	 btmp = btmp << (8 - 2 * i);
	 bret = bret | btmp;
      }
      for (i = 8; i > 3; i--)
      {
	 btmp = (1 << i) & bdata;
	 btmp = (btmp >> i) << (7 - i);
	 bret = bret | btmp;
      }
      _clip_retnl(ClipMachineMemory, bret);
      return 0;
   }
   else
   {
      for (i = 0; i < 8; i++)
      {
	 tmp = (1 << i) & data;
	 tmp = tmp << (15 - 2 * i);
	 ret = ret | tmp;
      }
      for (i = 16; i > 7; i--)
      {
	 tmp = (1 << i) & data;
	 tmp = (tmp >> i) << (15 - i);
	 ret = ret | tmp;
      }
      _clip_retnl(ClipMachineMemory, ret);
      return 0;
   }
   return 0;
}
