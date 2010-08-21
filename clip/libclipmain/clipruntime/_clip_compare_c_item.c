
CLIP_DLLEXPORT int
_clip_compare_c_item(const void *p1, const void *p2)
{
   return ((ContainerItem *) p1)->key_of_ContainerItem - ((ContainerItem *) p2)->key_of_ContainerItem;
}
