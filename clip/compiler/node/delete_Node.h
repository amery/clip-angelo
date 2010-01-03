void
delete_Node(Node * node)
{
   if (!node)
      return;
   node->pass(node, Destroy, 0, node);
   free(node);
}
