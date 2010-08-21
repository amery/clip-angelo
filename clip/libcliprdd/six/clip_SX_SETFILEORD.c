int
clip_SX_SETFILEORD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SETFILEORD";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int ord = _clip_parni(ClipMachineMemory, 1) - 1;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (!wa)
      return 0;

   if (wa->rd->curord == -1)
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, wa->rd->orders[wa->rd->curord]->index->indexno + 1);

   if (ord >= 0 && ord < wa->rd->idxs_opened)
   {
      if ((er = rdd_setorder(ClipMachineMemory, wa->rd, wa->rd->indices[ord]->orders[0]->orderno + 1, __PROC__)))
	 goto err;
   }
   else if (ord == -1)
   {
      if ((er = rdd_setorder(ClipMachineMemory, wa->rd, 0, __PROC__)))
	 goto err;
   }
   return 0;
 err:
   return er;
}
