TASK_DLLEXPORT void
Task_STOP()
{
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (!stopcount)
    {
       if (pth_inited)
	{
	   pth_t     ptask;

	   ptask = pth_self();
	   task_pth_stop(&Tasks, ptask);
	}
       Task_stop_sheduler();
    }

   ++stopcount;
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
}
