CLIP_DLLEXPORT char *
_clip_getenv(char *name)
{
   char *r;

   r = get_env(_clip_envp, name);
   return r;
}
