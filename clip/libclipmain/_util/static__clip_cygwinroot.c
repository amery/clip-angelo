static char *
_clip_cygwinroot()
{
#ifdef OS_CYGWIN
   /* need change to WIN32 registry data */
   char *s = "c:\\cygwin";
#else
   char *s = "";
#endif
   return s;
}
