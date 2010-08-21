int
first_List(List * list)
{
	if (list->head_of_List)
   {
		list->current_of_List = list->head_of_List;
      return 1;
   }
   return 0;
}
