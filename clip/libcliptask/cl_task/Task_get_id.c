TASK_DLLEXPORT long
Task_get_id(Task * task)
{
   if (task == NULL)
      return -1;
   return task->id;
}
