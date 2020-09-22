#include <stdio.h>
int main(void) {
	char buf[1024];
	FILE* fh = fopen("/etc/resolv.conf", "r");
	while(fgets(buf, sizeof(buf), fh))
	{
		fputs(buf, stdout);
	}
	fclose(fh);
	return 0;
}
