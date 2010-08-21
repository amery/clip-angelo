static char *
macro_name(void)
{
   static int no = 0;

   char name[32];

   snprintf(name, sizeof(name), "macro_%d", no++);
   return strdup(name);
}
