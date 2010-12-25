struct ncp_nls_ioctl
{
	unsigned char codepage_of_ncp_nls_ioctl[NCP_IOCSNAME_LEN + 1];
	unsigned char iocharset_of_ncp_nls_ioctl[NCP_IOCSNAME_LEN + 1];
};
