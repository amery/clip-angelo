TASK_DLLEXPORT TaskMessage *
TaskMessage_new(long id, void *data, void (*destroy) (void *data))
{
   TaskMessage *msg = NEW(TaskMessage);

   msg->id = id;
   msg->data = data;
   msg->destroy = destroy;

   return msg;
}
