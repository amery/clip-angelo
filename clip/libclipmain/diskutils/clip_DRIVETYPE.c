int
clip_DRIVETYPE(ClipMachine * ClipMachineMemory)	/* Determines the drive type */
{
/*
   0       RAM Disk
   1       Floppy Disk (disk change cannot be established by system)
   2       Floppy Disk (disk change can be established by system)
   3       Hard Disk
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
      ret = 0;
      break;
   case DRIVE_FIXED:
   case DRIVE_REMOTE:
   case DRIVE_CDROM:
      ret = 3;
      break;
   case DRIVE_REMOVABLE:
      ret = 2;
      break;

   }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
#else
   _clip_retni(ClipMachineMemory, 3);
   return 0;
#endif
}
