static void
sigpipe(int sig)
{
   fprintf(stderr, "\ndebugged program '%s' died.\n", progname);
   cleanup(100 + sig);
}
