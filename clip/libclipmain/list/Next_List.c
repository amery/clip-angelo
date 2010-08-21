int
Next_List(List * list)
{
	if (list->head_of_List)
   {
		list->current_of_List = NEXT(list->current_of_List);
      return 1;
   }
   return 0;
}
