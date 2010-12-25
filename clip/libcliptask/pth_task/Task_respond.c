TASK_DLLEXPORT int
Task_respond(TaskMessage * msg)
{
   int ret = 1;

#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (!pth_inited)
      return -1;
/* not need for PTH */
   return ret;
}
