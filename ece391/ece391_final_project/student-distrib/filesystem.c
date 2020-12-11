/* filesystem.c - implementation of kernel filesystem
* read-only 
 * vim:ts=4 noexpandtab */

#include "filesystem.h"
#include "syscall.h"
#include "types.h"
#include "lib.h"

/* check if filesystem has been initialized yet */
uint32_t fs_init_status = 0;

/* local helper functions */
void fs_init();
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

/* fs_init
 * description: initializes all the local variables and states
 * input: none
 * output: none
 * side effect: sets local variables and states to addresses based off of fs_start 
 */
void fs_init() {
    /* filesystem addresses */
    fs_boot = (boot_t*)fs_start;
    fs_inode = (inode_t*)(fs_start + BLOCK_SIZE);
    fs_dblock = (uint32_t)fs_inode + (fs_boot->fs_stats.num_inode) * BLOCK_SIZE;

    /* set status */
    fs_init_status = 1;
}

/* read_dentry_by_name
 * description: helper function, copies specific dentry by name to buffer
 * input:   fname - char array representing file name to copy
 *          dentry - dentry pointer to a buffer
 * output: 0 on success, -1 on failure
 * side effect: copies dentry information to dentry buffer
 */
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) {
    uint32_t i;

    for (i = 0; i < NUM_DENTRY; i++) {
        uint32_t dentry_name_length = strlen((int8_t*)fs_boot->fs_dentry[i].file_name);
        uint32_t fname_length = strlen((int8_t*)fname);
        /* if strlen returns longer than max chars, set to max chars */
        if (dentry_name_length > MAX_CHARS) dentry_name_length = MAX_CHARS;
        if (fname_length > MAX_CHARS) fname_length = MAX_CHARS;
        /* first check length, then check each character */
        if (dentry_name_length == fname_length && 
        strncmp((int8_t*)fs_boot->fs_dentry[i].file_name, (int8_t*)fname, fname_length) == 0) {
            /* if match, copy dentry information to dentry buffer */
            strncpy((int8_t*)dentry->file_name, (int8_t*)fs_boot->fs_dentry[i].file_name, fname_length);
            dentry->file_type = fs_boot->fs_dentry[i].file_type;
            dentry->inode_num = fs_boot->fs_dentry[i].inode_num;
            memcpy(dentry->reserved, fs_boot->fs_dentry[i].reserved, NUM_DENTRY_RESERVED);

            return 0;
        }
    }
    /* didn't find it */
    return -1;
}

/* read_dentry_by_index
 * description: helper function, copies specific dentry by index to buffer
 * input:   index - index into dentry to copy
 *          dentry - dentry pointer to a buffer
 * output: 0 on success, -1 on failure
 * side effect: copies dentry information to dentry buffer
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry) {
    uint32_t dentry_name_length;
    /* check if index is valid */
    if (index >= NUM_DENTRY) return -1;
    /* get dentry name length */
    dentry_name_length = strlen((int8_t*)fs_boot->fs_dentry[index].file_name);
    if (dentry_name_length > MAX_CHARS) dentry_name_length = MAX_CHARS;

    /* copy dentry information to dentry buffer */
    strncpy((int8_t*)dentry->file_name, (int8_t*)fs_boot->fs_dentry[index].file_name, dentry_name_length);
    dentry->file_type = fs_boot->fs_dentry[index].file_type;
    dentry->inode_num = fs_boot->fs_dentry[index].inode_num;
    memcpy(dentry->reserved, fs_boot->fs_dentry[index].reserved, NUM_DENTRY_RESERVED);

    return 0;
}

/* read_data
 * description: helper function, copies length number of bytes from filesystem to buffer
 * input:   inode - inode index of file
 *          offset - starting address of datablock to copy to buffer
 *          buf - pointer to a buffer to copy data to
 *          length - number of bytes to copy to buffer
 * output: number of bytes successfully copied, -1 if failure
 * side effect: copies file information to buffer
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    uint32_t dblock_index, nbytes_read;
    /* check for invalid parameters */
    if (inode >= fs_boot->fs_stats.num_inode) return -1;
    if (offset >= fs_inode[inode].length) return 0;

    /* make sure not to overread data */
    if ((length + offset) >= fs_inode[inode].length) length = fs_inode[inode].length - offset;

    /* find correct block to start at */
    dblock_index = offset / BLOCK_SIZE;
    nbytes_read = 0;
    while (nbytes_read < length) {
        uint8_t* dblock_addr;
        uint32_t nbytes_to_read;
        /* check if valid block to read */
        if (fs_inode[inode].dblock[dblock_index] >= fs_boot->fs_stats.num_dblock) return -1;        
        /* calculate starting dblock addr to copy from */
        dblock_addr = (uint8_t*)(fs_dblock + fs_inode[inode].dblock[dblock_index] * BLOCK_SIZE);
        nbytes_to_read = BLOCK_SIZE;
        /* check if reading starts in middle of block and adjust number of bytes to read */
        if ((offset - nbytes_read) % BLOCK_SIZE != 0) {
            nbytes_to_read = BLOCK_SIZE - (offset % BLOCK_SIZE);
            dblock_addr += BLOCK_SIZE - nbytes_to_read;            
        }
        /* check to see if reaching length */
        if ((nbytes_read + nbytes_to_read > length)) nbytes_to_read = length - nbytes_read;

        /* copy block into memory */
        memcpy(buf, dblock_addr, nbytes_to_read);

        /* increment positions to read from next dblock */
        buf += nbytes_to_read;
        nbytes_read += nbytes_to_read;    
        dblock_index++;    
    }
    return (int32_t)nbytes_read;
}

/* file_read
 * description: copies length number of bytes from filesystem to buffer
 * input:   fd - file descriptor
 *          buf - pointer to a buffer to copy data to
 *          nbytes - number of bytes to copy to buffer
 * output: number of bytes successfully copied, -1 if failure
 * side effect: copies file information to buffer
 */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes) {
    uint32_t nbytes_read;
    pcb_t* curr_pcb = get_current_pcb();
    /* check for invalid state and parameters */
    if (curr_pcb == NULL || buf == NULL || fd < MIN_FILESYSTEM_FD || fd >= MAX_FILES_NUMBER ||
    curr_pcb->file_descriptor_array[fd].flags_in_use == FLAG_NOT_IN_USE) return -1;
    /* call helper function, use f_read as the offset - important for multiple copies */
    nbytes_read = read_data(curr_pcb->file_descriptor_array[fd].inode_index,
    curr_pcb->file_descriptor_array[fd].file_position, buf, nbytes);
    curr_pcb->file_descriptor_array[fd].file_position += nbytes_read;
    return nbytes_read;
}

/* file_write
 * description: copies length number of bytes from buffer to filesystem
 * input:   fd - file descriptor (unused for now)
 *          buf - pointer to a buffer to copy data from
 *          nbytes - number of bytes to copy from buffer
 * output: number of bytes successfully copied, -1 if failure
 * side effect: copies file information to filesystem from buffer
 */
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes) {
    /* read-only filesystem */
    return -1;
}

/* file_close
 * description: signal that file reading is done, allowing another to be opened
 * input:   fd - file descriptor (unused for now)
 * output: 0 if closed, -1 if failure
 * side effect: sets f_open flag to be 0
 */
int32_t file_close(int32_t fd) {
    pcb_t* curr_pcb = get_current_pcb();
    /* if invalid or if not currently open return -1 */
    if (curr_pcb == NULL || fd < MIN_FILESYSTEM_FD || fd >= MAX_FILES_NUMBER ||
    curr_pcb->file_descriptor_array[fd].flags_in_use == FLAG_NOT_IN_USE) return -1;

    /* set as closed */
    curr_pcb->file_descriptor_array[fd].flags_in_use = FLAG_NOT_IN_USE;
    return 0;
}

/* file_open
 * description: open file by name and load its dentry
 * input:   fname - char array representing file name to print out
 * output: inode_num if opened, -1 if failure
 * side effect: none
 */
int32_t file_open(const uint8_t* fname) {
    dentry_t curr_dentry;
    /* check for invalid parameters */
    if (fname == NULL) return -1;
    /* initialize fs if needed */
    if (!fs_init_status) fs_init();
    /* if failed to open dentry return -1 */
    if (read_dentry_by_name(fname, &curr_dentry) == -1 || curr_dentry.file_type != REG_FILETYPE) return -1;

    return curr_dentry.inode_num;
}

/* file_open_index
 * description: open file by index and load its dentry, for testing
 * input:   index - index of file to open
 * output: inode_num if opened, -1 if failure
 * side effect: none
 */
int32_t file_open_index(uint32_t index) {
    dentry_t curr_dentry;
    /* initialize fs if needed */
    if (!fs_init_status) fs_init();
    /* if failed to open dentry or wrong filetype return -1 */
    if (read_dentry_by_index(index, &curr_dentry) == -1 || curr_dentry.file_type != REG_FILETYPE) return -1;
    /* success! */
    return curr_dentry.inode_num;
}

/* dir_read
 * description: copies length number of bytes of a filename to buffer
 * input:   fd - file descriptor
 *          buf - pointer to a buffer to copy data to
 *          nbytes - number of bytes to copy to buffer
 * output: number of bytes successfully copied, -1 if failure
 * side effect: copies file information to buffer
 */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes) {    
    uint32_t fname_length, d_read;    
    pcb_t* curr_pcb = get_current_pcb();
    /* check for invalid parameters */
    if (curr_pcb == NULL || buf == NULL || fd < MIN_FILESYSTEM_FD || fd >= MAX_FILES_NUMBER ||
    curr_pcb->file_descriptor_array[fd].flags_in_use == FLAG_NOT_IN_USE) return -1;
    d_read = curr_pcb->file_descriptor_array[fd].file_position;
    /* if read all in directory, reset current file position and return 0 */
    if (d_read >= fs_boot->fs_stats.num_dentry) {
        curr_pcb->file_descriptor_array[fd].file_position = 0;
        return 0;
    }
    /* check length of filename to copy to buffer */
    fname_length = strlen((int8_t*)fs_boot->fs_dentry[d_read].file_name);
    if (fname_length > nbytes) fname_length = nbytes;
    /* copy file name to buffer */
    strncpy((int8_t*)buf, (int8_t*)fs_boot->fs_dentry[d_read].file_name, fname_length);
    curr_pcb->file_descriptor_array[fd].file_position++;
    return (int32_t)fname_length;
}

/* dir_write
 * description: copies length number of bytes from buffer to directory
 * input:   fd - file descriptor (unused for now)
 *          buf - pointer to a buffer to copy data from
 *          nbytes - number of bytes to copy from buffer
 * output: number of bytes successfully copied, -1 if failure
 * side effect: copies file information to directory from buffer
 */
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes) {
    /* read-only filesystem */
    return -1;
}

/* dir_close
 * description: signal that directory reading is done, allowing another to be opened (unused)
 * input:   fd - file descriptor (unused for now)
 * output: 0 if closed, -1 if failure
 * side effect: none
 */
int32_t dir_close(int32_t fd) {
    return 0;
}

/* dir_open
 * description: open director by name and load its dentry
 * input:   fname - char array representing directory name to print out
 * output: inode_num if opened, -1 if failure
 * side effect: none
 */
int32_t dir_open(const uint8_t* fname) {
    dentry_t curr_dentry;
    /* check for invalid parameters */
    if (fname == NULL) return -1;
    /* initialize fs if needed */
    if (!fs_init_status) fs_init();
    /* if failed to open dentry/directory file return -1 */
    if (read_dentry_by_name(fname, &curr_dentry) == -1 || curr_dentry.file_type != DIR_FILETYPE) return -1;
    /* success! */
    return curr_dentry.inode_num;
}


