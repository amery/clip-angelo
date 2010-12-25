int
clip_RDDLOCATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDLOCATE";

   int r = _clip_parni(ClipMachineMemory, 1);

   char *fe = _clip_parc(ClipMachineMemory, 2);

   char *we = _clip_parc(ClipMachineMemory, 3);

   ClipVar *vnext = _clip_par(ClipMachineMemory, 4);

   ClipVar *vrec = _clip_par(ClipMachineMemory, 5);

   ClipVar *vrest = _clip_par(ClipMachineMemory, 6);

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   ClipVar fexpr;

   ClipVar wexpr;

   int found, er;

   char expexp[1024];

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);

   memset(&fexpr, 0, sizeof(ClipVar));
   memset(&wexpr, 0, sizeof(ClipVar));
   memset(expexp, 0, sizeof(expexp));
   rdd_expandmacro(rd->area, r, fe, expexp);
   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = _clip_eval_macro(ClipMachineMemory, expexp, strlen(expexp), &fexpr)))
      goto err_unlock;
   if (we)
   {
      memset(expexp, 0, sizeof(expexp));
      rdd_expandmacro(rd->area, r, we, expexp);
      if ((er = _clip_eval_macro(ClipMachineMemory, expexp, strlen(expexp), &wexpr)))
	 goto err_unlock;
   }
   if ((er = rdd_locate(ClipMachineMemory, rd, fe, &fexpr, &wexpr, vnext, vrec, vrest, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_destroy(ClipMachineMemory, &fexpr);
   _clip_destroy(ClipMachineMemory, &wexpr);

   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   _clip_destroy(ClipMachineMemory, &fexpr);
   _clip_destroy(ClipMachineMemory, &wexpr);
   return er;
}
