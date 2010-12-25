typedef enum
{
   Task_initial,		/*  ���� � � ����� */
   Task_ready,			/*  ��� ������ */
   Task_wait,			/*  �� ����� ��������� */
   Task_msg,			/*  �� ����� */
   Task_resp,			/*  �� ��� */
   Task_zombie			/*  �����, � ��� ����� */
}
TaskState;
