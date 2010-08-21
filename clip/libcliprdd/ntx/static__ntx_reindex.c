static int
_ntx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   int lastrec, i, er;

   ClipVar vv, *vp;

   if (ro->custom)
      return 0;

   rd->tagindexing = ro->name;
   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      return er;
   i = max(1, rd->os.nInterval);
   for (rd->recno = 1; rd->recno <= lastrec; rd->recno++)
   {
      rd->eof = 0;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
	 return er;
      if ((er = ntx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	 return er;
      /* bEval every nInterval */
      if (rd->recno % i == 0 && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
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
   rd->tagindexing = NULL;
   return 0;
}
