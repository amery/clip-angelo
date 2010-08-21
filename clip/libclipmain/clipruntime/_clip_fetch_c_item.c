CLIP_DLLEXPORT void *
_clip_fetch_c_item(ClipMachine * ClipMachineMemory, int key, int type)
{
   ContainerItem citem, *ci = NULL;

   if (ClipMachineMemory->container == NULL)
      return NULL;

   citem.key_of_ContainerItem = key;
   ci = bsearch(&citem, ClipMachineMemory->container->ContainerItem_items_of_Container, ClipMachineMemory->container->len_of_Container, sizeof(ContainerItem), _clip_compare_c_item);
   return (ci && ci->type_of_ContainerItem == type ? ci->item_of_ContainerItem : NULL);
}
