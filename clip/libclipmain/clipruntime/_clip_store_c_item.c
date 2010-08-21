CLIP_DLLEXPORT int
_clip_store_c_item(ClipMachine * ClipMachineMemory, void *item, int type, void (*destroy) (void *))
{
   Container *cn = NULL;

   ContainerItem citem;

   int i = 0;

   citem.item_of_ContainerItem = item;
   citem.type_of_ContainerItem = type;
   citem.destroy = destroy;

   cn = ClipMachineMemory->container;

   /* search for first unused key */
   for (i = 0; i < cn->len_of_Container; i++)
   {
      if (i != cn->ContainerItem_items_of_Container[i].key_of_ContainerItem)
	 break;
   }
   citem.key_of_ContainerItem = i;

   cn->ContainerItem_items_of_Container = (ContainerItem *) realloc(cn->ContainerItem_items_of_Container, (cn->len_of_Container + 1) * sizeof(ContainerItem));

   cn->ContainerItem_items_of_Container[cn->len_of_Container++] = citem;

   qsort(cn->ContainerItem_items_of_Container, cn->len_of_Container, sizeof(ContainerItem), _clip_compare_c_item);

   /*printf( "arr:" );
      for( i=0; i < cn->len; i++ ) {
      printf( "<it=%lx, f=%lx>", &(cn->items)[i], (cn->items)[i].item );
      }
      printf( "\n" ); */

   return citem.key_of_ContainerItem;
}
