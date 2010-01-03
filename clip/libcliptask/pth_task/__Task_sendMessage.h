int
__Task_sendMessage(long receiver, TaskMessage * msg, int wait)
{
   pth_t     ptask;

   Task     *task;

   pth_event_t ev = NULL;

   pth_msgport_t selfPort = NULL;

   pth_msgport_t recvPort = NULL;

   char      msgPortName[10];

   int       ret;

#ifdef _MDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (!pth_inited)
      return 0;
   snprintf(msgPortName, 10, "%ld", receiver);
   recvPort = pth_msgport_find(msgPortName);
#ifdef _MDEBUG
   printf("F=%s,L=%d,msgPortName=%s,recvPort=%p\n", __FILE__, __LINE__, msgPortName, recvPort);
#endif
   if (recvPort == NULL)
      return 0;
   ptask = pth_self();
   task = seach_task_in_list(&Tasks, ptask);
//      printf("F=%s,L=%d,%p,%p\n",__FILE__,__LINE__,ptask,task);
//      local_print_List(&Tasks);
   msg->receiver = receiver;
   if (task != NULL)
      msg->sender = task->id;
   ret = pth_msgport_put(recvPort, (pth_message_t *) msg);
   if (!wait)
      return ret;
   if (task == NULL)
      return ret;
   snprintf(msgPortName, 10, "%ld", task->id);
   selfPort = pth_msgport_find(msgPortName);
   ev = pth_event(PTH_EVENT_MSG, selfPort);
   pth_wait(ev);

   if (msg->destroy)
      msg->destroy((void *) msg->data);
#ifdef _MDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return ret;
}
