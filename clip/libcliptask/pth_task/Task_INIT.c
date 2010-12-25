TASK_DLLEXPORT void
Task_INIT(void)
{
   if (pth_inited != 0)
      return;
   pth_init();
   pth_inited = 1;
}
