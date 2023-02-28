#ifndef SD_CARD_H_
#define SD_CARD_H_
#include "fatfs.h"

#define BUFSIZE 4096
struct  SD_Iterface{
	FATFS fs;  // file system
	FIL fil; // File
	FRESULT fresult;  // result
	char buffer[BUFSIZE];//4kB data pack
	UINT br, bw;  // File read/write count

	/**** capacity related *****/
	FATFS *pfs;
	DWORD fre_clust;
	uint32_t total, free_space;
	uint8_t pcount; // number of sended packeds
};

uint8_t mountSDcard(void);
uint8_t writeDataPacked(const TCHAR* path,BYTE mode);
uint8_t checkerror(void);
void deleteFile(const TCHAR* path);
void readData(const TCHAR* path,BYTE mode);
void unmountSDcard(void);
int bufsize(char *buff);
void bufclear(void);

#endif /* SD_CARD_H_ */

