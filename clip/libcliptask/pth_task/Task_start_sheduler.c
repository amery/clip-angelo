TASK_DLLEXPORT int
Task_start_sheduler(void)
{
   int ret = canSwitch;

   /* not need under PTH */
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   canSwitch = 1;
   return ret;
}
