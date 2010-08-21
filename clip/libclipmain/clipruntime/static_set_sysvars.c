static void
set_sysvars(void)
{
   char *e;

   e = get_env(_clip_envp, "CLIP_LOGLEVEL");
   if (e && *e)
      log_level = atoi(e);

}
