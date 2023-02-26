/* CSC360 P3: A SIMPLE FILE SYSTEM (SFS)
 * SARAH CLAPOFF, V00886385
 * MARCH 2022
 * please see assignment writeup for details and specifications. 
******THIS IS TESTING STUFF AHHHHH*
 
 */


#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h> //for ntohl
#include <unistd.h> //for close

/**Define structs: **/
// 1. Superblock:
struct __attribute__((__packed__)) superblock_t {
    uint8_t   fs_id [8];
    uint16_t block_size;
    uint32_t file_system_block_count;
    uint32_t fat_start_block;
    uint32_t fat_block_count;
    uint32_t root_dir_start_block;
    uint32_t root_dir_block_count;
};

//2. Time and Date entry:
struct __attribute__((__packed__)) dir_entry_timedate_t {
    uint16_t    year;
    uint8_t     month;
    uint8_t     day;
    uint8_t     hour;
    uint8_t     minute;
    uint8_t     second;
};

//3. Directory entry:
struct __attribute__((__packed__)) dir_entry_t {
    uint8_t     status;
    uint32_t    starting_block;
    uint32_t    block_count;
    uint32_t    size;
    struct dir_entry_timedate_t create_time;
    struct dir_entry_timedate_t modify_time;
    uint8_t     filename[31];
    uint8_t     unused[6];
};



void diskinfo(int argc, char* argv[]){
    //PART 1. See readme file for details.

    //open disk image file
    int fd = open(argv[1], O_RDWR); //NOTE: maybe have to change O_RDWR????
    
    //init and create buffer stat structure from file.
    struct stat buffer;
    int status = fstat(fd, &buffer); 
    /*NOTE FOR fSTAT: status=(0,1): int that fstat returns, indicates success/fail.
    *                  buffer=stat structure. contains all the fields, is modified(/"returned")
    *                  by fstat, hence why &buffer is passed in.
    */

    //map disk image file to the virtual address space of this process.
    void* address = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /*NOTE FOR mmap: returns address of new mapping
                    PROT_READ | PROT_WRITE: means pages may be read or written
                    MAP_SHARED: means updates to this mapping are visible to other processes mapping the same region.
    */

    struct superblock_t* sb;
    sb = (struct superblock_t*)address;
    printf("block count in struct: %d\n", htonl(sb->file_system_block_count));

    munmap(address, buffer.st_size);
    close(fd);
}

void disklist(int argc, char* argv[]){
    //PART 2
    printf("part 2\n");
}

void diskget(int argc, char* argv[]){
    //PART 3
    printf("part 3");
}

void diskput(int argc, char* argv[]){
    //PART 4
    printf("part 4");
}

void diskfix(int argc, char* argv[]){
    //PART 5
    printf("part 5");
}


int main(int argc, char* argv[]){
//this method directs the 5 parts to 
// different methods corresponding to the
// executables created from the MakeFile.
#if defined(PART1)
    diskinfo(argc, argv);
#elif defined(PART2)
    disklist(argc, argv);
#elif defined(PART3)
    diskget(argc, argv);
#elif defined(PART4)
    diskput(argc,argv);
#elif defined(PART5)
    diskfix(argc,argv);
#else
#   error "PART[12345] must be defined"
#endif
    return 0;
}