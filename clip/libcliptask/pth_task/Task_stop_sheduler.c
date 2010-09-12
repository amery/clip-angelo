TASK_DLLEXPORT int
Task_stop_sheduler(void)
{
   int ret = canSwitch;

#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   canSwitch = 0;
   return ret;
}
