#include "ff.h"
#include "sd_card.h"


/*
*   Singleton class to manage SD card operations using FatFS. 
*   Provides methods to mount the filesystem, open files, write data, and unmount the filesystem. 
*   Uses a single instance of FATFS and FIL structures to manage the filesystem and file operations.
*/
class SD {
public:
    static SD& getInstance();
    static int mount();
    static void unmount();
    static int openFile(const TCHAR* fileName);
    static void write(const char* data);
    static void closeFile();
    static FIL& newFile();

private:
    SD();
    SD(const SD&);
    static SD* instance;
    static FATFS fs;
    static FIL fil;
    static FRESULT fr;
};