static void
errexit(const char *msg)
{
   char msg2[] = " terminal=";

   write(2, msg, strlen(msg));
   write(2, msg2, strlen(msg2));
   write(2, tname, strlen(tname));
   write(2, "\n", 1);
   exit(3);
}
