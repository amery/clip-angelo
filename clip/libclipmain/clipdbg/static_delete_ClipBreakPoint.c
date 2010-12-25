static void
delete_ClipBreakPoint(void *p)
{
   ClipBreakPoint *bp = (ClipBreakPoint *) p;

   if (!bp)
      return;
   free(bp->filename_of_ClipBreakPoint);
   free(bp->procname_of_ClipBreakPoint);
   free(bp);
}
