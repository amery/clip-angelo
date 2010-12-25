struct ncp_fs_info
{
	int version_of_ncp_fs_info;
	union ncp_sockaddr addr_of_ncp_fs_info;
	/*__kerXX_uid_t mounted_uid;*/
	uid_t mounted_uid_of_ncp_fs_info;
	int connection_of_ncp_fs_info;		/* Connection number the server assigned us */
   int buffer_size_of_ncp_fs_info;		/* The negotiated buffer size, to be
				   used for read/write requests! */

int volume_number_of_ncp_fs_info;
u_int32_t directory_id_of_ncp_fs_info;
};
