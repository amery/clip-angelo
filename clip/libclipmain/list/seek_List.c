int
seek_List(List * list, void *item)
{
   void *p;

	if (!list->head_of_List)
      return 0;

	for (p = list->head_of_List;;)
   {
      if (p == item)
      {
			list->current_of_List = p;
	 return 1;
      }
      p = NEXT(p);
		if (p == list->head_of_List)
	 return 0;
   }
   return 0;
}
