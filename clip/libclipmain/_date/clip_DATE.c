int
clip_DATE(ClipMachine * ClipMachineMemory)
{
   int yy, mm, dd;

#ifdef _WIN32
   SYSTEMTIME st;

   GetLocalTime(&st);
   yy = st.wYear;
   mm = st.wMonth;
   dd = st.wDay;
#else
   struct tm *sysTime;

   sysTime = _clip_sysdate();
   yy = sysTime->tm_year + 1900;
   mm = sysTime->tm_mon + 1;
   dd = sysTime->tm_mday;
   free(sysTime);
#endif

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
      yy = _clip_parni(ClipMachineMemory, 1);
   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      mm = _clip_parni(ClipMachineMemory, 2);
   if (_clip_parinfo(ClipMachineMemory, 3) == NUMERIC_type_of_ClipVarType)
      dd = _clip_parni(ClipMachineMemory, 3);

   _clip_retdc(ClipMachineMemory, yy, mm, dd);
   return 0;
}
