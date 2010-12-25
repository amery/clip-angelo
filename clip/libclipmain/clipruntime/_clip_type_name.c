CLIP_DLLEXPORT char *
_clip_type_name(ClipVar * vp)
{
   char *rc = "U";

   if (vp)
   {
      switch (_clip_type(vp))
      {
      case NUMERIC_type_of_ClipVarType:
	 rc = "N";
	 break;
      case CHARACTER_type_of_ClipVarType:
	 if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    rc = "M";
	 else
	    rc = "C";
	 break;
      case LOGICAL_type_of_ClipVarType:
	 rc = "L";
	 break;
      case DATE_type_of_ClipVarType:
	 rc = "D";
	 break;
      case DATETIME_type_of_ClipVarType:
	 rc = "T";
	 break;
      case OBJECT_type_of_ClipVarType:
	 rc = "O";
	 break;
      case PCODE_type_of_ClipVarType:
      case CCODE_type_of_ClipVarType:
	 rc = "B";
	 break;
      case ARRAY_type_of_ClipVarType:
	 rc = "A";
	 break;
      case MAP_type_of_ClipVarType:
	 rc = "O";
	 break;
      }
   }
   return rc;
}
