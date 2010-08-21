static void
new_array(ClipVar * vp, int n, long *dims)
{
   ClipVar *ap;

   int i, size;

   CLEAR_CLIPVAR(vp);
   if (!n)
   {
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = 0;
      vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      return;
   }

   --n;
   size = GETLONG(dims);
   ++dims;

   ap = NEW(ClipVar);
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
   ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) malloc(sizeof(ClipVar) * size);
   ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = size;
   for (i = 0; i < size; i++)
      new_array(&ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i], n, dims);
}
