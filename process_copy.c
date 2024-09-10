#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

void check(int arg1,char**argv1)
{
	if(arg1<2)
	{
		printf("parameter error\n");
		exit(-1);
	}
	else if(arg1!=3)
	{
		if(atoi(argv1[3])>100||atoi(argv1[3])<1)
		{
			printf("son process parameter error\n");
			exit(-1);
		}

	}
	else if(arg1>4)
	{
		printf("too many parameters\n");
		exit(-1);
	}
	else
	{
		printf("test para right\n");
	}
}

int main(int arg,char **argv)
{
	check(arg,argv);

	//create son process
	pid_t pid;
	int i;
	int pronum;
	if(arg==3)
		pronum=3;
	else
		pronum=atoi(argv[3]);
		
	printf("process num is %d\n",pronum);
	
	for(i=0;i<pronum;i++)
	{
		pid=fork();
		if(pid==0)
			break;
		else if(pid==-1)
		{
			printf("fork err\n");
			exit(-1);
	
		}
	
	}
	//assign misson
	if(pid==0)
	{
	
		int fd1=open(argv[1],O_RDONLY);
		//check origin file does exist or not
		if(fd1==-1)
		{
			printf("origin file does not exist");
			exit(-1);
		}
		int fd2=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0664);
		//check origin size
		int block=lseek(fd1,0,SEEK_END);
		int blocksize=block/pronum+1;//division may be -1
	
		char *buf=(char*)malloc(blocksize);
		int rsize;
		int wsize;
		lseek(fd1,blocksize*i,SEEK_SET);
		lseek(fd2,blocksize*i,SEEK_SET);
		rsize=read(fd1,buf,blocksize);
		//printf("%d,read %d byte success\n",i,rsize);
		//last size may be not blocksize
		wsize=write(fd2,buf,rsize);
		printf("%d,copy %d byte success\n",i,wsize);
		close(fd1);
		close(fd2);
	
		exit(0);
	
	}
	else if(pid>0)
	{
		//waitpid
		pid_t pidz;
		//pidz=waitpid(-1,NULL,WNOHANG);
		for(int k=0;k<pronum;k++)
		{
		
			pid_t pidz=wait(NULL);
			//printf();
			if(pidz>0)
			printf("recycle success,pid is %d\n",pidz);
			else if(pidz<0)
			printf("recycle fail\n");

		
		}
		

	}

	return 0;
}

