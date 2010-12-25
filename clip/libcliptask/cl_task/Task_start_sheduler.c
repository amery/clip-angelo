TASK_DLLEXPORT int
Task_start_sheduler(void)
{
   int ret = canSwitch;

   canSwitch = 1;
   return ret;
}
