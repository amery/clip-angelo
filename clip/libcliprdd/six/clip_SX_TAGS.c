int
clip_SX_TAGS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_TAGS";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *index = _clip_par(ClipMachineMemory, 1);

   RDD_INDEX *ri = NULL;
   long vect[1] = { 0 };
   ClipVar *rp = RETPTR(ClipMachineMemory);

   int i, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   if (!wa)
   {
      _clip_array(ClipMachineMemory, rp, 1, vect);
      return 0;
   }

   if (!index || index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
   {
      if (wa->rd->curord == -1)
      {
	 if (wa->rd->idxs_opened > 0 && wa->rd->indices[0]->structural)
	 {
	    ri = wa->rd->indices[0];
	 }
	 else
	 {
	    _clip_array(ClipMachineMemory, rp, 1, vect);
	    return 0;
	 }
      }
      else
      {
	 ri = wa->rd->orders[wa->rd->curord]->index;
      }
   }
   else if (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
   {
      int no = index->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1;

      if (no >= 0 && no < wa->rd->idxs_opened)
	 ri = wa->rd->indices[no];
   }
   else
   {
      int i;

      for (i = 0; i < wa->rd->idxs_opened; i++)
      {
	 if (strcasecmp(wa->rd->indices[i]->name, index->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 0)
	 {
	    ri = wa->rd->indices[i];
	    break;
	 }
      }
   }

   if (!ri)
   {
      _clip_array(ClipMachineMemory, rp, 1, vect);
      return 0;
   }

   vect[0] = ri->norders;
   _clip_array(ClipMachineMemory, rp, 1, vect);

   for (i = 0; i < ri->norders; i++)
   {
      ClipVar v;

      RDD_ORDER *ro = ri->orders[i];

      CLEAR_CLIPVAR(&v);
      v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = ro->name;
      v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(ro->name);
      vect[0] = i;
      _clip_aset(ClipMachineMemory, rp, &v, 1, vect);
   }
   return 0;
 err:
   return er;
}
