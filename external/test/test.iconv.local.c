#include "/usr/local/include/iconv.h"
int main(int argc, char **argv) { iconv_t it; it = iconv_open("utf-8", "utf-8"); return 0;}
