static int
fpt_setvalue(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int *id, ClipVar * vp, int binary, const char *__PROC__)
{
   long l;

   int type, i, er;

   char *str = NULL;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
   {
      _clip_var2str(ClipMachineMemory, vp, &str, &l, 0);
   }
   else
   {
      l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      str = _clip_memdup(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l);
      if (!binary)
	 loc_write(rm->loc, (unsigned char *) str, l);
   }

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
   {
      type = 3;
   }
   else
   {
      type = (binary == 0);
   }
   if ((er = flex_freeblock(ClipMachineMemory, rm, *id, __PROC__)))
      goto err;
   if ((er = flex_useblock(ClipMachineMemory, rm, id, l, __PROC__)))
      goto err;

   for (i = 0; i < rm->flex_nremoved; i++)
   {
      if ((er = _flex_freeblock(ClipMachineMemory, rm, rm->flex_removed[i], FLEX_PAGESIZE, __PROC__)))
	 return er;
   }
   free(rm->flex_removed);
   rm->flex_removed = NULL;
   rm->flex_nremoved = 0;

   if ((er = _fpt_setvalue(ClipMachineMemory, rm, *id, type, str, l, __PROC__)))
      goto err;
   free(str);
   rm->updated = 1;
   return 0;
 err:
   free(str);
   return er;
}
