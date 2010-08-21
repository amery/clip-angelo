int
clip_DEC(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   int rn = 0;

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case NUMERIC_type_of_ClipVarType:
      rn = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
      break;
   default:
      ;
   }

   _clip_retni(ClipMachineMemory, rn);
   return 0;
}
