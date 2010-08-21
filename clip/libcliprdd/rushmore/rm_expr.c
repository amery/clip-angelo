unsigned int *
rm_expr(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, int bytes, int *optimize, int npseudo, RDD_PSEUDO * pseudo, int test, const char *__PROC__)
{
   unsigned int *bm = rm_term(ClipMachineMemory, rd, fp, bytes, optimize, npseudo, pseudo, test,
			      __PROC__);

   unsigned int *bm2;

   int opt2;

   if (!bm)
      return NULL;
   for (;;)
   {
      if (rd->curlex == RM_OR)
      {
	 ADDLEX(" .or. ");
	 if (!(bm2 = rm_term(ClipMachineMemory, rd, fp, bytes, &opt2, npseudo, pseudo, test, __PROC__)))
	    return NULL;
	 rm_or(bm, bm2, optimize, opt2, bytes);
      }
      else
	 break;
   }
   return bm;
}
