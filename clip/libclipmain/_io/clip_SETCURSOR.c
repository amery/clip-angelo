int
clip_SETCURSOR(ClipMachine * ClipMachineMemory)
{
   int oldcursor;

   int ret = 0;

   _clip_fullscreen(ClipMachineMemory);
   oldcursor = ClipMachineMemory->screen->cursor;

   switch (oldcursor)
   {
   case 0:
      ret = 1;
      break;
   case 1:
      ret = 0;
      break;
   case 2:
      ret = 3;
      break;
   }

   if (ClipMachineMemory->argc > 0)
   {
      int cursor = _clip_parni(ClipMachineMemory, 1);

      int scr_cursor;

      switch (cursor)
      {
      case 0:
	 scr_cursor = 1;
	 break;
      case 1:
      default:
	 scr_cursor = 0;
	 break;
      case 2:
      case 3:
      case 4:
	 scr_cursor = 2;
	 break;
      }

      ClipMachineMemory->screen->cursor = scr_cursor;

      sync_mp(ClipMachineMemory);
   }

   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
