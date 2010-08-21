TASK_DLLEXPORT int
Task_start(Task * tp)
{
   if (initStack(tp))
   {
      if (tp != sheduler)
	 addToReady(tp);
      HashTable_insert(hashs, tp, tp->id);
      append_Coll(&allTasks, tp);
      return 1;
   }
   else
      return 0;
}
