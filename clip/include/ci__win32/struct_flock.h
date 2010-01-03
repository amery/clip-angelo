struct flock
{
   short int l_type;
   short int l_whence;
#ifndef __USE_FILE_OFFSET64
   long      l_start;
   long      l_len;
#else
   long      l_start;
   long      l_len;
#endif
};
