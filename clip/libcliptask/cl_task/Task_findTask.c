TASK_DLLEXPORT Task *
Task_findTask(long taskid)
{
   Task_init();
   return HashTable_fetch(hashs, taskid);
}
