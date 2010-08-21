int
clip_HS_CLOSE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_CLOSE";

   int h = _clip_parni(ClipMachineMemory, 1);

   HIPER *hs;

   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
      goto err;
   }

   if ((er = hs_close(ClipMachineMemory, hs, __PROC__)))
      goto err;

   _clip_retni(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}
