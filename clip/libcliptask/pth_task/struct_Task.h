struct Task
{
   ListEl    listel;

   List      recvlist;		/*  ������������� */
   List      proclist;		/*  ������� ����� */

   char     *name;		/*  ����� */
   long      id;		/*  ���������� (pid) */
   long      stacklen;		/*  length of task's stack */

   void     *(*run) (void *data);
   void      (*destroy) (void *data);
   void     *data;
   void     *ref;		/*other reference to any data */

};
