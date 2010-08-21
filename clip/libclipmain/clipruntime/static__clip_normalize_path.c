static char *
_clip_normalize_path(char *_path)
{
   /* translate "path1/../path2" -> "path2" */
   int pos[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   int pos2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   int flag[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   int i, beg, end, len, cur = 1, cur2 = 0;

   char *tmp;

   char *path;

   path = strdup(_path);

   i = 0;
   if (path[i] == '/' || path[i] == '\\')
   {
      i++;
      pos[0] = 1;
   }
   for (len = strlen(path); i < len; i++)
   {
      if (path[i] == '.')
	 flag[cur]++;
      else if (path[i] == '/' || path[i] == '\\')
      {
	 pos[cur] = i + 1;
	 cur++;
      }
      else
	 flag[cur]--;
      if (cur > 15)
	 break;
   }
   for (cur = 0, cur2 = -1; cur < 16; cur++)
   {
      if (flag[cur] == 2)	/* subpath ==".." */
	 cur2--;
      else
	 cur2++;
      if (cur2 < 0)
	 cur2 = 0;
      //printf("\ncur=%d,flag=%d,cur2=%d",cur,flag[cur],cur2);
      pos2[cur2] = cur;
   }

   tmp = strdup(path);
   //printf("\npos[0]=%d,pos[1]=%d,pos[2]=%d",pos[0],pos[1],pos[2]);
   //printf("\npo2[0]=%d,po2[1]=%d,po2[2]=%d",pos2[0],pos2[1],pos2[2]);
   memset(path, 0, len);
   for (i = 0, len = 0; i < 16; i++)
   {
      beg = pos[pos2[i]];
      end = pos[pos2[i] + 1];
      if (end == 0)
      {
	 end = strlen(tmp);
	 i = 200;
      }
      strncpy(path + len, tmp + beg, end - beg);
      len += (end - beg);
      //printf("\ni=%d,pos2=%d,beg=%d,end=%d,tmp=%s,str=%s\n",i,pos2[i],beg,end,tmp,path);
   }
   return path;
}
