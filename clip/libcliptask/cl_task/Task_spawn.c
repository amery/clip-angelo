TASK_DLLEXPORT void *
Task_spawn(Task * task, Task * chield)
{
   chield->parent = task;
   removeIt_List(&readyTasks, task);
   Task_start(chield);
   Task_yield();
   return task->result;
}
