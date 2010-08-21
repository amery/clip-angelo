/*
 * �������� ��� ����� unix, ������������ ��� ����� dos
 */
int
clip_DOSPATH(ClipMachine * ClipMachineMemory)
{
   char hstr[3], buf[MAXPATHLEN];

   int i = 0, len = 0;

   char *dsk = NULL, *ch = NULL, *uname = _clip_parc(ClipMachineMemory, 1);

   if (uname == NULL)
   {
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");*/
      return 1;
   }
   hstr[1] = ':';
   hstr[2] = 0;
   buf[0] = 0;
   for (i = 'A'; i <= 'Z'; i++)
   {
      hstr[0] = i;
      if ((dsk = _clip_fetch_item(ClipMachineMemory, _clip_hashstr(hstr))) == NULL)
	 continue;
      len = strlen(dsk);
      if (!memcmp(dsk, uname, len) && ((uname[len] == '/' || uname[len] == 0) || (len == 1 && dsk[len - 1] == '/')))
      {				/* found */
	 strcpy(buf, hstr);
	 strcat(buf, "\\");
	 if (uname[len] == '/')
	    strcat(buf, uname + len + 1);
	 else if (len == 1)
	    strcat(buf, uname + len);
	 ch = buf;
	 while ((ch = strchr(ch, '/')) != NULL)
	    *ch = '\\';
	 break;
      }
   }
   _clip_retc(ClipMachineMemory, buf);
   return 0;
}
