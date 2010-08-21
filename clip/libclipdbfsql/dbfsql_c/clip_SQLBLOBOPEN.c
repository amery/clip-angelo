int
clip_SQLBLOBOPEN(ClipMachine * ClipMachineMemory)
{
   int conn_item = _clip_parni(ClipMachineMemory, 1);

   SQLCONN *conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, conn_item, _C_ITEM_TYPE_SQL);

   unsigned int OID = (unsigned int) _clip_parni(ClipMachineMemory, 2);

   char *cmode = _clip_parc(ClipMachineMemory, 3);

   int mode = 0;

   char clen = 5;

   while (clen)
   {
      clen--;
      switch (*cmode)
      {
      case 'r':
      case 'R':
	 mode |= 1;
	 break;
      case 'w':
      case 'W':
	 mode |= 2;
	 break;
      case '\0':
	 clen = 0;
      }
      cmode++;
   }
   if (!conn)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOCONNECT, er_noconnect);
      return 1;
   }
   if (conn->vtbl->blob_open)
      return conn->vtbl->blob_open(ClipMachineMemory, conn, OID, mode);
   return 0;
}
