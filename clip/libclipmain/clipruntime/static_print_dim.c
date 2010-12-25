static void
print_dim(ClipMachine * ClipMachineMemory, char *buf, int size, int dim, int Dim, long *Vect)
{
   char *s = buf, *e = buf + size - 20;

   int i;

   for (i = 0, *s = 0; i < Dim - dim && s < e; ++i)
   {
      char bb[64];

      s = s + strlen(s);
      _clip_hash_name(ClipMachineMemory, GETLONG(Vect + i), bb, sizeof(bb));
      sprintf(s, "[%s]", bb);
   }
}
