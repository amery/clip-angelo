void
prepend_Node(Node * list, Node * node)
{
   if (!list || !node)
      return;
   prepend_List(&list->list, node);
}
