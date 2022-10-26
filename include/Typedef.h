
#ifndef	_TYPEDEF_H_
#define	_TYPEDEF_H_

typedef unsigned char 	uchar;
typedef unsigned int 	uint;
typedef unsigned short 	ushort;
typedef unsigned long	ulong;

typedef unsigned long		DWORD;
typedef unsigned int		WORD;
typedef unsigned char		BYTE;
typedef signed long			SDWORD;
typedef signed int			SWORD;
typedef signed char			SBYTE;
#define abs(x) ((x)>0?(x):-(x))

#define	DOWNLOAD_TOTAL_FILE		3			//һ��3���ļ��У�File1
#define	MAX_FILE		260								//ÿ���ļ������240���ļ�
#define	MAX_IO			4									//һ��4���ɽڵ�����
#define	MAX_ARROW			2							
#define	MESSAGE_INDEX	200						//״̬��Ϣ�ļ���ÿ������ռ��80���ļ�
#define	MESSAGE_FILE		3						//״̬��Ϣ�ļ���ÿ������ռ��80���ļ�
#define	FLOOR_NON_STANDARD		221		//�� 221 �ļ���ʼΪ�Ǳ�¥��
#define	MESSAGE_NON_STANDARD		247		//�� 247 �ļ���ʼΪ�Ǳ�״̬��Ϣ


//���뿪�ص�����ֵ
#define	MAX_LANGUAGE						3
	#define CHINESE             		0x00		//����
	#define ENGLISH             		0x01		//Ӣ��
	#define CHINESE_AND_ENGLISH   	0x02		//��Ӣ��
	#define CHINESE_AND_YUEYU    		0x03		//��Ӣ��
#define	MAX_PLAY_MODE					4
	#define PLAY_NO_BACKMUSIC 		0x00		//�����ű�������
	#define PLAY_ALL            		0x01		//��������
//	#define PLAY_NO_ARROW_MOVING		0x01		//�����������ź�
	#define PLAY_BACKMUSIC_TIMER		0x02		//һ��ʱ���ȡ��
	#define PLAY_DEMO             0x03		//����DEMO

#define	MAX_FLOOR		64
#define	MAX_SPECIAL	40
	

#endif

