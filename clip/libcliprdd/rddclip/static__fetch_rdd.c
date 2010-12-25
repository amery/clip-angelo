static RDD_DATA *
_fetch_rdd(ClipMachine * ClipMachineMemory, const char *__PROC__)
{
   RDD_DATA *rd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory,
						  _clip_parni(ClipMachineMemory, 1),
						  _C_ITEM_TYPE_RDD);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
   {
      rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badinstance);
      return NULL;
   }
   if (!rd)
   {
      rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_badinstance);
      return NULL;
   }
   return rd;
}
