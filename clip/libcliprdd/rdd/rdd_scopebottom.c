int
rdd_scopebottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, const char *__PROC__)
{
   RDD_ORDER *ro;

   int er;

   if (rd->curord == -1)
      return 0;

   ro = rd->orders[rd->curord];
   if (ro->scopebottom)
   {
      free(ro->scopebottom);
      ro->scopebottom = NULL;
      _clip_destroy(ClipMachineMemory, &ro->scopebottomvar);
   }
   if (v && v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
   {
      ro->scopebottom = malloc(ro->keysize);
      if ((er = ro->vtbl->formatkey(ClipMachineMemory, ro, v, ro->scopebottom, __PROC__)))
	 return er;
      _clip_clone(ClipMachineMemory, &ro->scopebottomvar, v);
      if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	 ro->sbotlen = min(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->keysize);
      else
	 ro->sbotlen = ro->keysize;
   }
   return 0;
}
