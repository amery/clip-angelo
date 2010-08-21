static void
addToResp(Task * task)
{
   activeCount++;
   prepend_List(&respTasks, task);
   task->state = Task_resp;
}
