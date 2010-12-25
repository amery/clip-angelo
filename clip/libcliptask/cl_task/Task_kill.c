TASK_DLLEXPORT int
Task_kill(long taskid)
{
   Task *tp = Task_findTask(taskid);

   Task *ct = currTask;

   if (!tp)
      return 0;
   if (tp == ct || tp == mainTask || tp == sheduler)
      return 0;
   removeFromList(tp);
   addToReady(tp);

   if (!setjmp(ct->taskEnv))
      resumeTask(tp, 2);

   tp = Task_findTask(taskid);
   if (!tp)
      return 1;

   return 0;
}
