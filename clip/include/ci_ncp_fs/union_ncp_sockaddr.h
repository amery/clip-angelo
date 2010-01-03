union ncp_sockaddr
{
   struct sockaddr any;
#ifdef NCP_IPX_SUPPORT
   struct sockaddr_ipx ipx;
#endif
#ifdef NCP_IN_SUPPORT
   struct sockaddr_in inet;
#endif
};
