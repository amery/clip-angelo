int
clip_KBDSTAT(ClipMachine * ClipMachineMemory)
{
   int flags;

   int r;

   _clip_fullscreen(ClipMachineMemory);

   flags = getState_Key(ClipMachineMemory->screen->base);

   r = 0;
   if (flags & (1 << KEY_SHIFTL))
      r |= 0x1;
   if (flags & (1 << KEY_SHIFTR))
      r |= 0x2;
   if (flags & (1 << KEY_SHIFT) && !r)
      r |= 0x3;

   if (flags & ((1 << KEY_CTRL) | (1 << KEY_CTRLL) | (1 << KEY_CTRLR)))
      r |= 0x4;
   if (flags & ((1 << KEY_ALT) | (1 << KEY_ALTGR)))
      r |= 0x8;
   if (flags & (1 << KEY_SLOCK))
      r |= 0x10;
   if (flags & (1 << KEY_NATKBD))
      r |= 0x20000;
   if (flags & (1 << KEY_NUMLOCK))
      r |= 0x20;
   if (flags & (1 << KEY_CAPSLOCK))
      r |= 0x40;
   if (flags & (1 << KEY_INSLOCK))
      r |= 0x80;

   _clip_retni(ClipMachineMemory, r);

   return 0;
}
