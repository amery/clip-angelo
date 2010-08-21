static void
print_dbg(ClipMachine * ClipMachineMemory, ClipVar * vp, int binary)
{
   if (!binary)
      _clip_print_dbg(ClipMachineMemory, vp);
   else
   {
      char *str = 0;

      long len = 0;

      _clip_var2str(ClipMachineMemory, vp, &str, &len, 1);
      fwrite(str, len, 1, _clip_dbg_out);
      fwrite("\n", 1, 1, _clip_dbg_out);
      free(str);
   }
}
