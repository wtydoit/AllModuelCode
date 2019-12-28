/***************************�������ӿƼ����޹�˾****************************
**  �������ƣ�YS-V0.4����ʶ�𿪷������
**	CPU: STM32f103RCT6
**	����8MHZ
**	�����ʣ�9600 bit/S
**	���ײ�Ʒ��Ϣ��YS-V0.4����ʶ�𿪷���
**                http://yuesheng001.taobao.com
**  ���ߣ�zdings
**  ��ϵ��751956552@qq.com
**  �޸����ڣ�2012.4.11
**  ˵���������� �߱�����ʶ�𡢴���ͨ�š����������������ʾ��
***************************�������ӿƼ����޹�˾******************************/
 #include "bsp_LDchip.h"
#include "bsp_RegRW.h"

#include <stdio.h>
/*************�˿���Ϣ********************
 * ����˵��
 * RST      PB6
 * CS   	PB8
 * WR/SPIS  PB12
 * P2/SDCK  PB13
 * P1/SDO   PB14
 * P0/SDI   PB15
 * IRQ      PC1
 * A0				PB7
 * RD       PA0
*****************************************/



/************************************************************************************
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_NONE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUNDOK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUNDZERO:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
*********************************************************************************/
uint8 nAsrStatus=0;	

void LD3320_Init(void);

uint8 RunASR(void);
void ProcessInt0(void);
void LD3320_EXTI_Cfg(void);
void LD3320_Spi_cfg(void);
void LD3320_GPIO_Cfg(void);
void LED_gpio_cfg(void);

//test1�鿴PIN29 30�Ƿ�����͵�ƽ

//test2��д�Ĵ�������
void test2(void)
{
	LD_reset();
	LD_ReadReg(0x6);
	LD_WriteReg(0x35, 0x33);
	LD_WriteReg(0x1b, 0x55);
	LD_WriteReg(0xb3, 0xaa);
	printf("1:%x\r\n",LD_ReadReg(0x35));
	printf("2:%x\r\n",LD_ReadReg(0x1b));
	printf("3:%x\r\n",LD_ReadReg(0xb3));
}

//test3���Ĵ�����ʼֵ
void test3(void)
{
	LD_reset();
	printf("00|87 == %x \r\n",LD_ReadReg(0x06));
	printf("87 == %x \r\n",LD_ReadReg(0x06));
	printf("80 == %x \r\n",LD_ReadReg(0x35));
	printf("FF == %x \r\n",LD_ReadReg(0xb3));
}

/***********************************************************
* ��    �ƣ� LD3320_main(void)
* ��    �ܣ� ������LD3320�������
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 

void  LD3320_main(void)
{
	uint8 nAsrRes=0;
	LD3320_Init();	  
  printf(" ����������....\r\n"); /*text.....*/
	printf("���1����ˮ��\r\n"); /*text.....*/
	printf("2����˸\r\n"); /*text.....*/
	printf("3����������\r\n"); /*text.....*/
	printf("4��ȫ��\r\n"); /*text.....*/

	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR
	test2();
	test3();
	while(1)
	{

 	switch(nAsrStatus)
		{
			case LD_ASR_RUNING://printf("running\n");
			case LD_ASR_ERROR://printf("error\n");		
												break;
			case LD_ASR_NONE:printf("none\n");
											nAsrStatus=LD_ASR_RUNING;
											if (RunASR()==0)	//	����һ��ASRʶ�����̣�ASR��ʼ����ASR���ӹؼ��������ASR����
											{		
												nAsrStatus = LD_ASR_ERROR;
											}
											break;

			case LD_ASR_FOUNDOK:printf("found one\n");
											 nAsrRes = LD_GetResult( );	//	һ��ASRʶ�����̽�����ȥȡASRʶ����										 
												printf("\r\nʶ����:");			 /*text.....*/
								        //USART_SendData(USART1,nAsrRes+0x30); /*text.....*/		
												printf("\r\nʶ����:%d\r\n",nAsrRes);			
												 switch(nAsrRes)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
												  {
													  case CODE_QM:			/*�����ˮ�ơ�*/
															printf("��ȫ������ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_NHSA:	 /*�����˸��*/
															printf("�����ɧ��������ʶ��ɹ�\r\n"); /*text.....*/
																						 break;
														case CODE_KD:		/*�������������*/
												
															printf("�����ơ�����ʶ��ɹ�\r\n"); /*text.....*/
																						break;
														case CODE_MJ:		/*���ȫ��*/
												
															printf("��ĸ��������ʶ��ɹ�\r\n"); /*text.....*/
																						break;
														default:break;
													}	
												nAsrStatus = LD_ASR_NONE;
											break;
			
			case LD_ASR_FOUNDZERO:
			default:
								nAsrStatus = LD_ASR_NONE;
								break;
			}//switch

	}// while

}
/***********************************************************
* ��    �ƣ�LD3320_Init(void)
* ��    �ܣ�ģ�������˿ڳ�ʼ����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_Init(void)
{
	LD3320_GPIO_Cfg();	
	LD3320_EXTI_Cfg();
	LD3320_Spi_cfg();	 
	LED_gpio_cfg();

	LD_reset();
}

/***********************************************************
* ��    �ƣ� void Delay_( int i)
* ��    �ܣ� ����ʱ
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void Delay_( int i)
 {   
while(i--);
 }	
/***********************************************************
* ��    �ƣ�	LD3320_delay(unsigned long uldata)
* ��    �ܣ�	����ʱ����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
 void  LD3320_delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	for (j=0;j<5;j++)
	{
		for (g=0;g<uldata;g++)
		{
			Delay_(150);
		}
	}
}

/***********************************************************
* ��    �ƣ�	RunASR(void)
* ��    �ܣ�	����ASR
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	for (i=0; i<5; i++)			//	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
	{
		LD_AsrStart();			//��ʼ��ASR
		LD3320_delay(100);
		if (LD_AsrAddFixed()==0)	//���ӹؼ����ﵽLD3320оƬ��
		{
			LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			LD3320_delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}

		LD3320_delay(10);

		if (LD_AsrRun() == 0)
		{
			LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			LD3320_delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}

		asrflag=1;
		break;					//	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
	}

	return asrflag;
}

/***********************************************************
* ��    �ƣ�LD3320_GPIO_Cfg(void)
* ��    �ܣ���ʼ����Ҫ�õ���IO��
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_GPIO_Cfg(void)
{	
	    GPIO_InitTypeDef GPIO_InitStructure;
		// ����PA8 ���	 8M ����	
		{	
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	    /*    MCO    configure */
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOA,&GPIO_InitStructure);	
	    RCC_MCOConfig( RCC_MCO_HSE);		//8M
		}
	 //io������
	 {
	 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB ,ENABLE);
		//LD_CS	/RSET
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;//;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
//		//LD_RD
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_7);	/*A0Ĭ������*/
		GPIO_SetBits(GPIOA,GPIO_Pin_0);	/*RDĬ������*/
	 }
}
/***********************************************************
* ��    �ƣ�LD3320_Spi_cfg(void)
* ��    �ܣ�����SPI���ܺͶ˿ڳ�ʼ��
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_Spi_cfg(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  //spi�˿�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);	   //ʹ��SPI2����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
//  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 

	//P0/P1/P2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			//spis Ƭѡ	WR
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	LD_CS_H();
	
	//spi��������
	SPI_Cmd(SPI2, DISABLE);
	/* SPI2 ���� */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //��������NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //�����ʿ��� SYSCLK/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݸ�λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC����ʽ�Ĵ�����ʼֵΪ7
	SPI_Init(SPI2, &SPI_InitStructure);
	/* ʹ��SPI2 */
	SPI_Cmd(SPI2, ENABLE);

}
/***********************************************************
* ��    �ƣ� LD3320_EXTI_Cfg(void) 
* ��    �ܣ� �ⲿ�жϹ������ú���ض˿�����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LD3320_EXTI_Cfg(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //�ж���������
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//�ⲿ�ж�������
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_Line1);
		
	GPIO_SetBits(GPIOC,GPIO_Pin_1);	 //Ĭ�������ж�����

	EXTI_ClearFlag(EXTI_Line1);
	EXTI_ClearITPendingBit(EXTI_Line1);
	//�ж�Ƕ������
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/***********************************************************
* ��    �ƣ�  EXTI1_IRQHandler(void)
* ��    �ܣ� �ⲿ�жϺ���
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET ) 
	{
 		ProcessInt0(); 
		printf("�����ж�\r\n");	/*text........................*/
		EXTI_ClearFlag(EXTI_Line1);
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
/***********************************************************
* ��    �ƣ�void LED_gpio_cfg(void)
* ��    �ܣ�LED�˿�����
* ��ڲ�����  
* ���ڲ�����
* ˵    ����
* ���÷����� 
**********************************************************/ 
void LED_gpio_cfg(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
}
