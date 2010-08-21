int
clip_HS_KEYCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_KEYCOUNT";

   int h = _clip_parni(ClipMachineMemory, 1);

   HIPER *hs;

   unsigned char c4[4];

   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
      goto err;
   }

   if ((er = rdd_read(ClipMachineMemory, &hs->file, 10, 4, c4, __PROC__)))
      goto err;
   _clip_retni(ClipMachineMemory, _rdd_uint(c4));
   return 0;
 err:
   return er;
}
