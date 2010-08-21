int
clip_ORDDESTROY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDDESTROY";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   int ord, er;

   if (!wa)
      return 0;

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return 0;

   if ((er = rdd_orddestroy(ClipMachineMemory, wa->rd, wa->rd->orders[ord], __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
