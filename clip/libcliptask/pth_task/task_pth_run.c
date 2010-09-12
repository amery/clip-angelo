void *
task_pth_run(void *data)
{
   Task *task;

   task = (Task *) data;
   char msgPortName[10];

   pth_msgport_t mp;

   pth_event_t ev;

#ifdef _LDEBUG
   printf("F=%s,L=%d,data_pth_run=%p,%p,name=%s,%p\n", __FILE__, __LINE__, data, task, task->name, task->name);
#endif
   if (task == NULL)
      return NULL;
   append_List(&Tasks, (void *) task);
   local_print_List(&Tasks);

   snprintf(msgPortName, 10, "%ld", task->id);
#ifdef _MDEBUG
   printf("F=%s,L=%d,task->id=%ld,msgPortName=%s\n", __FILE__, __LINE__, task->id, msgPortName);
#endif
   mp = pth_msgport_create(msgPortName);
   ev = pth_event(PTH_EVENT_MSG, mp);

   if (task->run)
      task->run(task->data);

   pth_event_free(ev, PTH_FREE_THIS);
   pth_msgport_destroy(mp);

   if (task->destroy)
      task->destroy(task->data);
   stopcount = 0;
   Task_START();
   removeIt_List(&Tasks, task);
//      local_print_List(&Tasks);
   free(task->name);
   free(task);
#ifdef _LDEBUG
   printf("	task end run=%p\n", task);
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return NULL;
}
