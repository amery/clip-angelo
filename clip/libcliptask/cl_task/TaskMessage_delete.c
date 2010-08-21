TASK_DLLEXPORT void
TaskMessage_delete(TaskMessage * msg)
{
   if (!msg)
      return;

   if (msg->destroy)
      msg->destroy(msg->data);
}
