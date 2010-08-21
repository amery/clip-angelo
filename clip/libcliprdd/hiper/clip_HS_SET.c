int
clip_HS_SET(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_SET";

   int h = _clip_parni(ClipMachineMemory, 1);

   int len;

   const char *val = _clip_parcl(ClipMachineMemory, 2, &len);

   int land = _clip_parl(ClipMachineMemory, 3);

   HIPER *hs;

   char *tmp, *s;

   int nvals = 0, i, j;

   int *lens = NULL;

   char **vals = NULL;

   unsigned int *ti;

   unsigned int count;

   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
      goto err;
   }

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

   if (hs->rmap)
   {
      free(hs->rmap);
      hs->rmap = NULL;
   }
   if ((er = hs_filter(ClipMachineMemory, hs, vals[0], lens[0], &hs->rmap, hs->lastrec, &count, __PROC__)))
      goto err;
   for (i = 1; i < nvals; i++)
   {
      if ((er = hs_filter(ClipMachineMemory, hs, vals[i], lens[i], &ti, hs->lastrec, &count, __PROC__)))
	 goto err;
      count = (hs->lastrec + 31) >> 5;
      for (j = 0; j < count; j++)
      {
	 if (land)
	    hs->rmap[j] &= ti[j];
	 else
	    hs->rmap[j] |= ti[j];
      }
      free(ti);
   }
   hs->recno = 0;
   return 0;
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
   return er;
}
