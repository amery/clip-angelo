TASK_DLLEXPORT void
Task_init(void)
{
   Task *tp;

   if (task_inited)
      return;
   task_inited = 1;

   canSwitch = 0;
   seqNo = 0;
   activeCount = 0;

   FD_ZERO(&readFiles);
   FD_ZERO(&writeFiles);
   FD_ZERO(&exceptFiles);

   tp = Task_new("sheduler", 1024, 0, 0, 0);
   sheduler = tp;
   initStack(sheduler);

   hashs = new_HashTable();
   HashTable_insert(hashs, tp, tp->id);

   append_Coll(&allTasks, tp);

   currTask = sheduler;
   if (!setjmp(mainEnv))
   {
      initTask(sheduler);
   }

   tp = Task_new("main", 0, 0, 0, 0);
   mainTask = tp;
   initMainTask(mainTask);
   HashTable_insert(hashs, tp, tp->id);
   append_Coll(&allTasks, tp);

   currTask = mainTask;
}
