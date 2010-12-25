void
append_List(List * list, void *item)
{
	if (!list->head_of_List)
   {
		list->head_of_List = list->current_of_List = item;
      NEXT(item) = item;
      PREV(item) = item;
   }
   else
   {
		void *tail = PREV(list->head_of_List);

      NEXT(tail) = item;
		PREV(list->head_of_List) = item;
      PREV(item) = tail;
		NEXT(item) = list->head_of_List;
   }
}
