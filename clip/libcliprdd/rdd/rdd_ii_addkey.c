int
rdd_ii_addkey(ClipMachine * ClipMachineMemory, int h, const char *id, ClipVar * key, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   if ((key->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && ro->type != 'C') ||
       (key->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && ro->type != 'N') ||
       (key->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType && ro->type != 'D') ||
       (key->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType && ro->type != 'L'))
      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, "Type mismatch");

   return ro->vtbl->ii_addkey(ClipMachineMemory, ro, id, key, __PROC__);
}
