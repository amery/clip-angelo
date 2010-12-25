/* create from stack */
CLIP_DLLEXPORT void
_clip_sarray(ClipMachine * ClipMachineMemory, int n)
{
   ClipVar arr, *ap, *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   int i;

   ap = NEW(ClipVar);
   CLEAR_CLIPVAR(&arr);
   arr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   arr.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags /*F_MREF_ClipFlags */ ;
   arr.ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
   ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) malloc(sizeof(ClipVar) * n);
   ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = n;
   for (i = 0; i < n; i++)
   {
      ClipVar *vpp = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

      *vpp = sp[-n + i];
      vpp->ClipType_t_of_ClipVar.field_of_ClipType = 0;
   }
   sp[-n] = arr;
   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame -= n - 1;
}
