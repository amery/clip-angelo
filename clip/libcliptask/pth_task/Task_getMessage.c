TASK_DLLEXPORT TaskMessage *
Task_getMessage(void)
{
   pth_t ptask;

   Task *task;

   pth_msgport_t selfPort = NULL;

   pth_event_t ev;

   char msgPortName[10];

   TaskMessage *msg = NULL;

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
   ev = pth_event(PTH_EVENT_MSG, selfPort);
   if (pth_wait(ev) != 1)
      return NULL;
   msg = (TaskMessage *) pth_msgport_get(selfPort);

#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return msg;
}
