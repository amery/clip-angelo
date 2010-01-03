int
clip___DBLOCATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBLOCATE";

   ClipVar  *fexpr = _clip_spar(ClipMachineMemory, 1);

   ClipVar  *wexpr = _clip_spar(ClipMachineMemory, 2);

   ClipVar  *vnext = _clip_par(ClipMachineMemory, 3);

   ClipVar  *vrec = _clip_par(ClipMachineMemory, 4);

   ClipVar  *vrest = _clip_par(ClipMachineMemory, 5);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       found, er;


   if (!wa)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_locate(ClipMachineMemory, wa->rd, NULL, fexpr, wexpr, vnext, vrec, vrest, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;

   wa->found = found;
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
