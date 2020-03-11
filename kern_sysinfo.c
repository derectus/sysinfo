#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/file.h>
#include <linux/printk.h>
#include <linux/slab.h>


char *fcpuinfo = "/proc/cpuinfo";
char *floadavg = "/proc/loadavg";
char *fuptime  = "/proc/uptime";
// maximum size number i define this for dynamic allocation
long max_size = (~0UL » 1);

void dump_buffer(uint8_t *buf, int first, int last);
ssize_t read_file(char *name, void **buffer, size_t max_size, loff_t *position);

/**
 * dump_buffer() - print data for requested range from allocated buffer 
 * 
 * @arg1: allocated buffer
 * @arg2: first cursor position
 * @arg3: last cursor position
 */
void dump_buffer(uint8_t *buf, int first, int last)
{
    char *buffer = buf;
    int cursor = first;
    /* print data dmesg */
    for (cursor = 0; cursor < last; cursor++)
    {
        printk(KERN_CONT "%c", buffer[cursor]);
    }
}

/**
 *  read_file() - can read any file the file of the desired size at kernel space
 *  
 * @arg1: name of file
 * @arg2: data buffer
 * @arg3: maximum number value for maximum file size
 * @arg4: seek position 
 * */
ssize_t read_file(char *name, void **buffer, size_t max_size, loff_t *position)
{
    ssize_t retst = 0; 
    struct file *file = NULL;

    *buffer = NULL;
    *position = 0LL; //cursor position defined 

    // read file if exists
    file = filp_open(name, O_RDONLY, 0);
    if (file)
    {
        ssize_t size = 0x400; // read chunk size at one times
        ssize_t allocated = 0;
        ssize_t cursor = 0;
        *buffer = kzalloc(size, GFP_KERNEL); // allocate memory up to size
        if (*buffer) // if allocated
            allocated = size;
        else
        {
            // return out of memory
            retst = -ENOMEM;
            if (file)
                filp_close(file, 0); //close file

            if (*buffer)
            {
                kfree(*buffer); // free data buffer
                *buffer = NULL;
            }
        }
         //read until the end of the file.
        do
        {
            cursor = *position;
            retst = kernel_read(file, *buffer + cursor, size, position);
            if (retst < 0)
            {
                printk(KERN_ALERT "Unable to read file size: %s (%ld)", name, retst);
                filp_close(file, 0);
                kfree(*buffer);
            }
            else
            {
                // next allocated memmory
                *buffer = krealloc(*buffer, allocated + size, GFP_KERNEL);
                if (!*buffer)
                {
                    // return out of memory
                    retst = -ENOMEM;
                    if (file)
                        filp_close(file, 0);

                    if (*buffer)
                    {
                        kfree(*buffer);
                        *buffer = NULL;
                    }
                }
                allocated += size; // increase allocated size
            }
        } while (retst && allocated <= max_size);
        filp_close(file, 0); // success file close
    }
    else
    {
        // return file descriptor in bad state 
        retst = -EBADF;
        printk(KERN_ALERT "Unable to open file: %s (%ld)", name, retst);
    }
    return retst;
}

SYSCALL_DEFINE1(sysinfo_cpuinfo)
{
    void *buffer = NULL;
    int rcode = 0;
    loff_t size = 0LL;
    buffer = NULL;
    rcode = read_file(fcpuinfo, &buffer, max_size, &size);
    if (rcode >= 0 && buffer != NULL && size >= 0)
    {
        printk(KERN_ALERT "----------------CPU INFO------------------\n");
        dump_buffer((char *)buffer, 13,131);
        kfree(buffer);
    }

    return rcode;
}

SYSCALL_DEFINE1(sysinfo_statistics)
{
    void *buffer = NULL;
    int rcode = 0;
    loff_t size = 0LL;
    buffer = NULL;
    char * uptime;
    int i = 0;

    rcode = read_file(fuptime, &buffer, max_size, &size);
    if (rcode >= 0 && buffer != NULL && size >= 0)
    {
        printk(KERN_ALERT "----------------STATISTICS----------------\n");
        uptime = kzalloc(100,GFP_KERNEL);
        uptime = strsep((char **)&buffer," ");
        printk(KERN_INFO "System was booted since : %s\n", uptime);
        uptime = strsep((char **)&buffer," ");
        printk(KERN_INFO "System was idle since   : %s\n", uptime);
        kfree(buffer);
    }
    rcode = read_file(floadavg, &buffer, max_size, &size);
    if (rcode >= 0 && buffer != NULL && size >= 0)
    {
        for (i = 0; i < 5; i++)
        {
            if (i == 3)
            {
                printk(KERN_INFO "The number of active tasks / The total number of processes : %s\n", uptime);
            }
            uptime = strsep((char **)&buffer," ");
        }
        kfree(buffer);

    }
    return rcode;
}

SYSCALL_DEFINE2(sysinfo_pidstatus, int pid)
{
    void *buffer = NULL;
    int ccode = 0;
    loff_t size = 0LL;
    buffer = NULL;
    char *buffrpid;
    
    buffrpid = kzalloc(25, GFP_KERNEL);
    snprintf(buffrpid, 25, "/proc/%d/status", pid);
    ccode = read_file(buffrpid, &buffer, max_size, &size);

    if (ccode >= 0 && buffer != NULL && size >= 0)
    {
        printk(KERN_ALERT "----------------CPU INFO------------------\n");
        dump_buffer((char *)buffer, 13,131);
        kfree(buffer);
    }

    return ccode;
}
