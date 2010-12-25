static void
resumeTask(Task * task, int code)
{
   if (task->state == Task_zombie)
      return;

   if (!task->isInited)
      initTask(task);
   else
   {
      longjmp(task->taskEnv, code);
   }
}
