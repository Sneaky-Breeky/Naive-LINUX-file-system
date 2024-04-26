// Author: Yunze Guan
// Date: 2023-04-03
// Description: implementation of functions

#include "filesystems.h"

// HELPER FUNCTIONS

// Given an inode ID, return a string of the format the file
// permissions in the format of rwxrwxrwx
char *format_permissions(int inode_id)
{
	uint16_t octal_num = inodes[inode_id].mode;
	char perms[] = "rwx";	  // permission characters
	static char perm_str[10]; // permission string buffer

	for (int i = 0; i < 9; i++)
	{
		perm_str[i] = ((octal_num >> (8 - i)) & 1) ? perms[i % 3] : '-';
	}
	perm_str[9] = '\0'; // null terminate the string

	return perm_str;
}

// Returns the ID of the next available inode ID and
// marks it as "in-use" on the bit vector
int get_inode()
{
	int bit_index = -1;
	uint64_t mask = 1;
	for (int i = 0; i < 64; i++)
	{
		if ((bitvector & mask) == 0)
		{ // finds a 0 in bit vector
			bitvector |= mask;
			bit_index = i;
			break;
		}
		mask <<= 1;
	}
	return bit_index;
}

// Given an inode ID, free its data blocks and
// mark it as free in the bit vector
void release_inode(int inode_id)
{
	uint64_t mask = 1 << inode_id;
	bitvector &= ~mask;
	free(inodes[inode_id].block_ptr);
}

// Given an inode ID, return 1 if the current_user has
// permission to read the file. Otherwise 0.
int can_read(int inode_id)
{
	uint16_t file_owner_id = inodes[inode_id].uid;
	uint16_t file_permissions = inodes[inode_id].mode;
	int mask = 04;

	if (current_user == file_owner_id)
	{
		mask = 0400;
	}

	return (file_permissions & mask) != 0;
}

// Given an inode ID, return 1 if the current_user has
// permission to write to the file. Otherwise 0.
int can_write(int inode_id)
{
	uint16_t file_owner_id = inodes[inode_id].uid;
	uint16_t file_permissions = inodes[inode_id].mode;
	int mask = 02;

	if (current_user == file_owner_id)
	{
		mask = 0200;
	}

	return (file_permissions & mask) != 0;
}

// Given an inode ID, return 1 if the current_user has
// permission to execute the file. Otherwise 0.
int can_execute(int inode_id)
{
	uint16_t file_owner_id = inodes[inode_id].uid;
	uint16_t file_permissions = inodes[inode_id].mode;
	int mask = 01;

	if (current_user == file_owner_id)
	{
		mask = 0100;
	}

	return (file_permissions & mask) != 0;
}

// Create new empty directory and return its inode ID
int make_dir_file()
{
	int inode_id = get_inode();
	inodes[inode_id].file_type = DIRECTORY_FILETYPE;
	inodes[inode_id].mode = 0755;
	inodes[inode_id].uid = current_user;
	inodes[inode_id].atime = time(NULL);
	inodes[inode_id].block_ptr = malloc(sizeof(dir_entry_t) * MAX_ENTRIES);
	dir_entry_t *entres = (dir_entry_t *)inodes[inode_id].block_ptr;
	for (int i = 0; i < MAX_ENTRIES; i++)
	{
		entres[i].inode_num = -1;
	}
	return inode_id;
}

// Create new regular file and return its inode ID.
// File contents is passes as args and copied into
// the file's memory block
int make_regular_file(char *args)
{
	int inode_id = get_inode();
	inodes[inode_id].file_type = REGULAR_FILETYPE;
	inodes[inode_id].mode = 0755;
	inodes[inode_id].uid = current_user;
	inodes[inode_id].atime = time(NULL);
	inodes[inode_id].block_ptr = malloc(sizeof(char) * strlen(args) + 1);

	char *data = (char *)inodes[inode_id].block_ptr;
	strcpy(data, args);
	return inode_id;
}

// Add a new entry into the parent directory
void make_entry(int parent_dir, char *file_name, int inode_id)
{
	int empty_entry = -1;
	dir_entry_t *entres = (dir_entry_t *)inodes[parent_dir].block_ptr;
	for (int i = 0; i < MAX_ENTRIES; i++)
	{

		if (entres[i].inode_num == -1)
		{
			empty_entry = i;
			break;
		}
	}
	entres[empty_entry].inode_num = inode_id;
	strcpy(entres[empty_entry].name, file_name);
}

// Search the given directory for the given file name.
// Returns the position inside the directory
int search_dir(int dir_id, char *file_name)
{
	if (strlen(file_name) == 0)
	{
		return -1;
	}
	dir_entry_t *entres = (dir_entry_t *)inodes[dir_id].block_ptr;
	for (int i = 0; i < MAX_ENTRIES; i++)
	{
		if (entres[i].inode_num != -1 && strcmp(entres[i].name, file_name) == 0)
		{
			return i;
		}
	}
	return -1;
}

// Search the given directory for the given file name.
// Returns the inode ID
int search_dir_inode(int dir_id, char *file_name)
{
	int dir_entry_id = search_dir(dir_id, file_name);
	if (dir_entry_id >= 0)
	{
		dir_entry_t *entres = (dir_entry_t *)inodes[dir_id].block_ptr;
		return entres[dir_entry_id].inode_num;
	}

	return -1;
}

// Checks if a file name is invalid for the current working
// directory. Meaning the file name is taken or the file name
// is too short.
int invalid_name(int cwd, char *file_name)
{
	return strlen(file_name) == 0 || search_dir(cwd, file_name) >= 0;
}

// COMPLETED COMMANDS

// Make a new directory file in the current working directory
void mkdir(char *file_name)
{
	if (!can_write(cwd) || !can_execute(cwd))
	{
		printf("Invalid file permissions\n");
		return;
	}

	if (invalid_name(cwd, file_name))
	{
		printf("Invalid file name\n");
		return;
	}
	int id = make_dir_file();
	make_entry(cwd, file_name, id);
}

// Make a new regular file current working directory
void mkfile(char *name)
{
	if (!can_write(cwd) || !can_execute(cwd))
	{
		printf("Invalid file permissions\n");
		return;
	}

	if (invalid_name(cwd, name))
	{
		printf("Invalid file name\n");
		return;
	}
	printf("Enter the contents of the file: \n");
	char buffer[MAX_FILE_SIZE];
	fgets(buffer, sizeof(buffer), stdin);
	buffer[strcspn(buffer, "\n")] = '\0';
	int id = make_regular_file(buffer);
	make_entry(cwd, name, id);
}

// Print out the contents of the current working directory
void ls()
{
	if (!can_execute(cwd))
	{
		printf("Invalid file permissions\n");
		return;
	}

	dir_entry_t *entres = (dir_entry_t *)inodes[cwd].block_ptr;
	for (int i = 0; i < MAX_ENTRIES; i++)
	{
		if (entres[i].inode_num != -1)
		{
			int inode_id = entres[i].inode_num;
			printf("%d \t", inode_id);
			printf("%c", (inodes[inode_id].file_type == DIRECTORY_FILETYPE) ? 'd' : '-');
			printf("%s \t", format_permissions(inode_id));
			printf("%d \t", inodes[inode_id].uid);

			char buf[20];
			strftime(buf, sizeof(buf), "%d %b %H:%M", localtime(&inodes[inode_id].atime));
			printf("%s \t", buf);

			printf("%s \n", entres[i].name);
		}
	}
}

// TO BE COMPLETED

// New helper functions

bool has_letter(char *args)
{
	int count = 0;
	char tok = args[count];

	while (tok != '\0')
	{
		if (tok > ASCII_9 || tok < ASCII_0)
		{
			if (tok != ASCII_DASH)
				return true;
		}
		count++;
		tok = args[count];
	}

	return false;
}

bool is_octal(int perm)
{
	while (perm != 0)
	{
		int digit = perm % 10;

		if (digit >= 8)
		{
			return false;
		}

		perm /= 10;
	}

	return true;
}

void remove_from_dir(char *args, int inode)
{
	// find the index of the file inside file list of dir
	int dir_idx = search_dir(cwd, args);
	dir_entry_t *entres = (dir_entry_t *)inodes[cwd].block_ptr;

	// set the file to invalid in the directory
	if (entres[dir_idx].inode_num != -1)
	{
		entres[dir_idx].inode_num = -1;
	}
}

// Implemental Functions

// Change the current user ID to the given ID
void su(char *args)
{
	// your code here
	if (has_letter(args))
	{
		printf("Invalid user ID: %s\n", args);
	}
	else
	{
		int new_id = atoi(args);

		if (new_id < 0)
		{
			printf("Invalid user ID: %d\n", new_id);
		}
		else
		{
			current_user = new_id;
			printf("Current user ID: %d\n", current_user);
		}
	}
}

// Print the contents of the regular file
void cat(char *name)
{
	// your code here
	int inode = search_dir_inode(cwd, name);
	if (inode >= 0)
	{
		if (can_read(inode))
		{

			// get content
			char *data = (char *)inodes[inode].block_ptr;
			printf("%s\n", data);
		}
		else
		{
			printf("Permission denied\n");
		}
	}
	else
	{
		printf("No such file called: %s\n", name);
	}
}

// Update the file permissions
void chmod(char *args)
{
	// your code here
	char *tok = strtok(args, " ");

	if (tok != NULL && !has_letter(tok))
	{
		int perm = atoi(tok);
		char octal[MAX_COMMAND_LEN];
		strcpy(octal, tok);

		if (perm >= 0 && is_octal(perm))
		{
			// get file name
			tok = strtok(NULL, " ");

			if (tok != NULL)
			{
				// now we have tok = filename
				// get inode of the file
				int inode = search_dir_inode(cwd, tok);

				if (inode >= 0)
				{
					inodes[inode].mode = strtoul(octal, NULL, 8);

					char *perm_str = format_permissions(inode);
					printf("Current permission of %s is %s\n", tok, perm_str);
				}
				else
				{
					printf("Incorrect file name: %s\n", tok);
				}
			}
			else
			{
				printf("Invalid arguments\n");
			}
		}
		else
		{
			printf("Invalid arguments: %d is not an octal number!\n", perm);
		}
	}
	else
	{
		printf("Invalid arguments: %s\n", args);
	}
}

// Change the cwd and cwd_path.
// Note: new_dir_path must be an absolute path
void cd(int current_dir_id, char *new_dir_path)
{
	// your code here
	char dup_path[MAX_COMMAND_LEN];
	strcpy(dup_path, new_dir_path);

	char *tok = strtok(dup_path, "/");

	// change to root dir
	if (tok == NULL)
	{
		cwd = current_dir_id;
		strcpy(cwd_path, "/");
	}
	else
	{
		// start from root dir
		int dup_cwd = current_dir_id;

		while (tok != NULL)
		{
			if (!can_read(dup_cwd))
			{
				printf("Permission denied\n");
				break;
			}

			int inode = search_dir_inode(dup_cwd, tok);

			if (inode == -1)
			{
				printf("No such directory\n");
				break;
			}
			else if (inodes[inode].file_type != DIRECTORY_FILETYPE)
			{
				printf("Not a directory\n");
				break;
			}

			// update dup_cwd to the new directory
			dup_cwd = inode;

			tok = strtok(NULL, "/");

			// last dir
			if (tok == NULL)
			{
				// we update the parameters lastly when we're
				// done with scanning the path
				cwd = dup_cwd;
				strcpy(cwd_path, new_dir_path);

				printf("Current directory: %s\n", cwd_path);
			}
		}
	}
}

// Remove the regular file in the cwd
void rm(char *args)
{
	// your code here
	int inode = search_dir_inode(cwd, args);

	if (inode == -1)
	{
		printf("No such file\n");
	}
	else
	{
		if (!can_write(inode) || !can_execute(inode))
		{
			printf("Permission denied\n");
		}
		else
		{
			if (inodes[inode].file_type != REGULAR_FILETYPE)
			{
				printf("Not a regular file\nhint: use rm -r to remove a directory\n");
			}
			else
			{
				remove_from_dir(args, inode);

				release_inode(inode);
				printf("File: %s has been removed\n", args);
			}
		}
	}
}

// helper function of rm_recursive
bool is_empty(int curr_dir)
{
	dir_entry_t *entres = (dir_entry_t *)inodes[curr_dir].block_ptr;

	for (int i = 0; i < MAX_ENTRIES; i++)
	{
		if (entres[i].inode_num != -1)
		{
			return false;
		}
	}

	return true;
}

bool rm_recursive_h(int curr_dir)
{
	// if the current directory is empty, return
	if (is_empty(curr_dir))
	{
		return true;
	}

	if (!can_write(curr_dir) || !can_execute(curr_dir))
	{
		printf("Permission denied\n");
		return false;
	}

	dir_entry_t *entres = (dir_entry_t *)inodes[curr_dir].block_ptr;

	for (int i = 0; i < MAX_ENTRIES; i++)
	{
		int inode = entres[i].inode_num;

		if (inode != -1)
		{
			if (inodes[inode].file_type == REGULAR_FILETYPE)
			{
				if (can_write(inode) && can_execute(inode))
				{
					// remove regular file
					entres[i].inode_num = -1;

					release_inode(inode);
					printf("File: %s has been removed\n", entres[i].name);
				}
			}
			else
			{
				if (can_write(inode) && can_execute(inode))
				{
					// recursively remove the directory
					// if the dir is empty
					if (rm_recursive_h(inode))
					{
						entres[i].inode_num = -1;
						release_inode(inode);

						printf("Directory: %s has been removed\n", entres[i].name);
					}
				}
			}
		}
	}

	// if true, the dir won't be deleted
	return is_empty(curr_dir) ? true : false;
}

// implemental function of rm_recursive

// Remove the directory file in the cwd
void rm_recursive(char *args)
{
	// your code here
	int inode = search_dir_inode(cwd, args);

	if (inode == -1)
	{
		printf("No such directory\n");
	}
	else
	{
		if (!can_write(inode) || !can_execute(inode))
		{
			printf("Permission denied\n");
		}
		else
		{
			if (inodes[inode].file_type != DIRECTORY_FILETYPE)
			{
				printf("Not a directory\nhint: use rm to remove a file\n");
			}
			else
			{
				if (rm_recursive_h(inode))
				{

					remove_from_dir(args, inode);

					release_inode(inode);
					printf("Directory: %s has been removed\n", args);
				}
			}
		}
	}
}
