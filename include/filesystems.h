//Author: Yunze Guan
//Date: 2023-04-03
//Description: definitions and header files


#ifndef FILESYSTEMS_H
#define FILESYSTEMS_H

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_INODES 64           // The amount of inodes created with the file system
#define MAX_NAME_LEN 128        // Maximum length of a file name in bytes
#define MAX_PATH_LEN 256        // Maximum length of the absolute path
#define MAX_COMMAND_LEN 100     // Maximum length of a command line typed by the user
#define MAX_ENTRIES 64          // Maximum files in one directory
#define MAX_FILE_SIZE 265       // Maximum size of a regular file in bytes

#define REGULAR_FILETYPE 1
#define DIRECTORY_FILETYPE 2

#define ASCII_0 48
#define ASCII_8 56
#define ASCII_9 57
#define ASCII_DASH 45

// Define the inode structure
typedef struct {
    uint16_t file_type;         // The type of file (directory or regular file)
    uint16_t mode;              // File permissions
    uint16_t uid;               // Owner user ID
    time_t atime;               // Time of creation
    void* block_ptr;                // Pointers to data block
} inode_t;

// Define the directory entry structure
typedef struct {
    int inode_num;                      // Inode number of the file
    char name[MAX_NAME_LEN + 1];        // Name of the file
} dir_entry_t;


inode_t inodes[MAX_INODES];             // all inodes crated in the system
uint64_t bitvector;                         // bit vector to indicate free inodes


int current_user;                               // id of active user
char cwd_path[MAX_PATH_LEN];            // path of current working directory
int cwd;                                            // inode id of current working directory


// HELPER FUNCTIONS

// Given an inode ID, return a string of the format the file
// permissions in the format of rwxrwxrwx
char* format_permissions(int inode_id);

// Returns the ID of the next available inode ID and
// marks it as "in-use" on the bit vector
int get_inode();

// Given an inode ID, free its data blocks and
// mark it as free in the bit vector
void release_inode(int inode_id);

// Given an inode ID, return 1 if the current_user has
// permission to read the file. Otherwise 0.
int can_read(int inode_id);

// Given an inode ID, return 1 if the current_user has
// permission to write to the file. Otherwise 0.
int can_write(int inode_id);

// Given an inode ID, return 1 if the current_user has
// permission to execute the file. Otherwise 0.
int can_execute(int inode_id);

// Create new empty directory and return its inode ID
int make_dir_file();

// Create new regular file and return its inode ID.
// File contents is passes as args and copied into
// the file's memory block
int make_regular_file(char* args);

// Add a new entry into the parent directory
void make_entry(int parent_dir, char* file_name, int inode_id);

// Search the given directory for the given file name.
// Returns the position inside the directory
int search_dir(int dir_id, char* file_name);

// Search the given directory for the given file name.
// Returns the inode ID
int search_dir_inode(int dir_id, char* file_name);

// Checks if a file name is invalid for the current working
// directory. Meaning the file name is taken or the file name
// is too short.
int invalid_name(int cwd, char* file_name);



// COMPLETED COMMANDS

// Make a new directory file in the current working directory
void mkdir(char* name);

// Make a new regular file current working directory
void mkfile(char* name);

// Print out the contents of the current working directory
void ls();


// New helper functions

// check if the argument has letthers other than hyphen
bool has_letter(char *args);

// check whether a number is an octal number
bool is_octal(int perm);

// remove the file or dir inside the list of files of cwd
void remove_from_dir(char *args, int inode);

// helper function of rm_recursive
bool rm_recursive_h(int curr_dir);

// check whether the folder is empty
bool is_empty(int curr_dir);

// TO BE COMPLETED

// Change the current user ID to the given ID
void su(char* args);

// Print the contents of the regular file
void cat(char* name);

// Update the file permissions
void chmod(char* args);

// Change the cwd and cwd_path.
// Note: new_dir_path must be an absolute path
void cd(int current_dir_id, char* new_dir_path);

// Remove the regular file in the cwd
void rm(char* args);

// Remove the directory file in the cwd
void rm_recursive(char* args);


#endif