TASK_DLLEXPORT int
Task_respond(TaskMessage * msg)
{
   int ret = 1;

   if (msg->needResp)
   {
      Task *sender = Task_findTask(msg->sender);

      if (!sender)
	 ret = 0;
      else if (sender->state != Task_resp)
	 ret = 0;
      else
      {
	 removeFromList(sender);
	 addToReady(sender);
      }
   }

   removeIt_List(&currTask->proclist, msg);

   if (msg->needDel)
      TaskMessage_delete(msg);

   return ret;
}
