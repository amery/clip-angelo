/*
   push XTERM mouse sequence
 */
static void
w32_push_mouse_event(MOUSE_EVENT_RECORD * ClipMachineMemory)
{
   int state = ClipMachineMemory->dwButtonState;

   int event = ClipMachineMemory->dwEventFlags;

   int bb, x, y, i;

   unsigned char sb[8];

   x = ClipMachineMemory->dwMousePosition.X;
   y = ClipMachineMemory->dwMousePosition.Y;

   if (event == MOUSE_MOVED)
      bb = 4;
   else if (!state)
      bb = 3;
   else if (state & FROM_LEFT_1ST_BUTTON_PRESSED)
      bb = 0;
   else if (state & FROM_LEFT_2ND_BUTTON_PRESSED)
      bb = 1;
   else if (state & RIGHTMOST_BUTTON_PRESSED)
      bb = 2;
   else
      bb = 4;

   sb[0] = RAWMODE_ESC;
   sb[1] = bb;
   sb[2] = x;
   sb[3] = y;

   for (i = 0; i < 4; i++)
      w32_put_scan_buf(sb[i]);
}
