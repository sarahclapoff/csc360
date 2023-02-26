#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

struct __attribute__((__packed__)) superblock_t {
    uint8_t   fs_id [8];
    uint16_t block_size;
    uint32_t file_system_block_count;
    uint32_t fat_start_block;
    uint32_t fat_block_count;
    uint32_t root_dir_start_block;
    uint32_t root_dir_block_count;
};

void main(int argc, char* argv[]) {

    int fd = open(argv[1], O_RDWR);
    struct stat buffer;
    int status = fstat(fd, &buffer);

    void* address=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    struct superblock_t* sb;
    sb=(struct superblock_t*)address;
    printf("fs_id in struct: %s\n", sb->fs_id);
    printf("block count in struct: %d\n", htonl(sb->file_system_block_count));

    char name[8];
    memcpy(name,address, 8);
    printf("fs_id: %s\n",name);

    int fssize;
    memcpy(&fssize, address+10, 4);
    //format: memcpy(&placement, address+numbytes, bytesize (in bits))
    //^ copies what is stored IN space address+10*4 TO variable fssize.
    fssize=htonl(fssize);
    printf("block count: %d\n",fssize);
    
    munmap(address,buffer.st_size);
    close(fd);
}
