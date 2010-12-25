/*
   block = compile("{||qout('asdf')}")
 */
int
clip_COMPILE(ClipMachine * ClipMachineMemory)
{
   char *str;

   int len, r;

   ClipBlock block;

   ClipVar *rp, *vp;

   str = _clip_parcl(ClipMachineMemory, 1, &len);

   if (!str)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "COMPILE: non-character parameter");
      return 1;
   }

   r = _clip_compile_Block(ClipMachineMemory, str, len, &block);

   if (r)
   {
      _clip_trap(ClipMachineMemory, __FILE__, __LINE__);
      return r;
   }
   rp = RETPTR(ClipMachineMemory);

   vp = NEW(ClipVar);
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar = NEW(ClipBlock);
   *vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar = block;

   CLEAR_CLIPVAR(rp);
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   rp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

   return 0;
}
