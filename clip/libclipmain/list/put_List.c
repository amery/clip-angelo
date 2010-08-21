void
put_List(List * list, void *item)
{
   void *p, *n;

	if (!list->head_of_List)
      return;

	p = PREV(list->current_of_List);
	n = NEXT(list->current_of_List);
	if (p == list->current_of_List)	/* last member */
   {
		list->head_of_List = list->current_of_List = item;
      NEXT(item) = item;
      PREV(item) = item;
   }
   else
   {
      NEXT(p) = item;
      PREV(n) = item;
      NEXT(item) = n;
      PREV(item) = p;
		if (list->head_of_List == list->current_of_List)
			list->head_of_List = item;
		list->current_of_List = item;
   }
}
