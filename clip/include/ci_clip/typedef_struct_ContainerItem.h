typedef struct ContainerItem
{
   void *item_of_ContainerItem;
   int key_of_ContainerItem;
   int type_of_ContainerItem;
   void (*destroy) (void *);
}
ContainerItem;
