int
type_weight(int type)
{
   int r = 0;

   switch (type)
   {
   case ARRAY_type_of_ClipVarType:
      r = 0;
      break;
   case CCODE_type_of_ClipVarType:
   case PCODE_type_of_ClipVarType:
      r = 1;
      break;
   case CHARACTER_type_of_ClipVarType:
      r = 2;
      break;
   case LOGICAL_type_of_ClipVarType:
      r = 3;
      break;
   case DATE_type_of_ClipVarType:
      r = 4;
      break;
   case DATETIME_type_of_ClipVarType:
      r = 5;
   case NUMERIC_type_of_ClipVarType:
      r = 6;
      break;
   case UNDEF_type_of_ClipVarType:
   default:
      r = 7;
      break;
   }
   return r;
}
