int
put_locale_string_plural(char *singular, char *plural)
{
   if (!out && file_name)
      set_locale();
   if (!out)
      return 0;

   fprintf(out, "#: %s:%ld\n", file_name, clic_line);
   fprintf(out, "msgid \"");
   put_str(out, singular);
   fprintf(out, "\"\n");
   fprintf(out, "msgid_plural \"");
   put_str(out, plural);
   fprintf(out, "\"\n");
   fprintf(out, "msgstr[0] \"\"\n");
   fprintf(out, "msgstr[1] \"\"\n\n");

   v_printf(2, "wrote plural locale message '%s' '%s'\n", singular, plural);

   /*put_locale_string(singular); */
   /*put_locale_string(plural); */

   return 0;
}
