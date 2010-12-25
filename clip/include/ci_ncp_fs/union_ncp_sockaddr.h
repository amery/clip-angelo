union ncp_sockaddr
{
	struct sockaddr any_of_ncp_sockaddr;
#ifdef NCP_IPX_SUPPORT
struct sockaddr_ipx ipx_of_ncp_sockaddr;
#endif
#ifdef NCP_IN_SUPPORT
struct sockaddr_in inet_of_ncp_sockaddr;
#endif
};
