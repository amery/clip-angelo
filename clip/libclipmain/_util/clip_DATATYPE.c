int
clip_DATATYPE(ClipMachine * ClipMachineMemory)
{
   int rc = 0;

   int t = _clip_parinfo(ClipMachineMemory, 1);

   switch (t)
   {
   case CHARACTER_type_of_ClipVarType:
      rc = 1;
      break;
   case NUMERIC_type_of_ClipVarType:
      rc = 2;
      break;
   case LOGICAL_type_of_ClipVarType:
      rc = 4;
      break;
   case DATE_type_of_ClipVarType:
      rc = 8;
      break;
   case MEMO_type_of_ClipVarType:
      rc = 64;
      break;
   case OBJECT_type_of_ClipVarType:
      rc = 8192;
      break;
   case PCODE_type_of_ClipVarType:
   case CCODE_type_of_ClipVarType:
      rc = 4096;
      break;
   case ARRAY_type_of_ClipVarType:
      rc = 512;
      break;
   case MAP_type_of_ClipVarType:
      rc = 1024;
      break;
   }
   _clip_retni(ClipMachineMemory, rc);
   return 0;
}
