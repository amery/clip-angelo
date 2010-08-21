static void
suicide(Task * task)
{
   if (task->state == Task_zombie)
      return;

   removeFromList(task);

   if (task->parent)
   {
      task->parent->result = task->result;
      prepend_List(&readyTasks, task->parent);
      task->parent = 0;
   }

   /*  main task never must go into zombie state */
   if (!task->isMain)
      addToZombie(task);
   /*  jmp to sheduler */
   longjmp(shedEnv, 1);
}
