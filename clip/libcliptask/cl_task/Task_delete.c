TASK_DLLEXPORT void
Task_delete(Task * task)
{
   HashTable_remove(hashs, task->id);
   remove_Coll(&allTasks, task);

   if (task->destroy)
      task->destroy(task->data);

   free(task->name);

   if (!task->isMain)
   {
      free(task->stack);
      free(task);
   }
}
