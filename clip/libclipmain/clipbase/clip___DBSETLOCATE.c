int
clip___DBSETLOCATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBSETLOCATE";

   ClipVar *fexpr = _clip_spar(ClipMachineMemory, 1);

   ClipVar *rp = RETPTR(ClipMachineMemory);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int er;

   if (!wa)
      return 0;

   CHECKOPT2(1, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
   {
      if (wa->rd->locate_filter && wa->rd->locate_filter->fps)
	 _clip_dup(ClipMachineMemory, rp, &wa->rd->locate_filter->fps->bfilter);
   }
   else
   {
      if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
	 goto err;
      READLOCK;
      if (wa->rd->locate_filter)
	 if ((er = rdd_destroyfilter(ClipMachineMemory, wa->rd->locate_filter, __PROC__)))
	    goto err_unlock;
      if ((er = rdd_createfilter(ClipMachineMemory, wa->rd, &wa->rd->locate_filter, fexpr, NULL, NULL, 0, __PROC__)))
	 goto err_unlock;
      if (wa->rd->filter)
	 if ((er = rm_intersect(ClipMachineMemory, wa->rd->locate_filter, wa->rd->filter, __PROC__)))
	    goto err_unlock;
      UNLOCK;
   }
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
