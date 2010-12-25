int
clip_HS_IFDEL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_IFDEL";

   int h = _clip_parni(ClipMachineMemory, 1);

   unsigned int rec = _clip_parni(ClipMachineMemory, 2);

   HIPER *hs;

   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
      goto err;
   }

   _clip_retni(ClipMachineMemory, _rm_getbit(hs->map, hs->lastrec, rec) != 0);
   return 0;
 err:
   return er;
}
