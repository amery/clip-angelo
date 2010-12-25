int
clip_DISKTYPE(ClipMachine * ClipMachineMemory)	/* Determines the type of data carrier */
{
/*
   255     DT_DS_SEC_8         Double sided 8 sectors
   254     DT_SS_SEC_8         Single sided 8 sectors
   253     DT_DS_SEC_9         Double sided 9 sectors
   252     DT_SS_SEC_9         Single sided 9 sectors
   249     DT_DS_SEC_15        Double sided 15 sectors (HD-Disk)
   249     DT_35_SEC_9         Double sided 3.5" 9 sectors (ident. to 5.25" HD)
   240     DT_35_SEC_18        Double sided 3.5" 18 sectors
   248     DT_HARDDISK         Hard disk
 */
#ifdef _WIN32
   LPTSTR path = _clip_parc(ClipMachineMemory, 1);

   LPTSTR buf;

   int dType;

   int ret = 0, len;

   if (path == NULL || (*path) == 0)
      path = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   len = strlen(path);
   switch (len)
   {
   case 1:
      buf = malloc(4 * sizeof(LPTSTR));
      buf[0] = path[0];
      buf[1] = ':';
      buf[2] = '/';
      buf[3] = 0;
      break;
   case 2:
      buf = malloc(4 * sizeof(LPTSTR));
      buf[0] = path[0];
      buf[1] = path[1];
      buf[2] = '/';
      buf[3] = 0;
      break;
   default:
      buf = malloc(len + 1);
      memcpy(buf, path, len);
      ((char *) buf)[len] = 0;
      break;
   }
   dType = GetDriveType(buf);
   free(buf);

   switch (dType)
   {
   case 0:
   case 1:
      break;
   case DRIVE_RAMDISK:
      ret = 254;
      break;
   case DRIVE_FIXED:
   case DRIVE_REMOTE:
   case DRIVE_CDROM:
      ret = 248;
      break;
   case DRIVE_REMOVABLE:
      ret = 249;
      break;

   }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
#else
   _clip_retni(ClipMachineMemory, 248);
   return 0;
#endif
}
