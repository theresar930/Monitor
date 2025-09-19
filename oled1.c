#include "oled1.h"
#include "fonts.h"
#include "dht11.h"
#include "light_sensor.h"
#include "system.h"


extern I2C_HandleTypeDef hi2c1;

uint8_t OLED_DisplayBuf[8][128];

void OLED_GPIO_Init(void)
{


	/*�ڳ�ʼ��ǰ������������ʱ����OLED�����ȶ�*/
	HAL_Delay(500);
	/*��SCL��SDA���ų�ʼ��Ϊ��©ģʽ*/
  HAL_I2C_MspInit(&hi2c1);


	/*�ͷ�SCL��SDA*/
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}
/**
  * ��    ����I2C��ʼ
  * ��    ������
  * �� �� ֵ����
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);		//�ͷ�SDA��ȷ��SDAΪ�ߵ�ƽ
	OLED_W_SCL(1);		//�ͷ�SCL��ȷ��SCLΪ�ߵ�ƽ
	OLED_W_SDA(0);		//��SCL�ߵ�ƽ�ڼ䣬����SDA��������ʼ�ź�
	OLED_W_SCL(0);		//��ʼ���SCLҲ���ͣ���Ϊ��ռ�����ߣ�ҲΪ�˷�������ʱ���ƴ��
}

/**
  * ��    ����I2C��ֹ
  * ��    ������
  * �� �� ֵ����
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);		//����SDA��ȷ��SDAΪ�͵�ƽ
	OLED_W_SCL(1);		//�ͷ�SCL��ʹSCL���ָߵ�ƽ
	OLED_W_SDA(1);		//��SCL�ߵ�ƽ�ڼ䣬�ͷ�SDA��������ֹ�ź�
}

/**
  * ��    ����I2C����һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ����ݣ���Χ��0x00~0xFF
  * �� �� ֵ����
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;

	/*ѭ��8�Σ��������η������ݵ�ÿһλ*/
	for (i = 0; i < 8; i++)
	{
		/*ʹ������ķ�ʽȡ��Byte��ָ��һλ���ݲ�д�뵽SDA��*/
		/*����!�������ǣ������з����ֵ��Ϊ1*/
		OLED_W_SDA(!!(Byte & (0x80 >> i)));
		OLED_W_SCL(1);	//�ͷ�SCL���ӻ���SCL�ߵ�ƽ�ڼ��ȡSDA
		OLED_W_SCL(0);	//����SCL��������ʼ������һλ����
	}

	OLED_W_SCL(1);		//�����һ��ʱ�ӣ�������Ӧ���ź�
	OLED_W_SCL(0);
}
/**********************************************************
 ���������ֲ���Բ鵽��������Ϊ�������ݣ�
0xAF���ر���ʾ����
0xD5 0x80��������ʾʱ�ӷ�Ƶ����/����Ƶ�ʡ�
0xA8 0x3F�����ö�·�����ʣ�1/64����
0xD3 0x00��������ʾƫ�ƣ���ƫ�ƣ���
0x40��������ʼ��Ϊ0��
0xA1�����ö��ض����е�ַ0ӳ�䵽SEG0����
0xC8������COMɨ�跽�������£���
0xDA 0x12������COMӲ���������á�
0x81 0xFF�����öԱȶȿ��ơ�
0xD9 0xF1������Ԥ������ڡ�
0xDB 0x40������VCOMH��ѹ���ʡ�
0xA4��������ʾȫ��������
0xA6��������ʾ�������Ƿ��ࣩģʽ��
0x8D 0x14������DC-DC��ɱÿ����͵�ɱñ��ʡ�
0xAE������ʾ������ʼ��ʾ��
 ***********************************************************/
uint8_t CMD_Data[]={
0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0x8D, 0x10,
0xA1, 0xC8, 0xDA, 0x12, 0x81, 0xCF, 0xD9, 0xf1, 0xDB, 0x30,
0xA4, 0xA6, 0xAF};


/*
 * @function: void OLED_Init(void)
 * @description: OLED��ʼ��
 * @return {*}
 */
void OLED_Init(void)
{
	HAL_Delay(200);

	uint8_t i = 0;
	for(i=0; i<23; i++)
	{
		OLED_WR_CMD(CMD_Data[i]);
	}
}

/**
 * @function: void OLED_WR_CMD(uint8_t cmd)
 * @description: ���豸д��������
 * ��һ��������ָ��I2C_HandleTypeDef�ṹ��ָ�룬�ýṹ����I2C���ߵ����ú�״̬��Ϣ��
 * �ڶ���������I2C�豸�ĵ�ַ����������0x78��
 * ������������Ҫд����ڴ��ַ����������0x00��
 * ���ĸ��������ڴ��ַ��С����������8λ��
 * �����������ָ��Ҫд�����ݵĻ�������ָ��
 * ������������Ҫд����ֽ�������������1��
 * ���һ�������ǳ�ʱʱ�䣨��������0x100����256����
 * ���������������ͨ��I2C������OLED��Ļ���������������ʾ���Ժ���Ϊ��
 * @param {uint8_t} cmd оƬ�ֲ�涨������
 * @return {*}
 */
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,100);
}

/**
 * @function: void OLED_WR_DATA(uint8_t data)
 * @description: ���豸д��������
 * @param {uint8_t} data ����
 * @return {*}
 */
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,100);
}

/**
 * @function: void OLED_On(void)
 * @description: ��Ļ����

 * @return {*}
 */
void OLED_On(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //����ҳ��ַ��0~7��
		OLED_WR_CMD(0x00);      //������ʾλ�á��е͵�ַ
		OLED_WR_CMD(0x10);      //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)
			OLED_WR_DATA(1);
	}
}


/**
 * @function: OLED_Clear(void)
 * @description: ����,������Ļ�Ǻ�ɫ��!��û����һ��!!!
 * @return {*}
 */
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //����ҳ��ַ��0~7��
		OLED_WR_CMD(0x00);      //������ʾλ�á��е͵�ַ
		OLED_WR_CMD(0x10);      //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
	}
}

/**
 * @function: void OLED_Display_On(void)
 * @description: ����OLED��ʾ
 * @return {*}
 */
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC����
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON,����ʾ
}


/**
 * @function: void OLED_Display_Off(void)
 * @description: �ر�OLED��ʾ
 * @return {*}
 */
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC����
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF���ر���ʾ
}

/**
 * @function: void OLED_Set_Pos(uint8_t x, uint8_t y)
 * @description: ��������
 * @param {uint8_t} x,y
 * @return {*}
 */
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_CMD(0xb0+y);	//����ҳ��ַ��0~7��
	OLED_WR_CMD(((x&0xf0)>>4)|0x10); //������ʾλ�á��иߵ�ַ
	OLED_WR_CMD(x&0x0f);	//������ʾλ�á��е͵�ַ
}


/**
 * @function: unsigned int oled_pow(uint8_t m,uint8_t n)
 * @description: m^n����
 * @param {uint8_t} m,n
 * @return {unsigned int} result
 */
unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;
	while(n--)result*=m;
	return result;
}

/**
 * @function: void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size,uint8_t Color_Turn)
 * @description: ��OLED12864�ض�λ�ÿ�ʼ��ʾһ���ַ�
 * @param {uint8_t} x�ַ���ʼ��ʾ�ĺ�����
 * @param {uint8_t} y�ַ���ʼ��ʾ��������
 * @param {uint8_t} chr����ʾ���ַ�
 * @param {uint8_t} Char_Size����ʾ�ַ��������С,ѡ������ 16/12
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size,uint8_t Color_Turn)
{
	unsigned char c=0,i=0;
		c=chr-' ';//�õ�ƫ�ƺ��ֵ
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
				{
				  if(Color_Turn)
					  OLED_WR_DATA(~ascii_1608x[c*16+i][i]);
				  else
					  OLED_WR_DATA(ascii_1608x[c*16+i][i]);
				}
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			    {
				  if(Color_Turn)
					  OLED_WR_DATA(~ascii_1608x[c*16+i+8][i]);
				  else
					  OLED_WR_DATA(ascii_1608x[c*16+i+8][i]);
			    }

			}
	     else
	     {
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
			    {
				  if(Color_Turn)
					  OLED_WR_DATA(~ascii_1608x[c][i]);
				  else
					  OLED_WR_DATA(ascii_1608x[c][i]);
			    }
		  }
}

/**
 * @function: void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_tChar_Size, uint8_t Color_Turn)
 * @description: ��OLED12864�ض�λ�ÿ�ʼ��ʾ�ַ���
 * @param {uint8_t} x����ʾ�ַ����Ŀ�ʼ������x:0~127
 * @param {uint8_t} y����ʾ�ַ����Ŀ�ʼ������ y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {uint8_t} *chr����ʾ���ַ���
 * @param {uint8_t} Char_Size����ʾ�ַ����������С,ѡ������ 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_ShowString(uint8_t x,uint8_t y,char*chr,uint8_t Char_Size, uint8_t Color_Turn)
{
	uint8_t  j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size, Color_Turn);
			if (Char_Size == 12) //6X8�������м�6����ʾ��һ���ַ�
				x += 6;
			else  //8X16�������м�8����ʾ��һ���ַ�
				x += 8;

			if (x > 122 && Char_Size==12) //TextSize6x8���һ�в�����ʾ�ˣ�����һ�м�����ʾ
			{
				x = 0;
				y++;
			}
			if (x > 120 && Char_Size== 16) //TextSize8x16���һ�в�����ʾ�ˣ�����һ�м�����ʾ
			{
				x = 0;
				y++;
			}
			j++;
	}
}

/**
 * @function: void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2, Color_Turn)
 * @description: ��ʾ����
 * @param {uint8_t} x����ʾ��������ʼ������,x:0~126
 * @param {uint8_t} y����ʾ��������ʼ������, y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {unsigned int} num:���������
 * @param {uint8_t } len:���������λ��
 * @param {uint8_t} size2:��������ݴ�С��ѡ�� 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2, uint8_t Color_Turn)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2, Color_Turn);
				continue;
			}else enshow=1;

		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2, Color_Turn);
	}
}


/**
 * @function: void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t Color_Turn)
 * @description: ��ʾ����������
 * @param {uint8_t} x����ʾ��������ʼ������,x:0~126
 * @param {uint8_t} y����ʾ��������ʼ������, y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {float} num:����ĸ���������
 * @param {uint8_t } z_ len:�������ֵ�λ��
 * @param {uint8_t } f_len: С�����ֵ�λ��
 * @param {uint8_t} size2:��������ݴ�С��ѡ�� 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t Color_Turn)
{
	uint8_t t,temp,i=0;//iΪ������־λ
	uint8_t enshow;
	int z_temp,f_temp;
	if(num<0)
	{
		z_len+=1;
		i=1;
		num=-num;
	}
	z_temp=(int)num;
	//��������
	for(t=0;t<z_len;t++)
	{
		temp=(z_temp/oled_pow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2, Color_Turn);
				continue;
			}
			else
			enshow=1;
		}
		OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2, Color_Turn);
	}
	//С����
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2, Color_Turn);

	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //С������
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2, Color_Turn);
	}
	if(i==1)//���Ϊ�����ͽ���ǰ��һλ��ֵ��-��
	{
		OLED_ShowChar(x,y,'-',size2, Color_Turn);
		i=0;
	}
}

/**
 * @function: void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
 * @description: ��OLED�ض�λ�ÿ�ʼ��ʾ16X16����
 * @param {uint8_t} x����ʾ�ĺ�����ʼ������x: 0~112�����к���֮����Ҫ���16
 * @param {uint8_t} y����ʾ�ĺ�����ʼ������ y: 0~6 , ���к���֮����Ҫ���2
 * @param {uint8_t} no����ʾ�ĺ��ֱ��
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
{
	uint8_t t=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~tfont16[2*no][t]); //��ʾ���ֵ��ϰ벿��
				else
					OLED_WR_DATA(tfont16[2*no][t]); //��ʾ���ֵ��ϰ벿��
        }

		OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~tfont16[2*no+1][t]); //��ʾ���ֵ��ϰ벿��
				else
					OLED_WR_DATA(tfont16[2*no+1][t]);//��ʾ���ֵ��ϰ벿��

         }
}

/**
 * @function: void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: ��OLED�ض�������ʾBMPͼƬ
 * @param {uint8_t} x0ͼ��ʼ��ʾ������  x0:0~127
 * @param {uint8_t} y0ͼ��ʼ��ʾ������  y0:0~7
 * @param {uint8_t} x1ͼ�������ʾ������  x1:1~128
 * @param {uint8_t} y1ͼ�������ʾ������  y1:1~8
 * @param {uint8_t} *BMP����ʾ��ͼ������
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
{
   uint32_t j = 0;
   uint8_t x = 0, y = 0;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
		{
			if (Color_Turn)
				OLED_WR_DATA(~BMP[j++]);//��ʾ����ͼƬ
			else
				OLED_WR_DATA(BMP[j++]);//��ʾͼƬ

		}
	}
}


/**
 * @function: void OLED_HorizontalShift(uint8_t direction)
 * @description: ��Ļ����ˮƽȫ����������
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @return {*}
 */
void OLED_HorizontalShift(uint8_t direction)

{
	OLED_WR_CMD(0x2e);//ֹͣ����
	OLED_WR_CMD(direction);//���ù�������
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(0x00);//���ÿ�ʼҳ��ַ
	OLED_WR_CMD(0x07);//����ÿ����������֮���ʱ������֡Ƶ
    //  0x00-5֡�� 0x01-64֡�� 0x02-128֡�� 0x03-256֡�� 0x04-3֡�� 0x05-4֡�� 0x06-25֡�� 0x07-2֡��
	OLED_WR_CMD(0x07);//���ý���ҳ��ַ
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(0xff);//�����ֽ����ã�Ĭ��Ϊ0xff
	OLED_WR_CMD(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
}

/**
 * @function: void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
 * @description: ��Ļ��������ˮƽ��������
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @param {uint8_t} start ��ʼҳ��ַ  0x00~0x07
 * @param {uint8_t} end  ����ҳ��ַ  0x01~0x07
 * @return {*}
 */
void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
{
	OLED_WR_CMD(0x2e);//ֹͣ����
	OLED_WR_CMD(direction);//���ù�������
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(start);//���ÿ�ʼҳ��ַ
	OLED_WR_CMD(0x07);//����ÿ����������֮���ʱ������֡Ƶ,0x07�������ٶ�2֡
	OLED_WR_CMD(end);//���ý���ҳ��ַ
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(0xff);//�����ֽ����ã�Ĭ��Ϊ0xff
	OLED_WR_CMD(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����

}

/**
 * @function: void OLED_VerticalAndHorizontalShift(uint8_t direction)
 * @description: ��Ļ���ݴ�ֱˮƽȫ����������
 * @param {uint8_t} direction				���Ϲ���	 0x29
 *                                                            ���Ϲ���   0x2A
 * @return {*}
 */
void OLED_VerticalAndHorizontalShift(uint8_t direction)
{
	OLED_WR_CMD(0x2e);//ֹͣ����
	OLED_WR_CMD(direction);//���ù�������
	OLED_WR_CMD(0x01);//�����ֽ�����
	OLED_WR_CMD(0x00);//���ÿ�ʼҳ��ַ
	OLED_WR_CMD(0x07);//����ÿ����������֮���ʱ������֡Ƶ���������ٶ�
	OLED_WR_CMD(0x07);//���ý���ҳ��ַ
	OLED_WR_CMD(0x01);//��ֱ����ƫ����
	OLED_WR_CMD(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	OLED_WR_CMD(0xff);//�����ֽ����ã�Ĭ��Ϊ0xff
	OLED_WR_CMD(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
}

/**
 * @function: void OLED_DisplayMode(uint8_t mode)
 * @description: ��Ļ����ȡ����ʾ
 * @param {uint8_t} direction			ON	0xA7  ��
 *                                                          OFF	0xA6	Ĭ�ϴ�ģʽ���������ص���
 * @return {*}
 */
void OLED_DisplayMode(uint8_t mode)
{
	OLED_WR_CMD(mode);
}

/**
 * @function: void OLED_IntensityControl(uint8_t intensity)
 * @description: ��Ļ���ȵ���
 * @param  {uint8_t} intensity	0x00~0xFF,RESET=0x7F
 * @return {*}
 */
void OLED_IntensityControl(uint8_t intensity)
{
	OLED_WR_CMD(0x81);
	OLED_WR_CMD(intensity);
}


void OLED_Main_Screen(void) {
    OLED_Clear();
//	  OLED_ShowString(32,32,"A",16,0);
//         OLED_ShowNum(4,4,temperature,2,16,0);
    OLED_ShowCHinese(4,4,0,0);
//         OLED_ShowNum(4,20, humidity,2,16,0);
//	  OLED_ShowString(4,32,"����",16,0);
//         OLED_ShowNum(4,36, adc_volt,4,16,0);
}

void OLED_Auto_Screen(void) {
    OLED_Clear();
	  OLED_ShowString(16,0,"�Զ�ģʽ��",16,0);
//	  OLED_ShowString(96,0, "!!",16);
	  OLED_ShowString(16,16,"�¶�",16,0);
         OLED_ShowNum(64,16,temperature,2,16,0);
    OLED_ShowString(16,32,"ʪ��",16,0);
         OLED_ShowNum(64,20, humidity,2,16,0);
	  OLED_ShowString(16,48,"����",16,0);
         OLED_ShowNum(64,36, adc_volt,2,16,0);
}
//void Show_Mode_Select_Screen(void) {
//    OLED_Clear();
//    OLED_ShowString(32, 8,  "ѡ  ��  ģ  ʽ",16,0);
//    OLED_ShowString(32, 8, "��  ��  ģ  ʽ", 16,0);
//    OLED_ShowString(32, 32, "��  ��  ģ  ʽ",16,0);
//}

void OLED_Manual_Mode_Screen(void) {
    OLED_Clear();
	
    switch (system_state.manual_page) {
			case MANUAL_PAGE_LED:  
            OLED_ShowString(40,8,"��",16,0);
            OLED_ShowString(80,8, system_state.led_state ? "�� " : "��", 16,0);
            OLED_ShowString(40,26, "����",16, 0);
            OLED_ShowString(40, 44, "ˮ��",16, 0);
            break;
        case MANUAL_PAGE_MOTOR_A:
		    OLED_ShowString(40,8,"��",16,0);
            OLED_ShowString(40,26, "����",16, 0);
			OLED_ShowString(80,26, system_state.motor_a_state ? "�� " : "��",16, 0);
            OLED_ShowString(40, 44, "ˮ��",16, 0);
            break;
        case MANUAL_PAGE_MOTOR_B:  
            OLED_ShowString(40,8,"��",16,0);
            OLED_ShowString(40,26, "����", 16,0);
            OLED_ShowString(40, 44, "ˮ��", 16,0);
			OLED_ShowString(80,44, system_state.motor_b_state ? "�� " : "��", 16,0);
            break;
    }
}
void I2C_Scan(void)
{
    uint8_t i;
    for(i = 1; i < 128; i++) {
        if(HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10) == HAL_OK) {
            printf("Found device at 0x%02X\n", i);
        }
    }
}
