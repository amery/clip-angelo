TASK_DLLEXPORT int
Task_yield(void)
{
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif

   if (!canSwitch)
      return 0;
   if (pth_inited)
      pth_yield(NULL);
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return 0;
}
