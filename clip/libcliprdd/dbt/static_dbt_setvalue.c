static int
dbt_setvalue(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int *id, ClipVar * vp, int binary, const char *__PROC__)
{
   ClipVar old;

   int pages, er;

   char *buf;

   int isnew = 0;

   char fuu[4];

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, "Incompatible types");
   loc_write(rm->loc, (unsigned char *) vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

   if ((er = dbt_getvalue(ClipMachineMemory, rm, *id, &old, __PROC__)))
      return er;
   pages =
      (old.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 2) / DBT_PAGE_SIZE + ((old.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 2) % DBT_PAGE_SIZE != 0);
   if (!*id || vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 2 > pages * DBT_PAGE_SIZE)
   {
      if ((er = rdd_read(ClipMachineMemory, &rm->file, 0, 4, fuu, __PROC__)))
	 return er;
      *id = _rdd_uint((unsigned char *) fuu);
      isnew = 1;
   }
   pages =
      (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 2) / DBT_PAGE_SIZE + ((vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 2) % DBT_PAGE_SIZE != 0);
   buf = malloc(pages * DBT_PAGE_SIZE);
   memcpy(buf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
   memset(buf + vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, 0x1a, 2);
   if ((er = rdd_write(ClipMachineMemory, &rm->file, *id * DBT_PAGE_SIZE, pages * DBT_PAGE_SIZE, buf, __PROC__)))
      return er;
   free(buf);
   if (isnew)
   {
      _rdd_put_uint((unsigned char *) fuu, *id + pages);
      if ((er = rdd_write(ClipMachineMemory, &rm->file, 0, 4, fuu, __PROC__)))
	 return er;
   }
   rm->updated = 1;
   return 0;
}
