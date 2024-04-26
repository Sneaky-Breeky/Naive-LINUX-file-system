Name: Yunze Guan
Student #: 100383717
--------------------------------


Description:
	This is a mini file system.	
--------------------------------


Running my solution:
> su 100
Current user ID: 100
> whoami
User ID: 100
> su 0
Current user ID: 0
> mkdir d
> cd d
Current directory: d
> mkfile f1
Enter the contents of the file:
asdf
> ls
2       -rwxr-xr-x      0       07 Apr 05:48    f1
> mkdir d1
> mkdir d2
> mkdir d3
> mkfile f2
Enter the contents of the file:
laksdjf
> mkfile f3
Enter the contents of the file:
l;kasdjf;lk
> ls
2       -rwxr-xr-x      0       07 Apr 05:48    f1
3       drwxr-xr-x      0       07 Apr 05:48    d1
4       drwxr-xr-x      0       07 Apr 05:48    d2
5       drwxr-xr-x      0       07 Apr 05:48    d3
6       -rwxr-xr-x      0       07 Apr 05:48    f2
7       -rwxr-xr-x      0       07 Apr 05:48    f3
> chmod 0 f1
Current permission of f1 is ---------
> rm f1
Permission denied
> chmod 777 f1
Current permission of f1 is rwxrwxrwx
> rm f1
File: f1 has been removed
> ls
3       drwxr-xr-x      0       07 Apr 05:48    d1
4       drwxr-xr-x      0       07 Apr 05:48    d2
5       drwxr-xr-x      0       07 Apr 05:48    d3
6       -rwxr-xr-x      0       07 Apr 05:48    f2
7       -rwxr-xr-x      0       07 Apr 05:48    f3
> chmod 0 d1
Current permission of d1 is ---------
> chmod 0 f2
Current permission of f2 is ---------
> cd /
> rm -r d
Directory: d2 has been removed
Directory: d3 has been removed
File: f3 has been removed
> cd d
Current directory: d
> ls
3       d---------      0       07 Apr 05:48    d1
6       ----------      0       07 Apr 05:48    f2
> cat f2
Permission denied
> chmod 777 f2
Current permission of f2 is rwxrwxrwx
> cat f2
laksdjf



--------------------------------


Compiler & OS:

gcc10-gcc (GCC) 10.3.1 20210422 (Red Hat 10.3.1-1)
Linux version 5.10.162-141.675.amzn2.x86_64

as well as

gcc version 12.2.0 (x86_64-posix-sjlj-rev0, Built by MinGW-W64 project)
Windows 10 Family Edition
--------------------------------


Assumptions:
1. The format of directory can be either like "/home/dir" or "home/dir"
2. If you don't have the permission for one folder, then you have no permissions to any files or folders inside that folder.
--------------------------------


Errors/Bugs:
1. rm -r part is a bit messy but I think I works properly.
--------------------------------

