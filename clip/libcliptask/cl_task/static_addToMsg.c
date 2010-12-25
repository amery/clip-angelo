static void
addToMsg(Task * task)
{
   activeCount++;
   prepend_List(&msgTasks, task);
   task->state = Task_msg;
}
