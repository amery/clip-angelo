TASK_DLLEXPORT long
Task_ID()
{
   Task *tp = Task_get_currTask();

   if (!tp)
      return -1;
   else
      return Task_get_id(tp);
}
