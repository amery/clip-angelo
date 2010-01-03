static void
pr_errorcount(int level)
{
   char      fmt[128];

   snprintf(fmt, sizeof(fmt), "%%d %s\n", _clic_ngettext("error", "errors", clic_errorcount));

   vr_printf(level, fmt, clic_errorcount);
}
