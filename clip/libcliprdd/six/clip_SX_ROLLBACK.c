int
clip_SX_ROLLBACK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ROLLBACK";

   int nwa = _clip_parni(ClipMachineMemory, 1);

   DBWorkArea *wa;

   int i, j, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   if (nwa > 0)
      wa = get_area(ClipMachineMemory, nwa, 0);
   else
      wa = cur_area(ClipMachineMemory);

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (nwa > 0)
      wa = get_area(ClipMachineMemory, nwa, 0);
   else
      wa = cur_area(ClipMachineMemory);

   WRITELOCK;
   for (i = 0; i < wa->rd->nfields; i++)
   {
      if (wa->rd->data[i])
      {
	 if ((er = rdd_setvalue(ClipMachineMemory, wa->rd, i, wa->rd->data[i], __PROC__)))
	    goto err_unlock;
	 _clip_destroy(ClipMachineMemory, wa->rd->data[i]);
	 free(wa->rd->data[i]);
	 wa->rd->data[i] = NULL;
      }
   }
   for (j = 0; j < wa->rd->rels_opened; j++)
   {
      RDD_DATA *child = wa->rd->relations[j]->child;

      for (i = 0; i < child->nfields; i++)
      {
	 if (child->data[i])
	 {
	    if ((er = rdd_setvalue(ClipMachineMemory, child, i, child->data[i], __PROC__)))
	       goto err_unlock;
	    _clip_destroy(ClipMachineMemory, child->data[i]);
	    free(child->data[i]);
	    child->data[i] = NULL;
	 }
      }
   }
   UNLOCK;

   _clip_retl(ClipMachineMemory, 1);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
