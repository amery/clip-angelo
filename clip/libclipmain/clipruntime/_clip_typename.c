CLIP_DLLEXPORT const char *
_clip_typename(ClipVar * vp)
{
   enum ClipVarType type = _clip_type(vp);

   if (!vp)
      return "NIL";
   switch (type)
   {
   case UNDEF_type_of_ClipVarType:
      return "UNDEF";
   case NUMERIC_type_of_ClipVarType:
      return "NUMERIC";
   case CHARACTER_type_of_ClipVarType:
      return "CHARACTER";
   case LOGICAL_type_of_ClipVarType:
      return "LOGICAL";
   case DATE_type_of_ClipVarType:
      return "DATE";
   case OBJECT_type_of_ClipVarType:
      return "OBJECT";
   case PCODE_type_of_ClipVarType:
      return "PCODE";
   case CCODE_type_of_ClipVarType:
      return "CCODE";
   case ARRAY_type_of_ClipVarType:
      return "ARRAY";
   case MAP_type_of_ClipVarType:
      return "MAP";
   case MEMO_type_of_ClipVarType:
      return "MEMO";
   case DATETIME_type_of_ClipVarType:
      return "DATETIME";
   }
   return "UNKNOWN";
}
