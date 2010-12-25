int
clip_SX_DESCEND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_DESCEND";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   int res, ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(2, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if ((er = wa->rd->orders[ord]->vtbl->descend(ClipMachineMemory, wa->rd->orders[ord], !wa->rd->orders[ord]->descend, &res, __PROC__)))
      return er;
   return 0;
 err:
   return er;

}
