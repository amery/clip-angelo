CLIP_DLLEXPORT int
_clip_destroy_c_item(ClipMachine * ClipMachineMemory, int key, int type)
{
   int len;

   Container *cn = ClipMachineMemory->container;

   ContainerItem citem, *ci = NULL;

   if (cn == NULL)
      return 0;
   citem.key_of_ContainerItem = key;
   ci = bsearch(&citem, cn->ContainerItem_items_of_Container, cn->len_of_Container, sizeof(ContainerItem), _clip_compare_c_item);
   if (ci == NULL || ci->type_of_ContainerItem != type)
      return 0;

   if (ci->destroy)
      ci->destroy(ci->item_of_ContainerItem);

   len = cn->len_of_Container * sizeof(ContainerItem) - (ci - cn->ContainerItem_items_of_Container + 1) * sizeof(ContainerItem);

   /*printf( "_destroy_: items=%lx, item=%lx, len=%d\n", cn->items, ci, len ); */

   if (len > 0)
      memmove(ci, ci + 1, len);

   cn->len_of_Container--;

   return 1;
}
