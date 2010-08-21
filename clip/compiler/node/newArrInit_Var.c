Var *
newArrInit_Var(char *name, Node * node)
{
   Node *p, *p0;

   int isNum = 1;

   ins_name(name);
   if (!node->isExprList)
      return newInit_Var(name, node);

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
      return newArr_Var(name, arr);
   }
   else
      return newInit_Var(name, new_NewArrNode(node));
}
