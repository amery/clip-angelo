Node     *
new_LoopExitNode(int isLoop)
{
   NEWVAR(LoopExitNode, ret);

   init_Node(ret, pass_LoopExitNode, "loopexit");
   ret->isLoop = isLoop;

   return (Node *) ret;
}
