static void
addToWait(Task * task)
{
   int n;

   activeCount++;
   if (task->isRead)
   {
      FD_SET_BY(&readFiles, &task->rfileset);
   }
   if (task->isWrite)
   {
      FD_SET_BY(&writeFiles, &task->wfileset);
   }
   if (task->isExcept)
   {
      FD_SET_BY(&exceptFiles, &task->efileset);
   }

   task->state = Task_wait;

   for (n = first_List(&waitTasks); n; n = next_List(&waitTasks))
   {
      Task *tp = (Task *) waitTasks.current;

      if (task->wakeUp <= tp->wakeUp)
      {
	 insertBefore_List(&waitTasks, task);
	 return;
      }
   }

   append_List(&waitTasks, task);
}
