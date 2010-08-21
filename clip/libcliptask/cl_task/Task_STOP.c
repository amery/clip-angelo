TASK_DLLEXPORT void
Task_STOP()
{
   if (!stopcount)
   {
      if (Task_get_currTask())
	 Task_stop_sheduler();
   }

   ++stopcount;
}
