void
insertBefore_List(List * list, void *item)
{
	if (!list->head_of_List)
   {
		list->head_of_List = list->current_of_List = item;
      NEXT(item) = item;
      PREV(item) = item;
   }
   else
   {
		void *p = PREV(list->current_of_List);

		PREV(list->current_of_List) = item;
      NEXT(p) = item;
		NEXT(item) = list->current_of_List;
      PREV(item) = p;

		if (list->current_of_List == list->head_of_List)
			list->head_of_List = item;
		list->current_of_List = item;
   }
}
