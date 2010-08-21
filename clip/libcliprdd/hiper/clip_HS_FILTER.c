int
clip_HS_FILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_FILTER";

   const char *fname = _clip_parc(ClipMachineMemory, 1);

   int len;

   const char *val = _clip_parcl(ClipMachineMemory, 2, &len);

   int elen;

   const char *expr = _clip_parcl(ClipMachineMemory, 3, &elen);

   int land = _clip_parl(ClipMachineMemory, 4);

   int mode = _clip_parni(ClipMachineMemory, 5);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   HIPER *hs = NULL;

   RDD_FILTER *fp;

   unsigned int count;

   char *fexpr = NULL, *tmp, *s;

   char **vals = NULL;

   int *lens = NULL;

   int nvals = 0, i, j;

   unsigned int *ti;

   int er;

   _clip_retni(ClipMachineMemory, 0);
   CHECKWA(wa);

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(4, LOGICAL_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(5, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 4) == NUMERIC_type_of_ClipVarType || _clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      land = 0;
   if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   if ((er = hs_open(ClipMachineMemory, fname, mode, &hs, __PROC__)))
      goto err_unlock;
   if (hs->filtset == 1)
   {
      tmp = strdup(val);
      nvals = 0;
      s = strtok(tmp, " ");
      while (s)
      {
	 lens = realloc(lens, sizeof(int) * (nvals + 1));
	 vals = realloc(vals, sizeof(char *) * (nvals + 1));
	 lens[nvals] = strlen(s);
	 vals[nvals] = strdup(s);
	 nvals++;
	 s = strtok(NULL, " ");
      }
      free(tmp);
   }
   else
   {
      vals = malloc(sizeof(char *));
      vals[0] = strdup(val);
      lens = malloc(sizeof(int));
      lens[0] = len;
      nvals = 1;
   }
   if (!(expr && _clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType && elen > 0))
   {
      fexpr = malloc(4);
      strcpy(fexpr, ".t.");
   }
   else
   {
      fexpr = calloc(1, 1);
      for (i = 0; i < nvals; i++)
      {
	 fexpr = realloc(fexpr, strlen(fexpr) + elen + strlen(vals[i]) + 27);
	 if (hs->lcase)
	 {
	    if (i == 0)
	    {
	       sprintf(fexpr + strlen(fexpr), "UPPER('%s') $ UPPER(%s)", vals[i], expr);
	    }
	    else
	    {
	       if (land)
		  sprintf(fexpr + strlen(fexpr), " .AND. UPPER('%s') $ UPPER(%s)", vals[i], expr);
	       else
		  sprintf(fexpr + strlen(fexpr), " .OR. UPPER('%s') $ UPPER(%s)", vals[i], expr);
	    }
	 }
	 else
	 {
	    if (i == 0)
	    {
	       sprintf(fexpr + strlen(fexpr), "'%s' $ (%s)", vals[i], expr);
	    }
	    else
	    {
	       if (land)
		  sprintf(fexpr + strlen(fexpr), " .AND. '%s' $ (%s)", vals[i], expr);
	       else
		  sprintf(fexpr + strlen(fexpr), " .OR. '%s' $ (%s)", vals[i], expr);
	    }
	 }
      }
   }

   if ((er = rdd_createfilter(ClipMachineMemory, wa->rd, &fp, NULL, fexpr, NULL, 0, __PROC__)))
      goto err_unlock;
   free(fexpr);
   fexpr = NULL;
   fp->active = 1;
   wa->rd->filter = fp;

   if ((er = rdd_lastrec(ClipMachineMemory, wa->rd, &wa->rd->filter->size, __PROC__)))
      goto err_unlock;
   if ((er = hs_filter(ClipMachineMemory, hs, vals[0], lens[0], &wa->rd->filter->rmap, wa->rd->filter->size, &count, __PROC__)))
      goto err_unlock;
   for (i = 1; i < nvals; i++)
   {
      if ((er = hs_filter(ClipMachineMemory, hs, vals[i], lens[i], &ti, wa->rd->filter->size, &count, __PROC__)))
	 goto err_unlock;
      count = (wa->rd->filter->size + 31) / 32;
      for (j = 0; j < count; j++)
      {
	 if (land)
	    wa->rd->filter->rmap[j] &= ti[j];
	 else
	    wa->rd->filter->rmap[j] |= ti[j];
      }
      free(ti);
   }
   if ((er = hs_close(ClipMachineMemory, hs, __PROC__)))
      goto err_unlock;

   if (expr && _clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType && len > 2)
   {
      if ((er = rm_evalpartial(ClipMachineMemory, fp, NULL, (unsigned int *) &len, &count, __PROC__)))
	 goto err_unlock;
   }
   if (wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__))
      goto err;

   for (i = 0; i < nvals; i++)
   {
      free(vals[i]);
   }
   free(vals);
   free(lens);
   _clip_retni(ClipMachineMemory, count);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   if (vals)
   {
      for (i = 0; i < nvals; i++)
      {
	 free(vals[i]);
      }
      free(vals);
   }
   if (lens)
      free(lens);
   if (fexpr)
      free(fexpr);
   return er;
}
