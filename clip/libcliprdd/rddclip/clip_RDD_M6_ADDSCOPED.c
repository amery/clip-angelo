int
clip_RDD_M6_ADDSCOPED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_M6_ADDSCOPED";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int h = _clip_parni(ClipMachineMemory, 2);

   RDD_FILTER *fp;

   ClipVar *t = _clip_vptr(_clip_par(ClipMachineMemory, 3));

   ClipVar *b = _clip_vptr(_clip_par(ClipMachineMemory, 4));

   int ord = _clip_parni(ClipMachineMemory, 5) - 1;

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(5, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if (!fp->rmap)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }
   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      ord = rd->curord;
   if (ord >= rd->ords_opened)
      ord = -1;
   if (ord == -1)
   {
      er = rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);
      goto err;
   }

   if (t->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      t = NULL;
   if (b->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      b = NULL;

   READLOCK;
   if ((er = rd->orders[ord]->vtbl->setscope(ClipMachineMemory, rd, rd->orders[ord], t, b, fp->rmap, fp->size, 0, __PROC__)))
      goto err_unlock;
   UNLOCK;

   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
