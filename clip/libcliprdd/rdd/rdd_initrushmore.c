int
rdd_initrushmore(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, ClipVar * a, int test, const char *__PROC__)
{
   unsigned int lastrec;

   unsigned int bytes;

   int optimize = 0;

   char *str = strdup(fp->sfilter);

   int er = EG_UNSUPPORTED;

   RDD_PSEUDO *pseudo = NULL;

   int npseudo = 0;

   if (a)
   {
      ClipVar *e, *alias, *name;

      long i[1], j[1];

      npseudo = a->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      pseudo = calloc(npseudo, sizeof(RDD_PSEUDO));
      for (i[0] = 0; i[0] < npseudo; i[0]++)
      {
	 e = _clip_vptr(_clip_aref(ClipMachineMemory, a, 1, i));
	 if (e->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType && e->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar == 2)
	 {
	    j[0] = 0;
	    alias = _clip_vptr(_clip_aref(ClipMachineMemory, e, 1, j));
	    j[0] = 1;
	    name = _clip_vptr(_clip_aref(ClipMachineMemory, e, 1, j));
	    if (alias->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType || name->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	       goto err_pseudo;
	    pseudo[i[0]].alias = alias->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	    pseudo[i[0]].name = name->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 }
	 else
	    goto err_pseudo;
      }
   }

   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
      goto err;

   bytes = (lastrec >> 5) + 1;

   if ((er = rm_init(ClipMachineMemory, rd, str, __PROC__)))
      goto err;
   if (fp->rmap)
      free(fp->rmap);
   fp->size = lastrec;

   rd->rmflen = 0;
   rd->rmfilter = malloc(1);
   if (!(fp->rmap = rm_expr(ClipMachineMemory, rd, fp, bytes, &optimize, npseudo, pseudo, test, __PROC__)))
   {
      er = 1;
      goto err;
   }
   free(fp->sfilter);
   fp->sfilter = rd->rmfilter;
   rd->rmfilter = NULL;

   if (test)
      optimize = 0;
   fp->optimize = optimize;
   if (fp->optimize == 0)
   {
      free(fp->rmap);
      fp->rmap = NULL;
   }
   if (fp->optimize == 2)
   {
      if ((er = _rdd_calcfiltlist(ClipMachineMemory, rd, fp, __PROC__)))
	 goto err;
   }
   free(str);
   if (pseudo)
      free(pseudo);
   if (test && fp->rmap)
   {
      free(fp->rmap);
      fp->rmap = NULL;
   }
   return 0;
 err_pseudo:
   free(pseudo);
   er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Bad argument");
 err:
   free(str);
   return er;
}
