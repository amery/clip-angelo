int
clip_DISPBOXTERM(ClipMachine * ClipMachineMemory)
{
   int Top = _clip_parni(ClipMachineMemory, 1);

   int Left = _clip_parni(ClipMachineMemory, 2);

   int Bottom = _clip_parni(ClipMachineMemory, 3);

   int Right = _clip_parni(ClipMachineMemory, 4);

   int cl = 0;

   unsigned char *chars = (unsigned char *) _clip_parcl(ClipMachineMemory, 5, &cl);

   int chars_n = _clip_parni(ClipMachineMemory, 5);

   char *color = _clip_parc(ClipMachineMemory, 6);

   if (cl > 0 || !chars)
      disp_box(ClipMachineMemory, Top, Left, Bottom, Right, chars, cl, chars_n, color, 0);
   else
      disp_box(ClipMachineMemory, Top, Left, Bottom, Right, (unsigned char *) ("         "), 9, chars_n, color, 0);

   return 0;
}
