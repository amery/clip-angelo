static int
get_color(ClipMachine * ClipMachineMemory, char *str)
{
   if (str)
   {
      int l = strcspn(str, ",");

      return _clip_str2attr(str, l);
   }
   else
      return _clip_colorSelect(ClipMachineMemory);
}
