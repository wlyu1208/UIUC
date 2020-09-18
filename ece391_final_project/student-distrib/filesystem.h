/* filesystem.h - header file for the implementation of kernel filesystem
 * vim:ts=4 noexpandtab */

#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "types.h"

#define BLOCK_SIZE              4096                /* num of bytes per block in filesystem */
#define NUM_DENTRY              63                  /* num of dentry structs in boot block */
#define NUM_DBLOCKS             1023                /* num of data block indexes in each inode */
#define MAX_CHARS               32                  /* max characters per file name */
#define NUM_STATS_RESERVED      52                  /* num bytes extra in the stats struct */
#define NUM_DENTRY_RESERVED     24                  /* num bytes extra in each dentry struct */

#define RTC_FILETYPE            0                   /* file type for RTC file */
#define DIR_FILETYPE            1                   /* file type for a directory file */
#define REG_FILETYPE            2                   /* file type for a regular file */
/* struct for filesystem statistics */
typedef struct {
    uint32_t num_dentry;
    uint32_t num_inode;
    uint32_t num_dblock;
    uint8_t reserved[NUM_STATS_RESERVED];
} stats_t;

/* struct for a dentry */
typedef struct {
    uint8_t file_name[MAX_CHARS];
    uint32_t file_type;
    uint32_t inode_num;
    uint8_t reserved[NUM_DENTRY_RESERVED];
} dentry_t;

/* struct consisting of the entire boot block */
typedef struct {
    stats_t fs_stats;
    dentry_t fs_dentry[NUM_DENTRY];
} boot_t;

/* struct for an inode */
typedef struct {
    uint32_t length;
    uint32_t dblock[NUM_DBLOCKS];
} inode_t;

/* starting address of filesystem */
uint32_t fs_start;

/* file read/write/close/open operations */
void fs_init();
int32_t file_read(int32_t fd, void* buf, int32_t nbytes);
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t file_close(int32_t fd);
int32_t file_open(const uint8_t* fname);
int32_t file_open_index(uint32_t index);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
/* dir read/write/close/open operations */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes);
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t dir_close(int32_t fd);
int32_t dir_open(const uint8_t* fname);
extern int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
boot_t* fs_boot;        /* address of boot block */
inode_t* fs_inode;      /* starting address of inodes */
uint32_t fs_dblock;     /* starting address of data blocks */
#endif
