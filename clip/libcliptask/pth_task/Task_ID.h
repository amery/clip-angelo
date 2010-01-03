TASK_DLLEXPORT long
Task_ID()
{
   long      id;

   pth_t     ptask;

   Task     *task;

#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (!pth_inited)
      return -1;
   ptask = pth_self();
   task = seach_task_in_list(&Tasks, ptask);
   if (task == NULL)
      return -1L;
   id = task->id;
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return id;
}
