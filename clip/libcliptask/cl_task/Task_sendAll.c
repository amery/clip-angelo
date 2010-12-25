TASK_DLLEXPORT int
Task_sendAll(TaskMessage * msg)
{
   Task *ct = currTask;

   Task *tp;

   int sent = 0;

   msg->needResp = 1;
   msg->needDel = 0;
   msg->sender = ct->id;

   while (!empty_List(&msgTasks))
   {
      first_List(&msgTasks);
      tp = (Task *) msgTasks.current;
      msg->receiver = tp->id;

      /*  ����������� ����������� */
      append_List(&tp->recvlist, msg);
      removeFromList(tp);
      addToReady(tp);

      /*  �� ��� */
      removeFromList(ct);
      addToResp(ct);

      Task_yield();
      sent++;
   }

   TaskMessage_delete(msg);

   return sent;
}
