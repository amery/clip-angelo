int
clip_TIME(ClipMachine * ClipMachineMemory)
{
   char *ret = (char *) malloc(9);

#ifdef _WIN32
   SYSTEMTIME st;

   GetLocalTime(&st);
   snprintf(ret, 9, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
#else
   time_t t = time(0);

#if 0
   struct tm sysTime;

   sysTime = *(localtime(&t));
   snprintf(ret, 9, "%02d:%02d:%02d", sysTime.tm_hour, sysTime.tm_min, sysTime.tm_sec);
#else
   struct tm *sysTime;

   sysTime = localtime(&t);
   snprintf(ret, 9, "%02d:%02d:%02d", sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);
#endif
#endif
   _clip_retcn_m(ClipMachineMemory, ret, 8);

   return 0;
}
