static    nls_uint32
SWAP(nls_uint32 i)
{
   return (i << 24) | ((i & 0xff00) << 8) | ((i >> 8) & 0xff00) | (i >> 24);
}
