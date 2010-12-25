TASK_DLLEXPORT const char *
Task_get_name(Task * task)
{
   if (task == NULL)
      return NULL;
   return task->name;
}
