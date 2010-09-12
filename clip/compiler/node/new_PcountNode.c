Node *
new_PcountNode()
{
   NEWVAR(Node, ret);

   init_Node(ret, pass_PcountNode, "pcount");

   return (Node *) ret;
}
