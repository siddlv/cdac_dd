#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
int main()
{
	int FileDesc;
	char Ubuff[]="User Buffer: Data from User Space";
	char Kbuff[100];	
	FileDesc=open("/dev/MyCharDevice",O_RDWR,0777);
	if(FileDesc <0)
	{
		printf("\nError Opening Device\n");	
		exit(1);
	}
	write(FileDesc,Ubuff,sizeof(Ubuff));
	read(FileDesc,Kbuff,sizeof(Kbuff));
	printf("\nData From Kernel\n%s\n",Kbuff);
	close(FileDesc);
}
