#include <stdio.h>
#include <string.h>
#include "SD_card.h"
#include "fatfs.h"
struct SD_Iterface SD;
uint8_t fresult;
uint32_t size;
uint8_t mountSDcard(void){
	fresult = f_mount(&SD.fs, "/", 1);
	return fresult;
}


void unmountSDcard(void){
	fresult = f_mount(NULL, "/", 1);

}

void deleteFile(const TCHAR* path){
	fresult = f_unlink(path);

}


uint8_t writeDataPacked(const TCHAR* path,BYTE mode){
	fresult = f_open(&SD.fil, path, mode);
	fresult=f_lseek(&SD.fil, f_size(&SD.fil));
	fresult = f_write(&SD.fil, SD.buffer,bufsize(SD.buffer), &SD.bw);
	f_close(&SD.fil);
	bufclear();
	return fresult;
}



//FA_CREATE_ALWAYS | FA_WRITE|FA_READ    FA_OPEN_ALWAYS | FA_WRITE

uint8_t checkerror(){
	fresult = f_mount(&SD.fs, "/", 1);
	f_mount(NULL, "/", 1);
	return fresult;
}

void readData(const TCHAR* path,BYTE mode){
	 fresult = f_open(&SD.fil, path, mode);
	 fresult =  f_read (&SD.fil, SD.buffer, f_size(&SD.fil), &SD.br);
	 f_close(&SD.fil);
	 bufclear();

}



int bufsize(char *buff){

	int i=0;
	while(*buff++ !=  '\0') i++;
	return i;

}

void bufclear(void){
	for(int i=0;i<1024;i++){
		SD.buffer[i]='\0';
	}


}



/*
 *
 *   mountSDcard();
  bufclear();



 // writeDatafirst("file.txt", FA_CREATE_ALWAYS | FA_WRITE|FA_READ );
  //readData("file.txt", FA_OPEN_ALWAYS | FA_READ );
//HAL_Delay(300);
  strcpy (SD.buffer, "TEST1\n");
  writeDataPacked("file.txt", FA_CREATE_ALWAYS | FA_WRITE );
 // HAL_Delay(300);
  strcpy (SD.buffer, "TEST4\n");
    writeDataPacked("file.txt", FA_OPEN_ALWAYS | FA_WRITE );
  //  HAL_Delay(300);
    strcpy (SD.buffer, "11wadddddd22d\n");
    writeDataPacked("file.txt", FA_OPEN_ALWAYS | FA_WRITE );
        strcpy (SD.buffer, "2222wwadadwawd\n");
     //   HAL_Delay(300);
        writeDataPacked("file.txt", FA_OPEN_ALWAYS | FA_WRITE );
   unmountSDcard();
 *
 */







