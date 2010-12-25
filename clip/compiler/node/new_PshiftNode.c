Node *
new_PshiftNode(Node * expr)
{
   NEWVAR(Node, ret);

   init_Node(ret, pass_PshiftNode, "pshift");

   return (Node *) ret;
}
