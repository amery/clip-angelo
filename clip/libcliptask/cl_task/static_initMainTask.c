static void
initMainTask(Task * task)
{
   task->isMain = 1;
   task->isInited = 1;
   if (task->state == Task_initial)
      addToReady(task);
}
