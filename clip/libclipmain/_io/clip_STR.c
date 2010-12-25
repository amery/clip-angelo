int
clip_STR(ClipMachine * ClipMachineMemory)
{
   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   int len = _clip_parni(ClipMachineMemory, 2);

   int dec = _clip_parni(ClipMachineMemory, 3);

   char *buf;

   _clip_retc(ClipMachineMemory, "");
   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STR");

   if (len < 0)
      return 0;

   if (_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType)
      len = -999;
   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      dec = -999;

   buf = _clip_strFunc(ClipMachineMemory, v, len, dec, 0);
   _clip_retcn_m(ClipMachineMemory, buf, strlen(buf));
   return 0;
}
