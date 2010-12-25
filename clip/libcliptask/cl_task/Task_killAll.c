TASK_DLLEXPORT int
Task_killAll(void)
{
   Task *tp, *ct;

   int r;

   if (currTask != mainTask)
      return 0;

   ct = currTask;
   if (!ct)
      return 0;

   /*  first, move all tasks into `ready` state */

   while (!empty_List(&waitTasks))
   {
      first_List(&waitTasks);
      tp = (Task *) waitTasks.current;
      removeIt_List(&waitTasks, tp);
      prepend_List(&readyTasks, tp);
      tp->state = Task_ready;
   }

   while (!empty_List(&msgTasks))
   {
      first_List(&msgTasks);
      tp = (Task *) msgTasks.current;
      removeIt_List(&msgTasks, tp);
      prepend_List(&readyTasks, tp);
      tp->state = Task_ready;
   }

   while (!empty_List(&respTasks))
   {
      first_List(&respTasks);
      tp = (Task *) respTasks.current;
      removeIt_List(&respTasks, tp);
      prepend_List(&readyTasks, tp);
      tp->state = Task_ready;
   }

   /*  spawned parents will be activated incrementally by them's chields */

   /*  now, activate all ready tasks with TaskKillException generation */

   setjmp(ct->taskEnv);

   for (r = first_List(&readyTasks); r; r = next_List(&readyTasks))
   {
      tp = (Task *) readyTasks.current;
      if (tp == mainTask)
	 continue;
      resumeTask(tp, 2);
   }

   return 1;
}
