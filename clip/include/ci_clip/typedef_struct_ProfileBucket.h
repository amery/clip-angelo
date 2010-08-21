typedef struct ProfileBucket
{
   long hash_of_ProfileBucket;
   char *procname_of_ProfileBucket;
   char *filename_of_ProfileBucket;
   struct timeval timeval_self_of_ProfileBucket;
   struct timeval timeval_start_of_ProfileBucket;
   unsigned long callno_of_ProfileBucket;
   int started_of_ProfileBucket;
#if 0
   Coll callby_of_ProfileBucket;
#endif
}
ProfileBucket;
