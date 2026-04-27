#include "SD.h"
#include <cstring>

// declare static members
SD* SD::instance = nullptr;
FATFS SD::fs;
FIL SD::fil;
FRESULT SD::fr;

SD& SD::getInstance() {
        if (instance == nullptr) {
            instance = new SD();
        }
        else {
            static SD instance;
        }
        return *instance;
}

SD::SD() {
    fil = {};
}

int SD::mount() {
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        return -1;
    }
    return 0;
}

int SD::openFile(const TCHAR* fileName) {
    // construct full path name
    TCHAR fullPath[150] = {"0:/"};
    strcat(fullPath, fileName);

    fr = f_open(&fil, fullPath, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        return -1;
    }
    return 0;
}

void SD::write(const char* data) {
    f_printf(&fil, data);
}

void SD::closeFile() {
    f_close(&fil);
    fil = {}; // reset file object
}

void SD::unmount() {
    f_unmount("0:");
}