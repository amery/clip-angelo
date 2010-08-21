static void
get_static(ClipMachine * ClipMachineMemory, ClipFile * file, ClipVar * statics, char *modbeg, int no, ClipVar ** dest)
{
   if (!statics->ClipType_t_of_ClipVar.count_of_ClipType)
   {
      ClipBlock init;

      long initOffs = GETLONG(F_OFFS(modbeg, 4, 4, 0));

      statics->ClipType_t_of_ClipVar.count_of_ClipType = 1;
      init.file_of_ClipBlock = file;
      init.function_of_ClipBlock = modbeg + initOffs;
      _clip_code_func(ClipMachineMemory, &init, 0, 0, 0);
   }
   *dest = statics + no + 1;
}
