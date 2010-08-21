CLIP_DLLEXPORT void
_clip_math_operation(ClipMachine * ClipMachineMemory, int op, ClipVar * Lval, ClipVar * Rval)
{
   rational *tmp1 = NULL, *tmp2 = NULL;

   double d1, d2;

   int ldec = Lval->ClipType_t_of_ClipVar.dec_of_ClipType;

   int rdec = Rval->ClipType_t_of_ClipVar.dec_of_ClipType;

   int llen = Lval->ClipType_t_of_ClipVar.len_of_ClipType;

   int rlen = Rval->ClipType_t_of_ClipVar.len_of_ClipType;

   if (ClipMachineMemory->flags & RATIONAL_FLAG)
   {
      if (!Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	 tmp1 = rational_double_init(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
      else
	 tmp1 = Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar;
      if (!Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	 tmp2 = rational_double_init(Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
      else
	 tmp2 = Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar;
      switch (op)
      {
      case '-':
	 rational_suba(tmp1, tmp2);
	 break;
      case '+':
	 rational_adda(tmp1, tmp2);
	 break;
      case '/':
	 rational_diva(tmp1, tmp2);
	 break;
      case '*':
	 rational_mula(tmp1, tmp2);
	 break;

      }
      if (!Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
      {
	 Lval->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
	 Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = tmp1;
      }
      if (!Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	 rational_destroy(tmp2);
   }
   else
   {
      if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	 d1 = rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      else
	 d1 = Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	 d2 = rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      else
	 d2 = Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      switch (op)
      {
      case '-':
	 d1 -= d2;
	 break;
      case '+':
	 d1 += d2;
	 break;
      case '/':
	 if (d2 == 0.0)
	    d1 = 0.0;
	 else
	    d1 /= d2;
	 break;
      case '*':
	 d1 *= d2;
	 break;
      }
      if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	 rational_destroy(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      Lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = d1;
   }
   switch (op)
   {
   case '-':
      llen = (llen < rlen ? rlen : llen);
      ldec = (ldec < rdec ? rdec : ldec);
      break;
   case '+':
      llen = (llen < rlen ? rlen : llen);
      ldec = (ldec < rdec ? rdec : ldec);
      break;
   case '/':
      if (ldec == 0)
      {
	 ldec += ClipMachineMemory->decimals;
	 llen += ClipMachineMemory->decimals + 1;
      }
      break;
   case '*':
      llen += Rval->ClipType_t_of_ClipVar.len_of_ClipType;
      ldec += Rval->ClipType_t_of_ClipVar.dec_of_ClipType;
      break;
   }
   if (ldec > 5)
      ldec = 5;
   if (ClipMachineMemory->flags & FIXED_FLAG)
      ldec = ClipMachineMemory->decimals;
   if (ldec > 0 && llen <= 10 + ldec)
      llen = 11 + ldec;
   if (llen > 20)
      llen = 20;
   Lval->ClipType_t_of_ClipVar.len_of_ClipType = llen;
   Lval->ClipType_t_of_ClipVar.dec_of_ClipType = ldec;
   return;
}
