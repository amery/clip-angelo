void
write_Main(File * file, FILE * out, const char *start)
{
   if (wrote_main)
      return;
   fprintf(out, "\n\nextern char **environ;\n");
   fprintf(out, "void _clip_init_dll(void);\n");
   fprintf(out, "\nint\nmain(int argc, char **argv)\n{\n");
   fprintf(out, "\tClipMachine *mp;\n\n");
   fprintf(out, "\t_clip_init_dll();\n");
   fprintf(out, "\tTask_INIT();\n");
   fprintf(out, "\tmp = new_ClipMachine(0);\n");
   fprintf(out, "\treturn _clip_main_func( mp, clip_%s, argc, argv, environ);\n", start);
   fprintf(out, "}\n");
   wrote_main = 1;
}
