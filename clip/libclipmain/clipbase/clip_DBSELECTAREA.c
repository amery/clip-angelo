int
clip_DBSELECTAREA(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   ClipVar *data;

   int ret = -1, er;

   if (!vp)
   {
      _clip_retni(ClipMachineMemory, ClipMachineMemory->curArea + 1);
      return 0;
   }

   data = NEW(ClipVar);
   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
   {
      _clip_eval(ClipMachineMemory, _clip_vptr(vp), 0, 0, data);
      vp = data;
   }
   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      char ch = toupper(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0]);

      if (ch >= '0' && ch <= '9')
	 ret = atol(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
      else if ((vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf == 1) && (ch >= 'A') && (ch <= 'L'))
	 ret = ch - 'A' + 1;
   }

   if (ret >= 0 || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
   {
      int n;

      if (ret >= 0)
	 n = ret;
      else
	 n = _clip_double(vp);

      if (n)
      {
	 ret = n - 1;
	 if (n > ClipMachineMemory->areas->count_of_ClipVect)
	 {
	    n -= ClipMachineMemory->areas->count_of_ClipVect;
	    while (n--)
	       add_ClipVect(ClipMachineMemory->areas, NULL);
	 }
      }
      else
      {
	 for (; n < ClipMachineMemory->areas->count_of_ClipVect; n++)
	 {
	    DBWorkArea *wa;

	    wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[n];
	    if (wa == NULL || !wa->used)
	       break;
	 }
	 ret = n;
	 if (ret == ClipMachineMemory->areas->count_of_ClipVect)
	    add_ClipVect(ClipMachineMemory->areas, NULL);
      }
      ClipMachineMemory->curArea = ret;
   }
   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      int len;

      long hash;

      DBWorkArea *wa;

      len = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf > 10 ? 10 : vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      hash = _clip_casehashword(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
      wa = HashTable_fetch(ClipMachineMemory->aliases, hash);

      if (wa)
	 ClipMachineMemory->curArea = wa->no;
      else
      {
	 er = rdd_err(ClipMachineMemory, EG_NOALIAS, 0, __FILE__, __LINE__, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, er_noalias);
	 goto err;
      }
   }

   _clip_retni(ClipMachineMemory, ret + 1);
   _clip_destroy(ClipMachineMemory, data);
   free(data);
   return 0;
 err:
   _clip_destroy(ClipMachineMemory, data);
   free(data);
   return er;
}
