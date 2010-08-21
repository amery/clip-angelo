static int
initStack(Task * task)
{
   if (!task->isMain && task->state == Task_initial)
   {
      if (!task->stack)
      {
	 task->stack = (char *) malloc(task->stacklen);
	 return 1;
      }
   }
   return 0;
}
