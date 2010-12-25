static char *
tskip(char *bp)
{
   while (*bp && *bp != ':')
      bp++;
   if (*bp == ':')
      bp++;
   return (bp);
}
