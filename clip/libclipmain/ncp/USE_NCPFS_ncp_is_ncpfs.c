int
ncp_is_ncpfs(int fd)
{
   struct ncp_fs_info ncp_fs_info;

   int r;

   memset(&ncp_fs_info, 0, sizeof(ncp_fs_info));

	ncp_fs_info.version_of_ncp_fs_info = NCP_GET_FS_INFO_VERSION;
   r = ioctl(fd, NCP_IOC_GET_FS_INFO, &ncp_fs_info);

	if (r || !ncp_fs_info.connection_of_ncp_fs_info)
      return 0;
   return 1;
}
