int
clip_INIT_DBFNTX(ClipMachine * ClipMachineMemory)
{
   char      a[2];

   int       i;


   a[1] = 0;
   for (i = 0; i < 11; i++)
    {
       a[0] = 'A' + i;
       A_K_aliasHashes[i] = _clip_hashstr(a);
    }
   register_dbdriver(ClipMachineMemory, "DBFNTX", "DBF", "NTX", "DBT");
   return 0;
}
