int
clip_STRZERO(ClipMachine * ClipMachineMemory)
{
   int lend = 10, decd = 2;

   char *s;

   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   int len = _clip_parni(ClipMachineMemory, 2);

   int dec = _clip_parni(ClipMachineMemory, 3);

   char *buf;

   _clip_parp(ClipMachineMemory, 1, &lend, &decd);

   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && !v->ClipType_t_of_ClipVar.memo_of_ClipType)
   {
      if (!len && !lend)
      {
	 char b[32];

	 snprintf(b, sizeof(b), "%d", (int) v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	 _clip_retc(ClipMachineMemory, b);
	 return 0;
      }
      buf = malloc((len ? len : lend) + 1);

#if 1
      /* uri added */
      _clip_dtostr(buf, len ? len : lend, _clip_parinfo(ClipMachineMemory, 3) != UNDEF_type_of_ClipVarType ? dec : decd, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
#else
      _clip_dtostr(buf, len ? len : lend, len ? dec : decd, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 1);
#endif
      _clip_retcn_m(ClipMachineMemory, buf, len ? len : lend);
   }
   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && v->ClipType_t_of_ClipVar.memo_of_ClipType)
   {
      if (len)
	 s = rational_toString(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, dec, 0);
      else
      {
	 if (lend)
	    s = rational_toString(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, decd, 0);
	 else
	    s = rational_toString(v->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, ClipMachineMemory->decimals, 0);
      }
      _clip_retcn_m(ClipMachineMemory, s, strlen(s));
   }
   return 0;
}
