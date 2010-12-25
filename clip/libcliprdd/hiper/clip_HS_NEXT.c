int
clip_HS_NEXT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_REMOVE";

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

   if (!hs->rmap)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }

   while ((++hs->recno <= hs->lastrec) && ((!_rm_getbit(hs->rmap, hs->lastrec, hs->recno)) || (_rm_getbit(hs->map, hs->lastrec, hs->recno))));

   _clip_retni(ClipMachineMemory, hs->recno > hs->lastrec ? 0 : hs->recno);
   return 0;
 err:
   return er;
}
