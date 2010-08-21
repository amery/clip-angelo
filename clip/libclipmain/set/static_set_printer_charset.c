static void
set_printer_charset(ClipMachine * ClipMachineMemory)
{
   char *p1, *p2;

   cons_CharsetEntry *cs1 = 0, *cs2 = 0;

   int len1 = 0, len2 = 0;

   p2 = (char *) _clip_fetch_item(ClipMachineMemory, _clip_hashstr("PRINTER_CHARSET"));
   p1 = _clip_getenv("CLIP_HOSTCS");
   if (!p2)
      p2 = _clip_getenv("CLIP_CLIENTCS");
   if (!p1 || !p2 || !*p1 || !*p2)
      goto norm;

   _clip_logg(2, "set printer translation  %s -> %s", p1, p2);

   if (load_charset_name(p1, &cs1, &len1))
   {
      _clip_logg(0, "cannot load charset file '%s': %s", p1, strerror(errno));
      return;
   }

   if (load_charset_name(p2, &cs2, &len2))
   {
      free(cs1);
      _clip_logg(0, "cannot load charset file '%s': %s", p2, strerror(errno));
      return;
   }

   make_translation(cs1, len1, cs2, len2, ClipMachineMemory->prntbl);

   free(cs1);
   free(cs2);
 norm:
   ;
}
