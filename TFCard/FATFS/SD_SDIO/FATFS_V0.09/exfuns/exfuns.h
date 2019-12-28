#ifndef __EXFUNS_H
#define __EXFUNS_H 	
#include "sys.h"		   
#include "ff.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////
extern FATFS *fs[3];  
extern FIL *file1;	 
extern FIL *file2;	 
extern UINT br,bw;
extern FILINFO fileinfo;
extern DIR dir;
extern FRESULT res;		
extern u8 *fatbuf;//SD�����ݻ�����


//f_typetell���ص����Ͷ���
//���ݱ�FILE_TYPE_TBL���.��exfuns.c���涨��
#define T_BIN		0X00	//bin�ļ�
#define T_LRC		0X10	//lrc�ļ�
#define T_NES		0X20	//nes�ļ�
#define T_TEXT		0X30	//.txt�ļ�
#define T_C			0X31	//.c�ļ�
#define T_H			0X32    //.h�ļ�
#define T_FLAC		0X4C	//flac�ļ�
#define T_BMP		0X50	//bmp�ļ�
#define T_JPG		0X51	//jpg�ļ�
#define T_JPEG		0X52	//jpeg�ļ�		 
#define T_GIF		0X53	//gif�ļ�  

 
u8 exfuns_init(void);		//�����ڴ�
u8 f_typetell(u8 *fname);	//ʶ���ļ�����
FRESULT exf_getfree(char *drv,u32 *total,u32 *free);//�õ�������������ʣ������
FRESULT f_scan_files (char* path ) ;   // Start node to be scanned (also used as work area) 
//u32 exf_fdsize(u8 *fdname);																				   //�õ��ļ��д�С
//u8* exf_get_src_dname(u8* dpfn);																		   
//u8 exf_copy(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8 *psrc,u8 *pdst,u32 totsize,u32 cpdsize,u8 fwmode);	   //�ļ�����
//u8 exf_fdcopy(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8 *psrc,u8 *pdst,u32 *totsize,u32 *cpdsize,u8 fwmode);//�ļ��и���

#endif

