static void
task_pth_start(List * list)
{
   int       r;

   Task     *tp;

   pth_t     ptask;

#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   for (r = first_List(list); r; r = next_List(list))
    {
       tp = (Task *) list->current;
       ptask = tp->ref;
       pth_resume(ptask);
    }
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return;
}
