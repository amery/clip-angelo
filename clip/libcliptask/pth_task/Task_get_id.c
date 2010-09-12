TASK_DLLEXPORT long
Task_get_id(Task * task)
{
#ifdef _LDEBUG
   printf("F=%s,L=%d,name=%s,%p\n", __FILE__, __LINE__, task->name, task->name);
#endif
   if (!pth_inited)
      return -1;
   if (task == NULL)
      return -1;
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return task->id;
}
