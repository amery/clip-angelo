Node     *
new_SeqNode(Node * list, Node * using, Node * recover, int seqNo)
{
   NEWVAR(SeqNode, ret);

   init_Node(ret, pass_SeqNode, "seq");
   ret->list = list;
   ret->using = using;
   ret->recover = recover;
   ret->seqNo = seqNo;
	ret->curSeqNo = curFunction->seqNo_of_Function;
	ret->seqLevel = curFunction->seqLevel_of_Function;

   return (Node *) ret;
}
