int
clip_DESCEND(ClipMachine * ClipMachineMemory)
{
   int i, j, l, d;

   int t = _clip_parinfo(ClipMachineMemory, 1);

   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   unsigned char *buf, *str, ch;

   double n;

   switch (t)
   {
   case MEMO_type_of_ClipVarType:
   case CHARACTER_type_of_ClipVarType:
      str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);
      buf = calloc(l + 1, 1);
      buf[l] = 0;
      for (i = 0; i < l; i++)
      {
	 ch = str[i];
	 ch = 256 - _clip_cmptbl[ch];
	 for (j = 0; j < 256; j++)
	    if (_clip_cmptbl[j] == ch)
	       break;
	 if (j < 256)
	    buf[i] = j;
	 else
	    buf[i] = ch;
      }
      _clip_retcn_m(ClipMachineMemory, (char *) buf, l);
      break;
   case DATE_type_of_ClipVarType:
      _clip_retnl(ClipMachineMemory, 5231808 - _clip_pardj(ClipMachineMemory, 1));
      break;
   case NUMERIC_type_of_ClipVarType:
      if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
      {
	 _clip_retnr(ClipMachineMemory, rational_inv(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar), l, d);
      }
      else
      {
	 n = _clip_parnd(ClipMachineMemory, 1);
	 _clip_parp(ClipMachineMemory, 1, &l, &d);
	 _clip_retndp(ClipMachineMemory, -1 * n, l, d);
      }
      break;
   case LOGICAL_type_of_ClipVarType:
      _clip_retl(ClipMachineMemory, !_clip_parl(ClipMachineMemory, 1));
      break;
   }
   return 0;
}
