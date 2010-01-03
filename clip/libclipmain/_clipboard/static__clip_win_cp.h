#ifdef _WIN32
static unsigned char *
_clip_win_cp(int who)
{
   int       cp_num = 0;

   unsigned char *buf;

   switch (who)
    {
    case 1:
       cp_num = GetConsoleCP();
       break;
    case 2:
       cp_num = GetACP();
       break;
    case 3:
       cp_num = GetOEMCP();
       break;
    case 4:
       cp_num = GetConsoleOutputCP();
       break;
    }
   buf = malloc(10);
   snprintf(buf, 10, "cp%d", cp_num);
   return buf;
}
#endif
