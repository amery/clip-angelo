TASK_DLLEXPORT int
Task_wakeup(Task * task)
{
   if (task->state == Task_wait)
   {
      /*  ������� ��� */
      removeFromList(task);
      task->isTimed = 1;
      addToReady(task);
      if (task->isRead)
      {
	 FD_CLR_BY(&readFiles, &task->rfileset);
	 task->isRead = 0;
      }
      if (task->isWrite)
      {
	 FD_CLR_BY(&writeFiles, &task->wfileset);
	 task->isWrite = 0;
      }
      if (task->isExcept)
      {
	 FD_CLR_BY(&exceptFiles, &task->efileset);
	 task->isExcept = 0;
      }

      return 1;
   }

   return 0;
}
