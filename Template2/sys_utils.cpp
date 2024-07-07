#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "sys_utils.h"

typedef struct _gpio_entry_t
{
    int gpio;
    int fd;
}gpio_entry_t;

static gpio_entry_t gpio_entry[64] = {0};

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

int utils_system_ex(const char *cmd, char *recv, unsigned int max_size)
{
    FILE *fp = NULL;
	int ret, rv;
	unsigned int sizet = 0;
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
		        printf("goto _per_end\n");
                goto _per_end;
            }
        }
        while(sizet == 0 && EINTR == errno);
        
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

static gpio_entry_t *gpio_entry_empty_get(void)
{
    int i = 0;
    
    for (i = 0; i < array_num(gpio_entry); ++i)
    {
        if (0 == gpio_entry[i].gpio)
        {
            return &gpio_entry[i];
        }
    }
    
    return NULL;
}

static gpio_entry_t *gpio_entry_search(int gpio)
{
    int i = 0;
    
    for (i = 0; i < array_num(gpio_entry); ++i)
    {
        if (gpio == gpio_entry[i].gpio)
        {
            return &gpio_entry[i];
        }
    }
    
    return NULL;
}

static gpio_entry_t *gpio_entry_get(int gpio)
{
    gpio_entry_t *p_entry = NULL;
    
    p_entry = gpio_entry_search(gpio);
    if (p_entry != NULL)
    {
        return p_entry;
    }
    
    return gpio_entry_empty_get();
}

int gpio_read(int gpio)
{
    gpio_entry_t *p_entry;
    char buff[256];
 
    p_entry = gpio_entry_search(gpio);
    if (NULL != p_entry && p_entry->fd > 0)
    {
        lseek(p_entry->fd, 0, SEEK_SET);
        read(p_entry->fd, buff, sizeof(buff));
        return buff[0] - '0';
    }

    return -1;
}

void gpio_open(int gpio)
{
    char buff[256];
    gpio_entry_t *p_entry = gpio_entry_get(gpio);

    if (NULL != p_entry)
    {
        sprintf(buff, "/sys/class/gpio/gpio%d/value", gpio);
        p_entry->fd = open(buff, O_RDWR);
        p_entry->gpio = (p_entry->fd >= 0) ? gpio : 0;
    }
}

void gpio_close(int gpio)
{
    gpio_entry_t *p_entry = gpio_entry_get(gpio);

    if (NULL != p_entry)
    {
        if (p_entry->gpio && p_entry->fd)
        {
            close(p_entry->fd);
            p_entry->gpio = 0;
            p_entry->fd = -1;
        }
    }
}