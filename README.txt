* CSC360 P1: A SIMPLE SHELL INTERPRETER (SSI)
* SARAH CLAPOFF, V00886385
* JANUARY 2022
* please see assignment writeup (p3.pdf) for details and specifications. 

MAKEFILE: Makefile 
    The Makefile for this program is executed through the command "make". 
    Using the descriptors PART1, PART2, PART3, and PART4 in correspondence with the
    #if directives in the main function of parts.c, specified portions of code will
    be compiled into seperate executables. That is, for the following executables:
        diskinfo is compiled from the function diskinfo(int argc, char* argv[]) in parts.c
        disklist is compiled from the function disklist(int argc, char* argv[]) in parts.c
        diskget is compiled from the function diskget(int argc, char* argv[]) in parts.c
        diskput is compiled from the function diskput(int argc, char* argv[]) in parts.c
        diskfix is compiled from the function diskfix(int argc, char* argv[]) in parts.c
    The command "make clean" will delete the executables created from "make".
    [Source: CSC360_tutorial8]

PROGRAM FILE: parts.c 
    This program performs the operations specified in parts I-V of the assignment description
    on the FAT file system specified in the assignment writeup (here referred to as test.dmg).
    
    The struct dir_entry_timedate_t represents the YYYYMMDDHHMMSS format used by the file system
    for create and modify times in a directory entry [4.2].
    The struct dir_entry_t represents a single directory entry, composed of the status, starting 
    block, number of blocks, file size (bytes), create time, modify time, file name, and unused bytes
    (set to 0xFF). Each directory entry is 64 bytes, and there are 8 directory entries per 512 byte block [4.2].
    The structs superblock_t represents the first block in the file system, which contains the following
    information: file system identifier, block size, file system size (blocks), block where FAT starts,
    number of blocks in FAT, block where root directory starts, and number of blocks in root directory [4.1].
    [Source: CSC360_tutorial8]

    As stated above, the main function evaluates one of diskinfo(), disklist(), diskget(), diskput(),
    or diskfix(), depending on the descriptor used in compilation, and then exits the program.
    If no -D flag is specified, an error is raised indicating that PART[12345] must be defined.

    FUNCTION: void diskinfo(argc, char* argv[])
        This function fufills the requirements specified in Part I of the assignment writeup [3.1].
        It can be executed through the command "./diskinfo test.img".
        It outputs the following information of the file system: block size, block count, block where 
        FAT starts, number of FAT blocks, block where root directory starts, number of root directory blocks,
        number of free blocks in FAT, number of reserved blocks in FAT, and number of allocated blocks in FAT.
        Superblock information is taken from the superblock, and FAT information is found through
        checking the first entry of every FAT block.

    FUNCTION: void disklist(int argc, char* argv[])
        This function fufills the requirements specified in Part II of the assignment writeup [3.2].
        It can be executed through the command "./disklist test.img".
        Since subdirectory requirement dropped, will always be checking in root directory.
        This function loops through each directory entry (8 entries/block * root_block_count)
        and sets the struct dir_entry equal to the current entry.
        Then the required information formatted and printed according to the assignment requirements.

    FUNCTION: diskget(int argc, chat* argv[])
        This function fufills the requirements specified in Part III of the assignment writeup [3.3].
        It can be executed through the command "./diskget test.img".
        1. convert filename to capitals and search filename in dir entries
        2. if match found, go to corresponding block in FAT tabel
        and copy until reach "FFFF FFFF" entry.
    
