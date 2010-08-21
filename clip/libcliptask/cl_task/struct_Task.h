struct Task
{

   ListEl listel;

   List recvlist;		/*  ������������� */
   List proclist;		/*  ������� ����� */

   int isMain:1;
   int isInited:1;		/*  �������� ���� ������ */
   int isTimed:1;		/*  �� ���� */
   int isRead:1;		/*  �� ��� � ��� */
   int isWrite:1;		/*  �� ��� ��� */
   int isExcept:1;		/*  �� sigexcept */

   char *name;			/*  ����� */
   long id;			/*  ���������� (pid) */
   long wakeUp;			/*  �� ������� */

   Task *parent;		/*  ���� ���, ����������spawn */
   void *result;		/*  reference to ���������� run() */

   TaskState state;		/*  �������� */

   long stacklen;		/*  length of task's stack */

   jmp_buf taskEnv;		/* environment for context switching */
   char *stack;			/* this task's stack space */
   fd_set rfileset;		/* read-wait fileset */
   fd_set wfileset;		/* write-wait fileset */
   fd_set efileset;		/* except fileset */

   void *(*run) (void *data);
   void (*destroy) (void *data);
   void *data;
   void *ref;			/*other reference to any data */
};
