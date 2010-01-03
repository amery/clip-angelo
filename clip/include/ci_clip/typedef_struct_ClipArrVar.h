typedef struct ClipArrVar
{
   ClipType  ClipType_type_of_ClipArrVar;
   union ClipVar *ClipVar_items_of_ClipArrVar;
   unsigned long count_of_ClipArrVar;
   unsigned long current_of_ClipArrVar;
}
ClipArrVar;
