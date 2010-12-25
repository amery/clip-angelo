void
prepend_List(List * list, void *item)
{
	if (!list->head_of_List)
   {
		list->head_of_List = list->current_of_List = item;
      PREV(item) = item;
      NEXT(item) = item;
   }
   else
   {
		void *tail = PREV(list->head_of_List);

		PREV(list->head_of_List) = item;
      NEXT(tail) = item;
		NEXT(item) = list->head_of_List;
      PREV(item) = tail;
		list->head_of_List = item;
   }
}
