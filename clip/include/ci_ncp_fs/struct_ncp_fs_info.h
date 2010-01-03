struct ncp_fs_info
{
   int       version;
   union ncp_sockaddr addr;
	/*__kerXX_uid_t mounted_uid;*/
   uid_t     mounted_uid;
   int       connection;	/* Connection number the server assigned us */
   int       buffer_size;	/* The negotiated buffer size, to be
				   used for read/write requests! */

   int       volume_number;
   u_int32_t directory_id;
};
