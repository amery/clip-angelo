Var *
mnewArrInit_Var(Node * macro, Node * node)
{
   Node *p, *p0;

   int isNum = 1;

   if (!node->isExprList)
      return mnewInit_Var(macro, node);

	for (p0 = p = (Node *) node->list.head_of_List; p;)
   {
      if (!p->isNum)
      {
	 isNum = 0;
	 break;
      }

      p = (Node *) p->listel.next_of_ListEl;
      if (p == p0)
	 break;
   }

   if (isNum)
   {
      Coll *arr = new_Coll(free, 0);

		for (p0 = p = (Node *) node->list.head_of_List; p;)
      {
	 append_Coll(arr, ((ConstNode *) p)->val);
	 p = (Node *) p->listel.next_of_ListEl;
	 if (p == p0)
	    break;
      }
      return mnewArr_Var(macro, arr);
   }
   else
      return mnewInit_Var(macro, new_NewArrNode(node));
}
