int
clip_M6_ISOPTIMIZE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ISOPTIMIZE";

   char *str = _clip_parc(ClipMachineMemory, 1);

   RDD_FILTER *fp = calloc(1, sizeof(RDD_FILTER));

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   ClipMachineMemory->m6_error = 0;
   if (!wa)
   {
      ClipMachineMemory->m6_error = 2001;
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   if (!wa->rd->ords_opened)
   {
      ClipMachineMemory->m6_error = 2005;
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }

   if (str)
   {
      fp->sfilter = strdup(str);
   }
   READLOCK;
   if ((er = rdd_initrushmore(ClipMachineMemory, wa->rd, fp, NULL, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, fp->optimize);

   if (fp->rmap)
      free(fp->rmap);
   free(fp);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   if (fp->rmap)
      free(fp->rmap);
   free(fp);
   return er;
}
