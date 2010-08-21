CLIP_DLLEXPORT void *
_clip_fetch_c_item_type(ClipMachine * ClipMachineMemory, int type, int *key)
{
   ContainerItem citem, *ci = NULL;

   if (ClipMachineMemory->container == NULL)
      return NULL;

   if (key)
      *key = -1;
   citem.type_of_ContainerItem = type;
   ci = bsearch(&citem, ClipMachineMemory->container->ContainerItem_items_of_Container, ClipMachineMemory->container->len_of_Container, sizeof(ContainerItem), _clip_compare_c_item_type);
   if (ci && key)
      *key = ci->key_of_ContainerItem;
   return (ci ? ci->item_of_ContainerItem : NULL);
}
