TASK_DLLEXPORT long
Task_version(void)
{
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (pth_inited)
      return pth_version();
   return 0;
}
