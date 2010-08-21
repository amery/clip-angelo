TASK_DLLEXPORT long
Task_sleep(long msec)
{
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (!pth_inited)
      return 0;
   if (!canSwitch)
   {
      struct timeval tv;

      calcTv(&tv, msec);
      select(0, 0, 0, 0, &tv);
      return 1;
   }
   pth_usleep(msec * 1000);
   return 0;
}
