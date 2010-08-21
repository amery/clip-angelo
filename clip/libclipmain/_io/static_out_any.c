static void
out_any(ClipMachine * ClipMachineMemory, char *buf, int n, int attr, int dev)
{

   switch (dev)
   {
   case DEV_DEV:
      out_dev(ClipMachineMemory, buf, n, attr, 0);
      break;
   case DEV_DEVS:
      out_dev(ClipMachineMemory, buf, n, attr, 1);
      break;
   case DEV_SCR:
      out_scr(ClipMachineMemory, buf, n, attr, 0);
      break;
   case DEV_STD:
      out_std(ClipMachineMemory, buf, n, attr);
      break;
   case DEV_ERR:
      out_err(ClipMachineMemory, buf, n, attr);
      break;
   case DEV_LOG:
      out_log(ClipMachineMemory, buf, n, attr);
      break;
   case DEV_DBG:
      out_dbg(ClipMachineMemory, buf, n);
      break;
   }
}
