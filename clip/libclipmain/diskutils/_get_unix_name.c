
/*
 * ���������� ��� ����� � �������� ������� unix, ������� ���� �������
 */

char *
_get_unix_name(ClipMachine * ClipMachineMemory, char *dname)
{
   char *buf = NULL;

   if (dname == NULL)
   {
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");*/
      return NULL;
   }
   if (!(buf = (char *) calloc(MAXPATHLEN, 1)))
   {
      _clip_trap_err(ClipMachineMemory, EG_MEM, 0, 0, __FILE__, __LINE__, "cannot allocate memory");
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "cannot allocate memory");*/
      return NULL;
   }
   _clip_translate_path(ClipMachineMemory, dname, buf, MAXPATHLEN);
   return buf;
}
