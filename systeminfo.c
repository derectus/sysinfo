#include <sys/syscall.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{

    if(argc == 1){
	    long sys_info_cpuinfo = syscall(333);
        printf("CPUINFO returned : (%ld)\n",sys_info_cpuinfo);
    }
    if(argc == 2 && strcmp(argv[1], "-all") != NULL){
               
        long sys_info_statistics = syscall(334);
        printf("sys_info_statistics returned : (%ld)\n",sys_info_statistics);

        long sys_info_cpuinfo = syscall(333);
        printf("sys_info_cpuinfo returned : (%ld)\n",sys_info_cpuinfo);
    }
    if(argc == 3 && strcmp(argv[1], "-p") != NULL){
	 
            long sys_info_status = syscall(335 , argv[2]);
            printf("sys_info_cpuinfo returned : (%ld)\n",sys_info_status);
            
            long sys_info_cpuinfo = syscall(333);
            printf("sys_info_cpuinfo returned : (%ld)\n",sys_info_cpuinfo);
    }
    if (argc == 3 && strcmp(argv[1], "-all") != NULL && strcmp(argv[2], "-p") != NULL)
    {
        long sys_info_statistics = syscall(334);
        printf("sys_info_statistics returned : (%ld)\n",sys_info_statistics);

        long sys_info_status = syscall(335, argv[3]);
        printf("sys_info_cpuinfo returned : (%ld)\n",sys_info_status);
               
        long sys_info_cpuinfo = syscall(333);
        printf("sys_info_cpuinfo returned : (%ld)\n",sys_info_cpuinfo);
    }

    return 0;
}
