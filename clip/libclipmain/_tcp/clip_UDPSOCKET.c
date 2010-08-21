int
clip_UDPSOCKET(ClipMachine * ClipMachineMemory)
{
   C_FILE *cf = NULL;

   int arg = 0, sock = -1, ret = -1;

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   *err = 0;
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
      goto err;

#ifndef OS_MINGW
   if ((arg = fcntl(sock, F_GETFL, 0)) == -1)
      goto err;
   fcntl(sock, F_SETFL, arg | O_NONBLOCK);
#endif

   cf = (C_FILE *) calloc(1, sizeof(C_FILE));
   cf->fileno_of_C_FILE = sock;
   cf->file_of_C_FILE = NULL;
   cf->type_of_C_FILE = FT_SOCKET;
   cf->pid_of_C_FILE = -1;
   cf->timeout_of_C_FILE = 60000;
   cf->stat_of_C_FILE = 0;	/* see FS_* flags */
   ret = _clip_store_c_item(ClipMachineMemory, cf, _C_ITEM_TYPE_FILE, destroy_c_file);

 err:
   if (ret == -1)
   {
      if (*err != 0)
	 *err = errno;
      if (sock != -1)
	 close(sock);
   }
   _clip_retni(ClipMachineMemory, ret);

   return 0;
}
