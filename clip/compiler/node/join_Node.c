void
join_Node(Node * list, Node * np)
{
   Node *p, *pp;

   if (!list || !np)
      return;
	for (p = (Node *) np->list.head_of_List; p; p = pp)
   {
		pp = (Node *) p->listel.next_of_ListEl;
      append_List(&list->list, p);
		if (pp == (Node *) np->list.head_of_List)
	 break;
   }

   np->list.head_of_List = np->list.current_of_List = NULL;
}
