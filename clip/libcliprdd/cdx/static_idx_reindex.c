static int
idx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   RDD_ORDER *ro;

   int lastrec;

   BTREE1 *bt;

   void *buf;

   ClipVar vv, *vp;

   int interval = max(1, rd->os.nInterval);

   unsigned int oldrn = rd->recno;

   int er;

   memset(&vv, 0, sizeof(ClipVar));

   if ((er = idx_zap(ClipMachineMemory, rd, ri, __PROC__)))
      return er;
   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      return er;

   ro = ri->orders[0];
   if (ro->custom)
      return 0;
   rd->tagindexing = ro->name;
   bt = bt1_create(ro->unique, ro->bufsize + sizeof(unsigned int), _cdx_compare, ClipMachineMemory->index_buffer_limit);
   buf = malloc(ro->bufsize + sizeof(unsigned int));
   for (rd->recno = 1; rd->recno <= lastrec; rd->recno++)
   {
      rd->eof = 0;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
	 return er;
      if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      {
	 if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
	    return er;
	 vp = _clip_vptr(&vv);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	    return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	 if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	 {
	    _clip_destroy(ClipMachineMemory, &vv);
	    continue;
	 }
	 _clip_destroy(ClipMachineMemory, &vv);
      }
      if (bt)
      {
	 if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	    return er;
	 vp = _clip_vptr(&vv);
	 memcpy(buf, &rd->recno, sizeof(unsigned int));
	 if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, buf + sizeof(unsigned int), __PROC__)))
	    return er;
	 _clip_destroy(ClipMachineMemory, &vv);
	 if (bt1_add(bt, ro, buf))
	 {
	    if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
	    {
	       bt1_destroy(bt);
	       return er;
	    }
	    bt1_destroy(bt);
	    bt = NULL;
	    if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	       return er;
	 }
      }
      else
      {
	 if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	    return er;
      }
      /* bEval every nInterval */
      if (rd->recno % interval == 0 && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      {
	 if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->os.bEval, &vv, 0)))
	    return er;
	 vp = _clip_vptr(&vv);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	    return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
	 if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	 {
	    _clip_destroy(ClipMachineMemory, &vv);
	    break;
	 }
	 _clip_destroy(ClipMachineMemory, &vv);
      }
   }
   if (bt)
   {
      if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
      {
	 bt1_destroy(bt);
	 return er;
      }
      bt1_destroy(bt);
   }
   free(buf);

   rd->tagindexing = NULL;
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrn, 0, __PROC__)))
      return er;
   return 0;
}
