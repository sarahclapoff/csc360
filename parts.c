/* CSC360 P3: A SIMPLE FILE SYSTEM (SFS)
 * SARAH CLAPOFF, V00886385
 * MARCH 2022
 * please see assignment writeup for details and specifications. 
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


//#include <limits.h>
//#include <assert.h>



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

//1. get superblock info from disk image:
    //open disk image file and check it exists.
    if (access(argv[1], F_OK) != 0){
        printf("File does not exist\n");
        return;
    }
    int fd = open(argv[1], O_RDWR);
    
    //init and create buffer stat structure from file.
    struct stat buffer;
    int status = fstat(fd, &buffer); 
    //use status??????
    status ++;
    /*NOTE FOR fSTAT: status=(0,1): int that fstat returns, indicates success/fail.
                      buffer=stat structure. contains all the fields, is modified(/"returned")
                      by fstat, hence why &buffer is passed in.
    */
    //map disk image file to the virtual address space of this process.
    void* address = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /*NOTE FOR mmap: maps files or devices into memory 
                    returns address of new mapping
                    PROT_READ | PROT_WRITE: means pages may be read or written
                    MAP_SHARED: means updates to this mapping are visible to other processes mapping the same region.
    */
    //create superblock struct for the mapping
    struct superblock_t* sb;
    sb = (struct superblock_t*)address;

//2. get FAT table info
    int fat_val;
    int avail = 0;
    int reserve = 0;
    int allo = 0;

    int start = 512*htonl(sb->fat_start_block); //address that FAT starts at

    //USE THIS TO GET A LIL CHUNK OF FAT:
    //for(int i=0; i<128; i++){
    for(int i=0; i<htonl(sb->fat_block_count)*128; i++){
        memcpy(&fat_val, address+start+(4*i), 4);
        //^ format: memcpy(&placement, address+numbytes, bytesize (in bits (maybe not in bits idk man)))
        if(htonl(fat_val) == 0){
            avail ++;
        } else if(htonl(fat_val) == 1){
            reserve ++;
        } else{
            allo++;
        }
    }
//3. print the shtuff (in decimal):
    printf("Super block information:\n");
    /*NOTE ON htonl/ntohl: CHANGE BYTE ORDER (big/little endian)
        htonl: host to network long (uint32_t)
        ntohl: network to host long
        htons: host to network short (uint16_t)
        ntohs: network to host short
        network expected to be BIG ENDIAN
        disk image file system using BIG ENDIAN
        intel (my computer / host) uses LITTLE ENDIAN
        SO in this case want host to network
    */
    printf("Block size: %d\n", htons(sb->block_size)); //block size is 2B so htons
    printf("Block count: %d\n", htonl(sb->file_system_block_count));
    printf("FAT starts: %d\n", htonl(sb->fat_start_block));
    printf("FAT blocks: %d\n", htonl(sb->fat_block_count));
    printf("Root directory start: %d\n", htonl(sb->root_dir_start_block));
    printf("Root directory blocks: %d\n\n", htonl(sb->root_dir_block_count));
    printf("FAT information:\n");
    printf("Free Blocks: %d\n", avail);
    printf("Reserved Blocks: %d\n", reserve);
    printf("Allocated Blocks: %d\n", allo);
    //close everything at the end
    munmap(address, buffer.st_size);
    close(fd);

    
}

void disklist(int argc, char* argv[]){
    //PART 2. See readme file for info.
    
//1. get info from disk image:
    //open disk image file and check it exists.
    if (access(argv[1], F_OK) != 0){
        printf("File does not exist\n");
        return;
    }
    int fd = open(argv[1], O_RDWR);
    
    //init and create buffer stat structure from file.
    struct stat buffer;
    int status = fstat(fd, &buffer); 
    //use status??????
    status = 0;
    //map disk image file to the virtual address space of this process.
    void* address = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    //create superblock struct for the mapping
    struct superblock_t* sb;
    sb = (struct superblock_t*)address;

    //get root directory start point:
    int rootstart = 512*htonl(sb->root_dir_start_block); //address that FAT starts at


//2. get file info from root directory
    //dir listing info:
    char* filestatus;
    
    for(int i=0; i<htonl(sb->root_dir_block_count)*8; i++){ //since 8 entries/block
        struct dir_entry_t* dir_entry;
        dir_entry = (struct dir_entry_t*)(address+rootstart+(i*64));
        if(dir_entry->status == 0){
            continue;
        }
        if(dir_entry->status == 3){ //aka bit 1=1, bit2=1, bit3=0
            filestatus="F";
        }
        else if(dir_entry->status == 9){ //aka bit1=1, bit2=0, bit3=1
            filestatus="D";
        }
        //format print for directory entry
        printf("%c %10d %30s %d/%02d/%02d %02d:%02d:%02d\n", *filestatus, htonl(dir_entry->size), dir_entry->filename, htons(dir_entry->create_time.year), dir_entry->create_time.month, dir_entry->create_time.day, dir_entry->create_time.hour, dir_entry->create_time.minute, dir_entry->create_time.second);
    }
    
    }

void diskget(int argc, char* argv[]){
    //PART 3. See readme file for info.
    
//1. get info from disk image:
    //open disk image file and check it exists.
    if (access(argv[1], F_OK) != 0){
        printf("File does not exist\n");
        return;
    }
    int fd = open(argv[1], O_RDWR);
    
    //init and create buffer stat structure from file.
    struct stat buffer;
    int status = fstat(fd, &buffer); 
    //use status??????
    status ++;
    //map disk image file to the virtual address space of this process.
    void* address = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    //create superblock struct for the mapping
    struct superblock_t* sb;
    sb = (struct superblock_t*)address;

    int rootstart = 512*htonl(sb->root_dir_start_block); //address that FAT starts at

    //filename = argv[3] since root dir is '/'
    //search all root dir filenames
    for(int i=0; i<htonl(sb->root_dir_block_count)*8; i++){ //since 8 entries/block
        struct dir_entry_t* dir_entry;
        //printf("%s", argv[3]);
        dir_entry = (struct dir_entry_t*)(address+rootstart+(i*64));
        /*char inputfile[strlen(argv[3])];
        int j=0;
        char t;
        while(argv[3][j]){
            t = argv[3][j];
            inputfile[j]=t;
            t++;
        }
        */
        printf("%s", inputfile);
        if(memcmp(dir_entry->filename, argv[3], strlen(argv[3]))==0){
            printf("%s\n",argv[3]);
        }
    }
}

void diskput(int argc, char* argv[]){
    //PART 4. See readme file for info.
    
//1. get info from disk image:
    //open disk image file and check it exists.
    if (access(argv[1], F_OK) != 0){
        printf("File does not exist\n");
        return;
    }
    int fd = open(argv[1], O_RDWR);
    
    //init and create buffer stat structure from file.
    struct stat buffer;
    int status = fstat(fd, &buffer); 
    //use status??????
    status ++;
    //map disk image file to the virtual address space of this process.
    void* address = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    //create superblock struct for the mapping
    struct superblock_t* sb;
    sb = (struct superblock_t*)address;
    
    
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