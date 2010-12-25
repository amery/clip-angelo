static ClipVar *
fetch_obj(ClipMachine * ClipMachineMemory, ClipVar * obj, long hash)
{
   if (obj->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      return 0;
   return _clip_mget(ClipMachineMemory, obj, hash);
}
