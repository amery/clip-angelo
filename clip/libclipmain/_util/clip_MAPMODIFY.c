/*
   enable/disable auto-call of map:modify for map modification

   MAPMODIFY(map [, lNewVal]) -> lPrevVal

   modify will be called as:
   modify(map, nFieldHash, newVal) -> valToAssign
 */
int
clip_MAPMODIFY(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   int ret;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      return 0;

   ret = vp->ClipType_t_of_ClipVar.memo_of_ClipType;
   if (ClipMachineMemory->argc > 1)
      vp->ClipType_t_of_ClipVar.memo_of_ClipType = _clip_parl(ClipMachineMemory, 2);

   _clip_retl(ClipMachineMemory, ret);

   return 0;
}
