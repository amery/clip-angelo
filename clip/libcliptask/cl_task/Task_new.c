TASK_DLLEXPORT Task *
Task_new(const char *name, long stacksize, void *data, void *(*run) (void *data), void (*destroy) (void *data))
{
   Task *task = NEW(Task);

   int psize;

   Task_init();

   ++seqNo;
   task->id = seqNo;
   task->name = strdup(name);
   task->data = data;
   task->run = run;
   task->destroy = destroy;

   if (stacksize < TASK_STACK_MIN)
      stacksize = TASK_STACK_MIN;

   psize = getpagesize();
   if (stacksize >= psize)
   {
      int n = stacksize / psize;

      int rest = stacksize % psize;

      stacksize = n * psize + (rest ? psize : 0);
   }

   task->stacklen = stacksize;
   task->state = Task_initial;

   return task;
}
