static void
task_pth_stop(List * list, pth_t _ptask)
{
   int r;

   Task *tp;

   pth_t ptask;

#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   for (r = first_List(list); r; r = next_List(list))
   {
		tp = (Task *) list->current_of_List;
      ptask = tp->ref;
      if (_ptask == ptask)
	 continue;
      pth_suspend(ptask);
   }
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return;
}
