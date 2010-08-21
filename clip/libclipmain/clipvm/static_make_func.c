static void
make_func(struct ClipFile *file, ClipCodeVar * dest, long *hash)
{
   char *modbeg = M_OFFS(file->body_of_ClipFile, 2, 0);

   dest->ClipType_type_of_ClipCodeVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
   dest->ClipType_type_of_ClipCodeVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   dest->union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar->file_of_ClipBlock = file;
   SETLONG(hash, GETLONG(file->pos_of_ClipFile + 0));
   dest->union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar->function_of_ClipBlock = modbeg + GETLONG(file->pos_of_ClipFile + 1);
}
