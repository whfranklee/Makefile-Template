#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>

int check_shell_status(int rv)
{
    /* no child pricess execute.
    */
    if (-1 == rv)
    {
        return -1;
    }

    /* check child process normal exit.
    */
    if (!WIFEXITED(rv))
    {
        return -1;
    }

    /* check shell execute correct.
    */
    if (0 != WEXITSTATUS(rv))  
    {
        return -1;
    }  
    
     return 0;  
}


int utils_system_ex(const char *cmd, char *recv, unsigned long max_size)
{
    FILE *fp = NULL;
	int ret, rv, sizet = 0;
	unsigned int cnt = 0;
	
	/* we use pipe to execute this cmd.
	*/
	fp = popen(cmd, "r");
	if (NULL == fp)
    {
        return -1;
    }
    
    if (NULL != recv)
    {
        do
        {
            sizet = fread(recv, sizeof(char), max_size, fp);
            if (++cnt > 30)
            {
                sizet = 0;
                goto _per_end;
            }
        }
        while(sizet <= 0 && EINTR == errno);
        
        sizet = (sizet >= max_size) ? max_size - 1 : sizet;
        recv[sizet] = '\0';
    }
    
_per_end:
    rv = pclose(fp);
    ret = check_shell_status(rv);
    if (0 == ret)
    {
        ret = sizet;
    }
    
	return ret;
}

int main(int argc, char **argv)
{
	int ret = -1;
	char cmd[512] = {0};
	char res[512] = {0};

	sprintf(cmd, "dirname $(readlink -f \"$0\")");
	printf("cmd = %s\n",cmd);

	ret = utils_system_ex(cmd, res, sizeof(res));
	
	if(ret > 0)
	{
		printf("len=%d recv:%s\n",ret,res);
	
		char base_path[128] = {'\0'};
		char exec_path[128] = {'\0'};
		char exec_arg[128] = {'\0'};
		pid_t pid;

		strncpy(base_path,res,strlen(res)-strlen("/bin"));
		sprintf(exec_path, "%s/bin/lbfs",base_path);
		sprintf(exec_arg, "%s/conf",base_path);

		pid = fork();
		if(pid < 0)
		{
			printf("monitor fork error.\n");
			exit(1);
		}
		else if(pid == 0) //child
		{

			int fd = open("/dev/null", O_RDWR);
			if(fd < 0)
			{
			    printf("open /dev/null error\n");
			}
			else
			{
			    //printf("dump to /dev/null\n");
			    dup2(fd, 0);
			    //dup2(fd, 1);
			    dup2(fd, 2);
			}

			printf("child pid= %d\n", getpid());
			close(fd);
		}
		else
		{
			printf("exec: %s\n", exec_path);
			printf("father process pid=%d  child_pid=%d\n", getpid(), pid);
		}
	}

	while(1)
	{
		sleep(1);
	}
	return 0;
}
