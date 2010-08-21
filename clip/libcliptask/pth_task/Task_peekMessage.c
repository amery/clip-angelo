TASK_DLLEXPORT TaskMessage *
Task_peekMessage(void)
{
   pth_t ptask;

   Task *task;

   pth_msgport_t selfPort = NULL;

   char msgPortName[10];

#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (!pth_inited)
      return NULL;
   ptask = pth_self();
   task = seach_task_in_list(&Tasks, ptask);
   if (task == NULL)
      return NULL;
   snprintf(msgPortName, 10, "%ld", task->id);
   selfPort = pth_msgport_find(msgPortName);
   if (selfPort == NULL)
      return NULL;
   if (pth_msgport_pending(selfPort))
      return Task_getMessage();

   Task_yield();
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return NULL;
}
