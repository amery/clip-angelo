static void
usage(void)
{
   fprintf(stderr, "CLIP debugger commandline frontend\n");
   fprintf(stderr, "usage: clip_dbg [-hv] progname|pid\n");
   fprintf(stderr, "\t-h\tthis help\n");
   fprintf(stderr, "\t-v\tverbose output\n");
   exit(0);
}
