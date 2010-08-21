typedef struct TaskLock
{
   OneTaskLock *locks;
   int count;
   int flockfd;
} TaskLock;
