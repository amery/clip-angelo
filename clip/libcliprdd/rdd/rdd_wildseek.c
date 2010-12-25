int
rdd_wildseek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *pat, int regular, int cont, int *found, const char *__PROC__)
{
   char *e;

   char *s;

   char *q;

   char *b = NULL;

   char *pattern;

   int er;

   if ((rd->ords_opened < 1) || (rd->curord == -1))
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if (rd->orders[rd->curord]->type != 'C')
      return rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, "Order is not CHARACTER");

   pattern = strdup(pat);
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      goto err;

   e = (char *) pattern + strlen(pattern);
   loc_write(rd->loc, (unsigned char *) pattern, e - pattern);
   if (!regular)
   {
      s = strchr(pattern, '*');
      q = strchr(pattern, '?');
      if (!s)
	 s = e;
      if (!q)
	 q = e;
      if (!b)
	 b = e;
      s = min(min(s, q), b);
   }
   else
   {
      s = NULL;
   }

   if (!cont)
   {
      ClipVar v;

      int fnd;

      if ((s == pattern) || regular)
      {
	 if ((er = rd->orders[rd->curord]->vtbl->gotop(ClipMachineMemory, rd, rd->orders[rd->curord], __PROC__)))
	    goto err;
      }
      else
      {
	 memset(&v, 0, sizeof(ClipVar));
	 v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(s - pattern + 1);
	 memcpy(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, pat, s - pattern);
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[s - pattern] = 0;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = s - pattern;
	 if ((er = rd->orders[rd->curord]->vtbl->seek(ClipMachineMemory, rd, rd->orders[rd->curord], &v, 0, 0, &fnd, __PROC__)))
	    goto err;
	 free(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
      }
   }
   else
   {
      if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
	 goto err;
   }
   if (rd->eof)
   {
      *found = 0;
   }
   else
   {
      if ((er = _rdd_wildskip(ClipMachineMemory, rd, pattern, regular, s, found, __PROC__)))
	 goto err;
      if (!(*found))
      {
	 unsigned int lastrec;

	 if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	    goto err;
	 rd->eof = 1;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    goto err;
      }
   }
   free(pattern);
   return 0;
 err:
   free(pattern);
   return er;
}
