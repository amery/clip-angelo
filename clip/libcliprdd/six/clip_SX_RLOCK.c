int
clip_SX_RLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_RLOCK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int t = _clip_parinfo(ClipMachineMemory, 1);

   ClipVar *ap, *vp;

   int ok = 1, aok, i, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, ARRAY_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   READLOCK;
   if (t == ARRAY_type_of_ClipVarType)
   {
      ap = _clip_vptr(_clip_par(ClipMachineMemory, 1));
      for (i = 0; i < ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
      {
	 vp = _clip_vptr(&ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i]);
	 if ((er = rdd_rlock(ClipMachineMemory, wa->rd, (unsigned int) vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, &aok, __PROC__)))
	    goto err_unlock;
	 if (!aok)
	    ok = 0;
      }
   }
   else if (t == NUMERIC_type_of_ClipVarType)
   {
      if ((er = rdd_rlock(ClipMachineMemory, wa->rd, _clip_parni(ClipMachineMemory, 1), &ok, __PROC__)))
	 goto err_unlock;
   }
   else if (t == UNDEF_type_of_ClipVarType)
   {
      if ((er = rdd_rlock(ClipMachineMemory, wa->rd, wa->rd->recno, &ok, __PROC__)))
	 goto err_unlock;
   }
   _clip_retl(ClipMachineMemory, ok);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
