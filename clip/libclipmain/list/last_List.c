int
last_List(List * list)
{
	if (list->head_of_List)
   {
		list->current_of_List = PREV(list->head_of_List);
      return 1;
   }
   return 0;
}
