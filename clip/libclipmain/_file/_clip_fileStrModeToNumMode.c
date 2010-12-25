int
_clip_fileStrModeToNumMode(char *mode)
{
   /* mode can formats "777" or "7777" */
   /* sticky/user/group/other access mode */
   int modes[4][3] = {
      {S_ISUID, S_ISGID, S_ISVTX},
      {S_IEXEC, S_IWRITE, S_IREAD},
      {S_IXGRP, S_IWGRP, S_IRGRP},
      {S_IXOTH, S_IWOTH, S_IROTH}
   };
   int ret = 0, cur, pos;

   int tmp, end = strlen(mode);

   for (cur = 4 - end, pos = 0; pos <= end; cur++, pos++)
   {
      /* sticky/user/group/other access mode */
      tmp = *(mode + pos) - '0';
      if (tmp & 0x0001)
	 ret += modes[cur][0];
      if (tmp & 0x0002)
	 ret += modes[cur][1];
      if (tmp & 0x0004)
	 ret += modes[cur][2];
   }
   return ret;
}
