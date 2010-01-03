static Task *
seach_task_in_list(List * list, pth_t ref)
{
   int       r;

   Task     *ret = NULL;

   Task     *tp;

   pth_t     ptask;

   for (r = first_List(list); r; r = next_List(list))
    {
       tp = (Task *) list->current;
       ptask = tp->ref;
       if (ptask == ref)
	{
	   ret = (Task *) tp;
	   break;
	}
    }
   return ret;
}
