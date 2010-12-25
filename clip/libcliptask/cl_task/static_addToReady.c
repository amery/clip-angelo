static void
addToReady(Task * task)
{
   activeCount++;
   prepend_List(&readyTasks, task);
   task->state = Task_ready;
}
