TASK_DLLEXPORT Task *
Task_new(const char *name, long stacksize, void *data, void *(*run) (void *data), void (*destroy) (void *data))
{
   Task     *task = NEW(Task);

   pth_t     ptask;

   pth_attr_t attr;

#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (pth_inited == 0)
    {
       if (pth_init() == 0)
	  return NULL;
       pth_inited = 1;
    }

   if (stacksize < TASK_STACK_MIN)
      stacksize = TASK_STACK_MIN;

   ++seqNo;
   task->id = seqNo;
   task->stacklen = stacksize;
   task->name = strdup(name);
   task->data = data;
   task->run = run;
   task->destroy = destroy;

   attr = pth_attr_new();
   pth_attr_set(attr, PTH_ATTR_NAME, name);
   pth_attr_set(attr, PTH_ATTR_STACK_SIZE, (unsigned int) stacksize);
   ptask = pth_spawn(attr, task_pth_run, task);
   pth_attr_destroy(attr);
   if (ptask == NULL)
    {
       free(task);
       return NULL;
    }
   task->ref = (void *) ptask;

#ifdef _LDEBUG
   printf("	open ptask=%p,task=%p,name=%s,%p\n", (void *) ptask, (void *) task, task->name, task->name);
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return task;
}
