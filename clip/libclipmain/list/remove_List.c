void
remove_List(List * list)
{
   void *p, *n;

	if (!list->head_of_List)
      return;
	p = PREV(list->current_of_List);
	n = NEXT(list->current_of_List);

	if (p == list->current_of_List)	/* last member */
		list->head_of_List = list->current_of_List = 0;
   else
   {
      NEXT(p) = n;
      PREV(n) = p;
		if (list->head_of_List == list->current_of_List)
			list->head_of_List = list->current_of_List = n;
      else
			list->current_of_List = n;
   }
}
