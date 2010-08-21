static void
put_function(File * file, StrBuf * bp, Function * fp, long *loffs)
{
   int j, count, namelen;

   long lp, blp;

   /*printf("put_function: beg: %s\n", fp->name_of_Function);fflush(stdout); */
   if (loffs)
   {
      SETLONG(bp, *loffs, hashstr(fp->name_of_Function));
      LONGINCR(*loffs);
      SETLONG(bp, *loffs, BEGOFFS);
      LONGINCR(*loffs);
   }
   /* offs from beg of module */
   putLong_StrBuf(bp, BEGOFFS);
   lp = CUROFFS;
   /* body offs (from BOM) */
   /* 1l */
   putLong_StrBuf(bp, 0);
   /* body len */
   /* 2l */
   putLong_StrBuf(bp, 0);
   /* 3l */
   putByte_StrBuf(bp, fp->isPublic_of_Function);
   /* 3l,0s,1b */
   putShort_StrBuf(bp, fp->nlocals_of_Function);
   /* 3l,1s,1b */
   putShort_StrBuf(bp, fp->reflocals_of_Function);
   /* 3l,2s,1b */
   putShort_StrBuf(bp, fp->maxdeep_of_Function + 1);
	count = fp->VarColl_privates_of_Function->unsorted.count_of_Coll + fp->VarColl_parameters_of_Function->unsorted.count_of_Coll;
   /* 3l,3s,1b */
   putShort_StrBuf(bp, count);
   /*!!! *//* 3l,4s,1b */
      putShort_StrBuf(bp, fp->VarColl_params_of_Function->unsorted.count_of_Coll);

   /* 3l,5s,1b */
	for (j = 0; j < fp->VarColl_privates_of_Function->unsorted.count_of_Coll; ++j)
   {
		VAR(Var, vp, fp->VarColl_privates_of_Function->unsorted.items_of_Coll[j]);
      putLong_StrBuf(bp, hashstr(vp->name));
   }
      for (j = 0; j < fp->VarColl_parameters_of_Function->unsorted.count_of_Coll; ++j)
   {
		VAR(Var, vp, fp->VarColl_parameters_of_Function->unsorted.items_of_Coll[j]);
      putLong_StrBuf(bp, hashstr(vp->name));
   }

   for (j = 0; j < fp->VarColl_locals_of_Function->coll.count_of_Coll; ++j)
   {
		VAR(Var, vp, fp->VarColl_locals_of_Function->coll.items_of_Coll[j]);
      putLong_StrBuf(bp, hashstr(vp->name));
   }
      for (j = 0; j < fp->VarColl_locals_of_Function->coll.count_of_Coll; ++j)
   {
		VAR(Var, vp, fp->VarColl_locals_of_Function->coll.items_of_Coll[j]);
      putShort_StrBuf(bp, vp->no);
   }

   for (namelen = 1, j = 0; j < fp->VarColl_locals_of_Function->unsorted.count_of_Coll; j++)
   {
		VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items_of_Coll[j]);
      namelen += strlen(vp->name) + 1;
   }

   if (fp->isPublic_of_Function == 0)
   {
      putByte_StrBuf(bp, strlen(fp->name_of_Function + 3));
      putStr_StrBuf(bp, fp->name_of_Function + 3);
   }
   else
   {
      putByte_StrBuf(bp, strlen(fp->name_of_Function));
      putStr_StrBuf(bp, fp->name_of_Function);
   }

   for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count_of_Coll; j++)
   {
		VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items_of_Coll[j]);
      if (vp->isRef)
	 continue;
      putStr_StrBuf(bp, vp->name);
   }
      for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count_of_Coll; j++)
   {
		VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items_of_Coll[j]);
      if (!vp->isRef)
	 continue;
      putStr_StrBuf(bp, vp->name);
   }

   SETLONG(bp, lp, BEGOFFS);
   lp += sizeof(long);

   blp = CUROFFS;
   /*printf("put_function: body: %s\n", fp->name_of_Function);fflush(stdout); */
   pass_Node(fp->Node_body_of_Function, OText, 0, bp);
   SETLONG(bp, lp, CUROFFS - blp);
   /*printf("put_function: end: %s\n", fp->name_of_Function);fflush(stdout); */
}
