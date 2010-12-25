TASK_DLLEXPORT int
Task_forward(long receiver, TaskMessage * msg)
{
   Task *tp = Task_findTask(receiver);

   if (!tp)
      return Task_respond(msg);

   removeIt_List(&currTask->proclist, msg);

   /*  ����������� ����������� */
   append_List(&tp->recvlist, msg);

   /*  �� ��� �� ����� ������ � */
   if (tp->state == Task_msg)
   {
      removeFromList(tp);
      addToReady(tp);
   }

   return 1;
}
