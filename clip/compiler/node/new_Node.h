Node     *
new_Node()
{
   NEWVAR(Node, ret);
   init_Node(ret, pass_Node, "node");
   return ret;
}
