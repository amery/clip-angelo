void
init_parser()
{
   fileIncludes = new_Coll(free, NULL);

   if (!clic_outpath)
      clic_outpath = strdup(".");
   init_Coll(&hashNames, free, cmp_long);
   /* malloc_logpath="_memlog"; */
   /* debug2              log-stats, log-non-free, log-perror, log-trans,
      log-bad-pnt, \ check-fence, check-heap, check-lists, check-dblock-fence, \
      error-abort */
   /* malloc_debug( 0x401c1f ); */
   /* malloc_debug(0x405c17); */
}
