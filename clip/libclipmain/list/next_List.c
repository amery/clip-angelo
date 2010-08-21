int
next_List(List * list)
{
	if (list->head_of_List)
   {
		void *item = NEXT(list->current_of_List);

		if (item != list->head_of_List)
      {
			list->current_of_List = item;
	 return 1;
      }
   }
   return 0;
}
