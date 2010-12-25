typedef struct OneTaskLock
{
   off_t pos;
   int write;
   int fd;
} OneTaskLock;
