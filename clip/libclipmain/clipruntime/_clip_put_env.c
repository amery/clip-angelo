CLIP_DLLEXPORT int
_clip_put_env(char *name, char *val)
{
   int l1 = strlen(name);

   int l2 = strlen(val);

   int r;

#ifdef OS_MINGW
   char *buf = (char *) malloc(l1 + l2 + 2);
#else
   char *buf = (char *) alloca(l1 + l2 + 2);
#endif

   memcpy(buf, name, l1);
   memcpy(buf + l1 + 1, val, l2);
   buf[l1] = '=';
   buf[l1 + l2 + 1] = 0;
   r = put_env(&_clip_envp, buf);
#ifdef OS_MINGW
   free(buf);
#endif

   return r;
}
