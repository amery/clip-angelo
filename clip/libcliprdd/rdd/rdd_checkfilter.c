int
rdd_checkfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *ok, const char *__PROC__)
{
   /*1:true,0:false,-1:error */
   int hidedeleted = ClipMachineMemory->flags & DELETED_FLAG;

   int filterok = 1;

   int deletedok = 1;

   int er;

   *ok = 1;
   if (rd->indexing)
      return 0;
   if ((!rd->filter) && !hidedeleted)
      return 0;

   if (hidedeleted)
   {
      int deleted;

      if ((er = rd->vtbl->deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
	 return er;
      deletedok = !deleted;
   }
   if (deletedok && rd->filter)
   {
      int lastrec;

      if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	 return er;
      if (rd->recno > lastrec)
      {
	 *ok = 0;
	 return 0;
      }
      if (!rd->filter->custom && rd->filter->fps[0].bfilter.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      {
	 if (rd->filter->rmap)
	 {
	    filterok = _rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno);
	 }
	 if (filterok)
	 {
	    if ((er = rdd_calcfilter(ClipMachineMemory, rd, &filterok, __PROC__)))
	       return er;
	 }
      }
      else
      {
	 filterok = _rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno);
      }
   }
   *ok = filterok && deletedok;
   return 0;
}
