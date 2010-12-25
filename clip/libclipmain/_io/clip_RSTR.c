int
clip_RSTR(ClipMachine * ClipMachineMemory)
{
   int lend = 10, decd = 2;

   char buf[32], *s;

   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   int len = _clip_parni(ClipMachineMemory, 2);

   int dec = _clip_parni(ClipMachineMemory, 3);

   _clip_parp(ClipMachineMemory, 1, &lend, &decd);

   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && !v->ClipType_t_of_ClipVar.memo_of_ClipType)
   {
      if (len)
	 snprintf(buf, sizeof(buf), "%*.*f", len, dec, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
      else
	 snprintf(buf, sizeof(buf), "%*.*f", lend, decd, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
      _clip_retc(ClipMachineMemory, buf);
   }
   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && v->ClipType_t_of_ClipVar.memo_of_ClipType)
   {
      s = rational_toString2(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      _clip_retcn_m(ClipMachineMemory, s, strlen(s));
   }
   return 0;
}
