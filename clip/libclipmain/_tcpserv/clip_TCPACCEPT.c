int
clip_TCPACCEPT(ClipMachine * ClipMachineMemory)
/* Return a new socket for pending connections.
	uso : tcpAccept( sock, timeout ) ->  nNewSocket  | -1 == error
*/
{
   int ret = -1;

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int sock = _clip_parni(ClipMachineMemory, 1);

   int timeout = _clip_parni(ClipMachineMemory, 2);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, sock, _C_ITEM_TYPE_FILE);

   struct sockaddr_in sin;

   int socknew;

   socklen_t sinlen = sizeof(sin);

   fd_set readfd;

   struct timeval tv;

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
   {
      *err = EBADF;
   }
   else
   {

      if (_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType)
	 timeout = cf->timeout_of_C_FILE;

      FD_ZERO(&readfd);
      FD_SET(cf->fileno_of_C_FILE, &readfd);
      //timeout = 0;
      tv.tv_sec = timeout / 1000;
      tv.tv_usec = (timeout % 1000) * 1000;

      if (select(cf->fileno_of_C_FILE + 1, &readfd, NULL, NULL, &tv) < 1)
      {
	 *err = errno;
	 _clip_retnl(ClipMachineMemory, ret);
	 return 0;
      }

      if ((socknew = accept(cf->fileno_of_C_FILE, (struct sockaddr *) &sin, &sinlen)) == -1)
	 *err = errno;
      else
      {
	 /* Accept correcto. */
	 // Se registra el socket en clip.
	 cf = (C_FILE *) calloc(1, sizeof(C_FILE));
	 cf->fileno_of_C_FILE = socknew;
	 cf->file_of_C_FILE = NULL;
	 cf->type_of_C_FILE = FT_SOCKET;
	 cf->pid_of_C_FILE = -1;
	 cf->timeout_of_C_FILE = timeout;	/* timeout; */
	 cf->stat_of_C_FILE = 0;	/* see FS_* flags */
	 ret = _clip_store_c_item(ClipMachineMemory, cf, _C_ITEM_TYPE_FILE, destroy_c_file);
      }

   }
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
