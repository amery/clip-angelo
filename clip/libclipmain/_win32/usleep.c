void
usleep(unsigned int usecs)
{
/* DWORD end, cur = GetTickCount();

	end = cur + usecs * 18.2 / 1000;
	while (GetTickCount() < end); */
   Sleep(usecs);
}
