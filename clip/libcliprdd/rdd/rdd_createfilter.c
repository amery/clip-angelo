int
rdd_createfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER ** fpp, ClipVar * _block, const char *str, ClipVar * remap, int lNoOptimize, const char *__PROC__)
{
   char expr[PATH_MAX];

   RDD_FILTER *fp = calloc(sizeof(RDD_FILTER), 1);

   int er;

   *fpp = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   fp->fps = calloc(1, sizeof(RDD_FPS));
   fp->rd = rd;
   if (str)
   {
      fp->sfilter = strdup(str);
   }
   if ((ClipMachineMemory->flags1 & OPTIMIZE_FLAG) && !lNoOptimize && str)
   {
      er = rdd_initrushmore(ClipMachineMemory, rd, fp, remap, 0, __PROC__);
      if (fp->rmap)
	 _clip_destroy(ClipMachineMemory, _block);
   }
   else if (str && remap && remap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      er = rdd_initrushmore(ClipMachineMemory, rd, fp, remap, 1, __PROC__);
      if (fp->rmap)
	 _clip_destroy(ClipMachineMemory, _block);
   }
   fp->nfps = 1;
   if ((!_block || _block->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) && str)
   {
      rdd_expandmacro(rd->area, _clip_parni(ClipMachineMemory, 1), fp->sfilter, expr);
      if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &fp->fps->bfilter)))
	 goto err;
   }
   else
   {
      _clip_clone(ClipMachineMemory, &fp->fps->bfilter, _block);
   }
   fp->handle = _clip_store_c_item(ClipMachineMemory, fp, _C_ITEM_TYPE_RYO, destroy_ryo);
   *fpp = fp;
   return 0;
 err:
   free(fp);
   return er;
}
