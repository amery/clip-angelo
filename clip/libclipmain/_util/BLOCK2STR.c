int
BLOCK2STR(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   ClipBlock *bp;

   ClipVar *rp;

   int l;

   if (!vp || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "block2str: non-pcode parameter");
      return 1;
   }

   bp = vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar;
   rp = RETPTR(ClipMachineMemory);
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l = bp->file_of_ClipBlock->bodySize_of_ClipFile;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) malloc(l);
   memcpy(rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, bp->file_of_ClipBlock->body_of_ClipFile, l);

   return 0;
}
