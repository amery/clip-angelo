typedef struct ClipType
{
   enum ClipVarType ClipVartype_type_of_ClipType:4;
   unsigned len_of_ClipType:6;
   unsigned dec_of_ClipType:6;
   enum ClipFlags ClipFlags_flags_of_ClipType:2;
   unsigned count_of_ClipType:11;
   unsigned memo_of_ClipType:1;
   unsigned field_of_ClipType:1;
/*	unsigned noref:1;*/
}
ClipType;
