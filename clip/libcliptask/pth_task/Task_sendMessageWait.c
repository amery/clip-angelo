TASK_DLLEXPORT int
Task_sendMessageWait(int receiver, TaskMessage * msg)
{
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   return __Task_sendMessage(receiver, msg, 1);
}
