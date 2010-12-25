void
append_Node(Node * list, Node * node)
{
   if (!list || !node)
      return;
   append_List(&list->list, node);
}
