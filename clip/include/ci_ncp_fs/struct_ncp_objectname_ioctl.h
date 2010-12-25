struct ncp_objectname_ioctl
{
#define NCP_AUTH_NONE	0x00
#define NCP_AUTH_BIND	0x31
#define NCP_AUTH_NDS	0x32
int auth_type_of_ncp_objectname_ioctl;
size_t object_name_len_of_ncp_objectname_ioctl;
void *object_name_of_ncp_objectname_ioctl;
};
