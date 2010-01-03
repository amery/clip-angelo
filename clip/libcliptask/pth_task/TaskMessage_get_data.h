TASK_DLLEXPORT void *
TaskMessage_get_data(TaskMessage * msg)
{
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (msg == NULL)
      return NULL;
   if (!pth_inited)
      return NULL;
   return msg->data;
}
