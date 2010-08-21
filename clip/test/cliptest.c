
#include <ci_clip.h>

int
QQout( ClipVar v)
{
	printf("this is : %d", v);
}

int main()
{
	int v = 1;
	QQout(v);
}