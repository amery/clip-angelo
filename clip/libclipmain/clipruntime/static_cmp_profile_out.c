static int
cmp_profile_out(void *p1, void *p2)
{
   ProfileBucket *bp1 = (ProfileBucket *) p1;

   ProfileBucket *bp2 = (ProfileBucket *) p2;

   if (bp1->timeval_self_of_ProfileBucket.tv_sec > bp2->timeval_self_of_ProfileBucket.tv_sec)
      return -1;
   else if (bp1->timeval_self_of_ProfileBucket.tv_sec < bp2->timeval_self_of_ProfileBucket.tv_sec)
      return 1;
   else if (bp1->timeval_self_of_ProfileBucket.tv_usec > bp2->timeval_self_of_ProfileBucket.tv_usec)
      return -1;
   else if (bp1->timeval_self_of_ProfileBucket.tv_usec < bp2->timeval_self_of_ProfileBucket.tv_usec)
      return 1;
   else
      return 0;
#if 0
   {
      int r;

      r = strcasecmp(bp1->procname, bp2->procname_of_ProfileBucket);
      if (r)
	 return r;
      r = strcasecmp(bp1->filename, bp2->filename_of_ProfileBucket);
      return r;
   }
#endif
}
