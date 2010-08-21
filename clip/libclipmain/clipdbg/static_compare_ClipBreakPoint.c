static int
compare_ClipBreakPoint(void *p1, void *p2)
{
   ClipBreakPoint *bp1 = (ClipBreakPoint *) p1;

   ClipBreakPoint *bp2 = (ClipBreakPoint *) p2;

   int l;

   if (bp1->filename_of_ClipBreakPoint && bp2->filename_of_ClipBreakPoint && bp1->filename_of_ClipBreakPoint[0] && bp2->filename_of_ClipBreakPoint[0])
   {
      l = strcasecmp(nullstr(bp1->filename_of_ClipBreakPoint), nullstr(bp2->filename_of_ClipBreakPoint));
      if (l != 0)
	 return l;
   }

   if (bp1->line_of_ClipBreakPoint < bp2->line_of_ClipBreakPoint)
      return -1;
   else if (bp1->line_of_ClipBreakPoint > bp2->line_of_ClipBreakPoint)
      return 1;
   return 0;

   /*l = strcasecmp(nullstr(bp1->procname_of_ClipBreakPoint), nullstr(bp2->procname_of_ClipBreakPoint));
      return l; */
}
