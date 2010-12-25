int
clip_ORDDESCEND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDDESCEND";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   int descend = _clip_parl(ClipMachineMemory, 3);

   int res, ord, er;

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      descend = -1;
   if ((er = wa->rd->orders[ord]->vtbl->descend(ClipMachineMemory, wa->rd->orders[ord], descend, &res, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, res);
   return 0;
 err:
   return er;
}
