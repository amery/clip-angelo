int
put_locale_string(char *name)
{
   if (!out && file_name)
      set_locale();
   if (!out)
      return 0;

   fprintf(out, "#: %s:%ld\n", file_name, clic_line);
   fprintf(out, "msgid \"");
   put_str(out, name);
   fprintf(out, "\"\n");
   fprintf(out, "msgstr \"\"\n\n");

   v_printf(2, "wrote locale message entry '%s'\n", name);
   return 0;
}
