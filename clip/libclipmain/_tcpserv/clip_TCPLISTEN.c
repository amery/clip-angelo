/*****************************/
int
clip_TCPLISTEN(ClipMachine * ClipMachineMemory)
/* Create a socket and stand for connetions.
  Use : TcpListen( port[, nMaxConn] )
*/
{
   struct sockaddr_in sin;

   long port = 0, timeout = 60000;

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror), i;

   char *sport;

   int sock = -1, ret = -1;

   int backlog = 5;

   C_FILE *cf = NULL;

   int arg;

   *err = 0;

   if (_clip_parinfo(ClipMachineMemory, 0) < 1)
   {
      *err = EINVAL;
      goto err;
   }

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
      port = htons(_clip_parnl(ClipMachineMemory, 1));
   else if ((sport = _clip_parc(ClipMachineMemory, 1)) != NULL)
   {
      struct servent *sp;

      if ((sp = getservbyname((const char *) sport, "tcp")) != NULL)
	 port = sp->s_port;
      else
      {
	 for (i = 0; sport[i] >= '0' && sport[i] <= '9'; i++);
	 if (sport[i] == '\0')
	    port = htons(atol(sport));
      }
   }

   if (port == 0)
   {
      *err = EINVAL;
      goto err;
   }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      backlog = _clip_parnl(ClipMachineMemory, 2);

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      goto err;

   if ((arg = fcntl(sock, F_GETFL, 0)) == -1)
      goto err;
   fcntl(sock, F_SETFL, arg | O_NONBLOCK);

   memset((void *) &sin, 0, sizeof(sin));
   sin.sin_family = AF_INET;	/* PF_INET ?? */
   sin.sin_addr.s_addr = INADDR_ANY;
   sin.sin_port = port;

   if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1)
      /* Error, no bind */
   {

      *err = EFAULT;
      goto err;

   }

   if (listen(sock, backlog) == -1)
   {
      *err = EFAULT;
      goto err;
   }

   cf = (C_FILE *) calloc(1, sizeof(C_FILE));
   cf->fileno_of_C_FILE = sock;
   cf->file_of_C_FILE = NULL;
   cf->type_of_C_FILE = FT_SOCKET;
   cf->pid_of_C_FILE = -1;
   cf->timeout_of_C_FILE = timeout;
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
