/* load system DLL */
int
clip_LOADLIB(ClipMachine * ClipMachineMemory)
{
   /*void *hp; */
   char *name = _clip_parc(ClipMachineMemory, 1);

   char buf[256], *e, *s;

   int r;

   if (!name)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "no name given");
      return 1;
   }

#if 0
   hp = dlopen(name, RTLD_NOW | RTLD_GLOBAL);
   if (!hp)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "ELF '%s' loading problem: %s", name, dlerror());
      return 1;
   }
#else
   snprintf(buf, sizeof(buf), "%s", name);
   e = strrchr(buf, '.');
   s = strrchr(buf, '/');
   if (e && (!s || e > s))
      *e = 0;
   else
      e = buf + strlen(buf);
   strncpy(e, DLLREALSUFF, sizeof(buf) - strlen(buf));

   r = _clip_load(ClipMachineMemory, buf, 0, 0);

#endif
   _clip_retl(ClipMachineMemory, !r);

   return 0;
}
