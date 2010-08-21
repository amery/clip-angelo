TASK_DLLEXPORT void
Task_START()
{
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   --stopcount;
   if (stopcount <= 0)
   {
      if (pth_inited)
      {
	 pth_t ptask;

	 ptask = pth_self();
	 task_pth_start(&Tasks);
      }
      Task_start_sheduler();
      Task_yield();
   }
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
}
