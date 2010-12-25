static void
removeFromList(Task * task)
{
   activeCount--;
   switch (task->state)
   {
   case Task_ready:
      removeIt_List(&readyTasks, task);
      break;
   case Task_wait:
      removeIt_List(&waitTasks, task);
      break;
   case Task_msg:
      removeIt_List(&msgTasks, task);
      break;
   case Task_resp:
      removeIt_List(&respTasks, task);
      break;
   case Task_zombie:
      removeIt_List(&zombieTasks, task);
      break;
   case Task_initial:
      break;
   }
}
