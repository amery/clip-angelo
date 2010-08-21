static void
deathMatch(void)
{
   fprintf(stderr, "abnormal Task sheduler state, abort\n");
   fflush(stderr);
   abort();
}
