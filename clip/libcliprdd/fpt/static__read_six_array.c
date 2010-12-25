static void
_read_six_array(ClipMachine * ClipMachineMemory, ClipVar * vp, DbfLocale * loc, char **s, int len)
{
   char *b = *s;

   char *e = b + len;

   int size, i;
   long dims[1] = { 0 };

   _clip_array(ClipMachineMemory, vp, 1, dims);
   *s += 2;
   size = _rdd_uint((unsigned char *) (*s));
   *s += 12;
   for (i = 0; i < size; i++)
   {
      int type = _rdd_backushort((unsigned char *) (*s));

      ClipVar v;

      memset(&v, 0, sizeof(ClipVar));
      switch (type)
      {
      case 0x4:
	 _read_six_str(&v, loc, s);
	 _clip_aadd(ClipMachineMemory, vp, &v);
	 break;
      case 0x800:
	 _read_six_double(&v, ClipMachineMemory->decimals, s);
	 _clip_aadd(ClipMachineMemory, vp, &v);
	 break;
      case 0x200:
	 _read_six_int(&v, s);
	 _clip_aadd(ClipMachineMemory, vp, &v);
	 break;
      case 0x2000:
	 _read_six_date(&v, s);
	 _clip_aadd(ClipMachineMemory, vp, &v);
	 break;
      case 0x8000:
	 _read_six_log(&v, s);
	 _clip_aadd(ClipMachineMemory, vp, &v);
	 break;
      case 0x80:
	 _read_six_array(ClipMachineMemory, &v, loc, s, len - (*s - b));
	 _clip_aadd(ClipMachineMemory, vp, &v);
	 break;
      default:
	 *s = e;
	 break;
      }
      if (*s >= e)
	 break;
   }
}
