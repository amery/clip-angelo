CLIP_DLLEXPORT int
_clip_compare_c_item_type(const void *p1, const void *p2)
{
   return ((ContainerItem *) p1)->type_of_ContainerItem - ((ContainerItem *) p2)->type_of_ContainerItem;
}
