unsigned long
getpagesize()
{
   static DWORD ps = 0;

   SYSTEM_INFO si;

   if (ps == 0)
   {
      GetSystemInfo(&si);
      ps = si.dwPageSize;
   }

   return ps;
}
