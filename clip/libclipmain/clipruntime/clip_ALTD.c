/*
*	ALTD(0) - disable debugging
*	ALTD(1) - enable debugging
*	ALTD(2) - enable debugging and wait connection
*/

int
clip_ALTD(ClipMachine * ClipMachineMemory)
{
   if (!_clip_debuglevel)
   {
      _clip_debuglevel = 1;
#ifndef OS_MINGW
      signal(SIGUSR1, _clip_sigdebug);
#endif
   }

   _clip_in_breakpoint();
   return 0;
}
