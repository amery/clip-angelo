int
rdd_scopetop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, const char *__PROC__)
{
   RDD_ORDER *ro;

   int er;

   if (rd->curord == -1)
      return 0;

   ro = rd->orders[rd->curord];
   if (ro->scopetop)
   {
      free(ro->scopetop);
      ro->scopetop = NULL;
      _clip_destroy(ClipMachineMemory, &ro->scopetopvar);
   }
   if (v && v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
   {
      ro->scopetop = malloc(ro->keysize);
      if ((er = ro->vtbl->formatkey(ClipMachineMemory, ro, v, ro->scopetop, __PROC__)))
	 return er;
      _clip_clone(ClipMachineMemory, &ro->scopetopvar, v);
      if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	 ro->stoplen = min(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->keysize);
      else
	 ro->stoplen = ro->keysize;
   }
   return 0;
}
