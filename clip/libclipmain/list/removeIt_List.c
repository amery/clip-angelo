void
removeIt_List(List * list, void *item)
{
	if (list->current_of_List != item)
   {
		void *curr = list->current_of_List;

		list->current_of_List = item;
      remove_List(list);
		list->current_of_List = curr;
   }
   else
      remove_List(list);
}
