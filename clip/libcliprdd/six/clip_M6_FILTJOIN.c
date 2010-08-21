int
clip_M6_FILTJOIN(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTJOIN";

   int h1 = _clip_parni(ClipMachineMemory, 1);

   int h2 = _clip_parni(ClipMachineMemory, 2);

   int oper = _clip_parni(ClipMachineMemory, 3);

   RDD_FILTER *f1;

   RDD_FILTER *f2;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   CHECKARG1(3, NUMERIC_type_of_ClipVarType);

   if (oper < 1 || oper > 3)
   {
      er = rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, er_invjoin);
      goto err;
   }
   f1 = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h1, _C_ITEM_TYPE_RYO);
   f2 = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h2, _C_ITEM_TYPE_RYO);

   if ((er = rm_checkjoin(ClipMachineMemory, f1, f2, __PROC__)))
      goto err;

   if (oper == 1)
   {
      if ((er = rm_union(ClipMachineMemory, f1, f2, __PROC__)))
	 goto err;
   }
   else if (oper == 2)
   {
      if ((er = rm_intersect(ClipMachineMemory, f1, f2, __PROC__)))
	 goto err;
   }
   else
   {
      if (!f1->custom)
      {
	 er = rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
	 goto err;
      }
      if ((er = rm_difference(ClipMachineMemory, f1, f2, __PROC__)))
	 goto err;
   }
   if (!f2->active)
   {
      if ((er = rdd_destroyfilter(ClipMachineMemory, f2, __PROC__)))
	 goto err;
   }
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}
