void
join_Node(Node * list, Node * np)
{
   Node     *p, *pp;

   if (!list || !np)
      return;
   for (p = (Node *) np->list.head; p; p = pp)
    {
       pp = (Node *) p->listel.next;
       append_List(&list->list, p);
       if (pp == (Node *) np->list.head)
	  break;
    }

   np->list.head = np->list.current = NULL;
}
