int
clip_DBCREATEINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCREATEINDEX";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   const char *name = _clip_parc(ClipMachineMemory, 1);

   const char *expr = _clip_parc(ClipMachineMemory, 2);

   ClipVar  *block = _clip_spar(ClipMachineMemory, 3);

   int       unique = _clip_parl(ClipMachineMemory, 4);

   int       er;


   CHECKWA(wa);
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(3, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      unique = ClipMachineMemory->flags & UNIQUE_FLAG;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   if (wa->rd->readonly)
    {
       READLOCK;
    }
   else
    {
       WRITELOCK;
    }
   if ((er = rdd_createindex(ClipMachineMemory, wa->rd, wa->idx_driver, name, NULL, expr, block, unique, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
