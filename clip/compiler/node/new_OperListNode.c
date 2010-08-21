Node *
new_OperListNode()
{
   NEWVAR(Node, ret);
   init_Node(ret, pass_OperListNode, "operlist");
   ret->isExec = 0;
   return ret;
}
