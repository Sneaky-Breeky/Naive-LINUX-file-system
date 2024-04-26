// Author: Yunze Guan
// Date: 2023-04-03
// Description: main function
// Parameters: int argc, char **argv

#include "filesystems.h"

int main(int argc, char **argv)
{

	bitvector = 0;		   // set all inodes as free
	current_user = 0;	   // set user in as 0 (sudo user)
	strcpy(cwd_path, "/"); // set the path as root

	int root_id = make_dir_file(); // create the root directory
	cwd = root_id;				   // point cwd to root

	char buffer[MAX_COMMAND_LEN];

	printf("Welcome to Our Mini File System\n");

	// wait for user commands and parse appropriately
	while (1)
	{
		printf("> ");
		fgets(buffer, sizeof(buffer), stdin);
		// change \n to \0
		buffer[strcspn(buffer, "\n")] = '\0';

		if (strcmp(buffer, "") == 0)
		{
			printf("No command entered.\n");
		}
		else if (strncmp(buffer, "mkdir", 5) == 0)
		{
			mkdir(&buffer[6]);
		}
		else if (strncmp(buffer, "mkfile", 6) == 0)
		{
			mkfile(&buffer[7]);
		}
		else if (strncmp(buffer, "ls", 2) == 0)
		{
			ls();
		}
		else if (strncmp(buffer, "pwd", 3) == 0)
		{
			printf("%s\n", cwd_path);
		}
		else if (strncmp(buffer, "whoami", 6) == 0)
		{
			printf("User ID: %d\n", current_user);
		}

		else if (strncmp(buffer, "su", 2) == 0)
		{
			su(&buffer[3]);
		}
		else if (strncmp(buffer, "cat", 3) == 0)
		{
			cat(&buffer[4]);
		}
		else if (strncmp(buffer, "chmod", 5) == 0)
		{
			chmod(&buffer[6]);
		}
		else if (strncmp(buffer, "cd", 2) == 0)
		{
			if (buffer[3] == '\0')
			{
				printf("hint: cd <absolute_file_path>\n");
			}
			else
			{
				cd(root_id, &buffer[3]);
			}
		}
		else if (strncmp(buffer, "rm -r", 5) == 0)
		{
			rm_recursive(&buffer[6]);
		}
		else if (strncmp(buffer, "rm", 2) == 0)
		{
			rm(&buffer[3]);
		}
		else if (strncmp(buffer, "exit", 4) == 0)
		{
			break;
		}
		else
		{
			printf("Unknown command: %s\n", buffer);
		}
	}

	return 0;
}
