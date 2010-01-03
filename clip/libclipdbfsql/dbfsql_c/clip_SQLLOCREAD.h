int
clip_SQLLOCREAD(ClipMachine * ClipMachineMemory)
{
   int       conn_item = _clip_parni(ClipMachineMemory, 1);

   SQLCONN  *conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, conn_item, _C_ITEM_TYPE_SQL);

   ClipVar  *str = _clip_vptr(_clip_par(ClipMachineMemory, 2));

   unsigned char *c;

   unsigned char *e;

   if (!conn)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOCONNECT, er_noconnect);
       return 1;
    }

   c = (unsigned char *) str->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
   if (c && c[0])
    {
       e = c + str->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
       for (; c < e; c++)
	  if (*c & 0x80)
	     *c = conn->loc->read[*c & 0x7f];
    }
   return 0;
}
