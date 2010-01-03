struct ncp_nls_ioctl
{
   unsigned char codepage[NCP_IOCSNAME_LEN + 1];
   unsigned char iocharset[NCP_IOCSNAME_LEN + 1];
};
