TASK_DLLEXPORT int
Task_stop_sheduler(void)
{
   int ret = canSwitch;

   canSwitch = 0;
   return ret;
}
