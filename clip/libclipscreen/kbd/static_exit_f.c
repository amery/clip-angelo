static void
exit_f(void)
{
/* restore normal mode */
   printf("\033[R");
   fflush(stdout);
   tcsetattr(0, TCSANOW, &ts0);
}
