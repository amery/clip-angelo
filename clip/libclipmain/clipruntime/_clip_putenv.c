CLIP_DLLEXPORT int
_clip_putenv(char *name)
{
   int r;

   r = put_env(&_clip_envp, name);
   return r;
}
