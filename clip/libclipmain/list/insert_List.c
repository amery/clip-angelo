void
insert_List(List * list, void *item)
{
	if (!list->head_of_List)
   {
		list->head_of_List = list->current_of_List = item;
      NEXT(item) = item;
      PREV(item) = item;
   }
   else
   {
		void *p = NEXT(list->current_of_List);

		NEXT(list->current_of_List) = item;
      PREV(p) = item;
		PREV(item) = list->current_of_List;
      NEXT(item) = p;
		list->current_of_List = item;
   }
}
