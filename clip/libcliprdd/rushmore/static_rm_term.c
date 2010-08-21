static unsigned int *
rm_term(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, int bytes, int *optimize, int npseudo, RDD_PSEUDO * pseudo, int test, const char *__PROC__)
{
   unsigned int *bm = rm_prim(ClipMachineMemory, rd, fp, bytes, optimize, npseudo, pseudo, test,
			      NULL, __PROC__);

   unsigned int *bm2;

   int opt2, recs;

   if (!bm)
      return NULL;
   for (;;)
   {
      if (rd->curlex == RM_AND)
      {
	 if (*optimize == 2)
	 {
	    unsigned int bytes = ((fp->size + 1) >> 5) + 1;

	    int i;		//,b,bb,t,tt;

	    unsigned int tmp1;	//,tmp2;

	    recs = 0;
	    for (i = 0; i < bytes; i++)
	    {
	       tmp1 = bm[i];
	       if (!tmp1)
		  continue;
	       //if (tmp1 & 0x000000FF)
	       recs += ((tmp1 & 0x00000001) != 0) +
		  ((tmp1 & 0x00000002) != 0) +
		  ((tmp1 & 0x00000004) != 0) + ((tmp1 & 0x00000008) != 0) + ((tmp1 & 0x00000010) != 0) + ((tmp1 & 0x00000020) != 0) + ((tmp1 & 0x00000040) != 0) + ((tmp1 & 0x00000080) != 0);
	       //if (tmp1 & 0x0000FF00)
	       recs += ((tmp1 & 0x00000100) != 0) +
		  ((tmp1 & 0x00000200) != 0) +
		  ((tmp1 & 0x00000400) != 0) + ((tmp1 & 0x00000800) != 0) + ((tmp1 & 0x00001000) != 0) + ((tmp1 & 0x00002000) != 0) + ((tmp1 & 0x00004000) != 0) + ((tmp1 & 0x00008000) != 0);
	       //if (tmp1 & 0x00FF0000)
	       recs += ((tmp1 & 0x00010000) != 0) +
		  ((tmp1 & 0x00020000) != 0) +
		  ((tmp1 & 0x00040000) != 0) + ((tmp1 & 0x00080000) != 0) + ((tmp1 & 0x00100000) != 0) + ((tmp1 & 0x00200000) != 0) + ((tmp1 & 0x00400000) != 0) + ((tmp1 & 0x00800000) != 0);
	       //if (tmp1 & 0xFF000000)
	       recs += ((tmp1 & 0x01000000) != 0) +
		  ((tmp1 & 0x02000000) != 0) +
		  ((tmp1 & 0x04000000) != 0) + ((tmp1 & 0x08000000) != 0) + ((tmp1 & 0x10000000) != 0) + ((tmp1 & 0x20000000) != 0) + ((tmp1 & 0x40000000) != 0) + ((tmp1 & 0x80000000) != 0);
	       /*
	          for(b=(i<<2),bb=0;bb<4;b++,bb++){
	          if( !(((char*)bm)[b]) )
	          continue;
	          for(t=(b<<3)+1,tt=0;tt<8;t++,tt++){
	          if(_rm_getbit(bm,fp->size,t))
	          recs++;
	          }
	          }
	        */
	    }
	 }
	 else
	 {
	    recs = fp->size;
	 }
	 ADDLEX(" .and. ");
	 if ((recs < (fp->size >> 10)) && (rm_yylex(rd, 1) != RM_LP))
	 {
	    if (!(rm_prim(ClipMachineMemory, rd, fp, bytes, &opt2, npseudo, pseudo, test, bm, __PROC__)))
	       return NULL;
	 }
	 else
	 {
	    if (!(bm2 = rm_prim(ClipMachineMemory, rd, fp, bytes, &opt2, npseudo, pseudo, test, NULL, __PROC__)))
	       return NULL;
	    rm_and(bm, bm2, optimize, opt2, bytes);
	 }
      }
      else
	 break;
   }
   return bm;
}
