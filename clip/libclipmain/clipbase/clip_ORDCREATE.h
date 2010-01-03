int
clip_ORDCREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDCREATE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   const char *name = _clip_parc(ClipMachineMemory, 1);

   const char *tag = _clip_parc(ClipMachineMemory, 2);

   const char *expr = _clip_parc(ClipMachineMemory, 3);

   ClipVar  *block = _clip_spar(ClipMachineMemory, 4);

   int       unique = _clip_parl(ClipMachineMemory, 5);

   char     *nm = NULL;

   char     *s = NULL;

   int       er;


   CHECKWA(wa);
   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(4, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(5, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      unique = ClipMachineMemory->flags & UNIQUE_FLAG;

   if (!name)
    {
       nm = strdup(wa->rd->path);
       s = strrchr(nm, '/');
       strcpy(s + 1, wa->rd->name);
    }

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
   if ((er = rdd_createindex(ClipMachineMemory, wa->rd, wa->idx_driver, nm ? nm : name, tag, expr, block, unique, __PROC__)))
      goto err_unlock;
   UNLOCK;
   if (nm)
      free(nm);
   return 0;
 err_unlock:
   if (nm)
      free(nm);
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
