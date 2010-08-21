static ClipVarEl *
get_map_el(ClipMapVar * a, long no)
{
   int ind;

   if (search_map(a->ClipVarEl_items_of_ClipMapVar, a->count_of_ClipMapVar, no, &ind))
      return a->ClipVarEl_items_of_ClipMapVar + ind;
   else
      return 0;
}
