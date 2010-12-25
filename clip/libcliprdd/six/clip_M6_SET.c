int
clip_M6_SET(ClipMachine * ClipMachineMemory)
{
   int opt = _clip_parni(ClipMachineMemory, 1);

   int f = _clip_parl(ClipMachineMemory, 2);

   int r = 0;

   ClipMachineMemory->m6_error = 0;
   switch (opt)
   {
   case _SET_TYPECHECK:
      r = 0;
      break;
   case _SET_OPTIMIZE:
   case 2:
      r = ClipMachineMemory->flags1 & OPTIMIZE_FLAG;
      if (_clip_parinfo(ClipMachineMemory, 2) == LOGICAL_type_of_ClipVarType)
      {
	 if (f)
	    ClipMachineMemory->flags1 |= OPTIMIZE_FLAG;
	 else
	    ClipMachineMemory->flags1 &= ~OPTIMIZE_FLAG;
      }
      break;
   case _SET_RECHECK:
      r = 0;
      break;
   }
   _clip_retl(ClipMachineMemory, r);
   return 0;
}
