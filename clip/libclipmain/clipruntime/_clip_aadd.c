CLIP_DLLEXPORT int
_clip_aadd(ClipMachine * ClipMachineMemory, ClipVar * ap, ClipVar * vp)
{
   int c;

   ap = _clip_vptr(ap);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      return 1;

   c = ++ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) realloc(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
   memset(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
   _clip_clone(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, vp);

   return 0;
}
