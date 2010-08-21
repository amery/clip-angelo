static void
sigfunc(int sig)
{
   cleanup(100 + sig);
}
