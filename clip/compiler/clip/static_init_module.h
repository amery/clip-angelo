static void
init_module(void)
{
   char      path[256], *s;

   if (!getcwd(path, sizeof(path)))
    {
       CLIP_MODULE = "root";
       return;
    }

   s = strrchr(path, '/');
   if (s)
      s++;
   else
      s = path;

   if (!strcmp(s, "/"))
      s = "root";

   CLIP_MODULE = strdup(s);
}
