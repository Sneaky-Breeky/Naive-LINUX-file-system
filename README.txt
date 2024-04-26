INTRO

The file systems is the portions of the OS that most users know best. File management is a basic skill in all
computer operations. UNIX uses a structure to store all file attributes known as the inode. It contains all
the information about a file including:
• File type: Indicates whether the inode represents a regular file, directory, symbolic link, or other type
of file.
• File permissions: Specifies who is allowed to read, write, or execute the file, and whether the permissions
apply to the owner, group, or other users.
• File owner: Indicates the user who owns the file.
• File group: Indicates the group to which the file belongs.
• File size: Specifies the size of the file in bytes.
• Timestamps: Records the time the file was last modified, accessed, or changed.
• Pointers to data blocks: Indicates the location of the data associated with the file, such as the actual
file contents or directory entries.

A directory is also a file with its own inode data. But the contents of the directory are only entries associating
a file name with an inode number.
In UNIX, all inodes for the system are created when the file system is installed. When a new file or directory
is created, it finds the next available inode and uses that to store the file data. There are many ways to store
the list of available inodes. One method is to use a bit vector, where each binary number 0 is a free inode
and 1 is an unavailable inode.

FUNCTIONALITY

1. whoami: Print the ID of the current user.
2. pwd: Print present working directory (also called the current working directory)
3. mkdir <directory name>: Create a directory of the given file name. No duplicate names can be used
in the same parent directory. A user must have write and execute permissions in the parent to create
a new directory.
4. mkfile <file name>: Create a regular file of the given file name. The user will be asked to enter the
text for the file next. No duplicate names can be used in the same parent directory. A user must have
write and execute permissions in the parent to create a new regular file.
5. ls: List all entries in the current working directory. The data printed for each entry is:
• Inode ID
• File permissions
• Owner ID
• Date created
• File name
A user must have execute permissions in the directory to list all entries.
6. su <user ID>: Change the currently active user to the new ID. Only a positive integer may be given
as user IDs. No passwords are needed to switch users.
7. cat <file name>: Print the contents of the regular file. (The file contents is stored in the the inode at
block ptr). First, we must search if the current working directory has a file name matching the given
name so that we can find the inode ID. Then once we have the ID and can find the pointer to the data,
we can print that data. The pointer in our inode is listed as the datatype void*. In C, the easiest way
to read this data as regular text is to create a char pointer to that address with:
char* data = (char*)inodes[file ID].block ptr;
A user must have read permissions for the file to read the data.
8. chmod <permissions> <file name>: Update the file permissions for the given file name. First, we
must search the current directory for a file matching the given name. Then using use inode, we can
update the mode attribute. The <permissions> entered must be an octal number. For instance 755
to indicate rwxr-xr-x permissions. A user must have write permissions for the file to change the file
permissions.
9. cd <absolute file path>: Change the current working directory to the path given. Must also update
the cwd variable with the inode ID of the new directory. Since this must be an absolute file path, you
should start with the root directory and follow the path to find the new directory given. A user must
have write and execute permissions to move into the new directory.
10. rm <file name>: Remove the file from the current directory. This must release the inode (there is a
helper function to do that) and then remove the entry from the parent directory. A user must have
write and execute permissions for the parent directory to remove the file. The permission of the file
do not matter.
11. rm -r <directory name>: Remove a directory and recursively remove all subdirectories and files. A
user must have write and execute permissions for the parent directory to remove the target directory.
This is a limited set of what a real file system can do, but enough to let us explore the complexly of these systems. To compare your solution with my sample solution, I have provided the executable file. It was compiled
on an AWS EC2 instance, thus should work on those systems.