#ifndef _FONT_GB_H_
#define _FONT_GB_H_ 	   

typedef struct 
{
	unsigned char Index[2];	
	unsigned char Msk[24];
}typFNT_GB12; 
const typFNT_GB12 FONT_GB12[]={
"思",0xFE,0x03,0x22,0x02,0x22,0x02,0xFE,0x03,0x22,0x02,0x22,0x02,0xFE,0x03,0x20,0x00,
0x4A,0x02,0x4A,0x04,0x09,0x05,0xF0,0x01,/*"思",0*/
"迈",0x01,0x00,0xFA,0x07,0x20,0x00,0x20,0x00,0xE0,0x03,0x23,0x02,0x22,0x02,0x12,0x02,
0x12,0x02,0x8A,0x01,0x02,0x00,0xFD,0x07,/*"迈",1*/
"微",0x24,0x02,0xAA,0x02,0xA9,0x06,0xFC,0x05,0x82,0x04,0x73,0x05,0x02,0x05,0x72,0x05,
0x52,0x02,0xD2,0x02,0x52,0x05,0x8A,0x04,/*"微",2*/
"电",0x10,0x00,0x10,0x00,0xFF,0x01,0x11,0x01,0x11,0x01,0xFF,0x01,0x11,0x01,0x11,0x01,
0xFF,0x01,0x11,0x04,0x10,0x04,0xE0,0x07,/*"电",3*/
"子",0x00,0x00,0xFC,0x01,0x80,0x00,0x40,0x00,0x20,0x00,0x20,0x00,0xFF,0x07,0x20,0x00,
0x20,0x00,0x20,0x00,0x20,0x00,0x38,0x00,/*"子",4*/

};

typedef struct 
{
	unsigned char Index[2];	
	unsigned char Msk[32];
}typFNT_GB16; 
const typFNT_GB16 FONT_GB16[]={
"思",0x00,0x00,0xFC,0x1F,0x84,0x10,0x84,0x10,0xFC,0x1F,0x84,0x10,0x84,0x10,0xFC,0x1F,
0x04,0x10,0x80,0x00,0x10,0x11,0x12,0x21,0x12,0x48,0x12,0x48,0xE1,0x0F,0x00,0x00,/*0*/
"迈",0x00,0x00,0x04,0x00,0xE8,0x7F,0x08,0x01,0x00,0x01,0x00,0x3F,0x0F,0x21,0x08,0x21,
0x08,0x21,0x88,0x20,0x88,0x20,0x48,0x14,0x28,0x08,0x14,0x00,0xE2,0x7F,0x00,0x00,/*1*/
"微",0x88,0x10,0xA8,0x12,0xA4,0x12,0xA2,0x0A,0xE9,0x7B,0x08,0x24,0x04,0x28,0xF6,0x2B,
0x05,0x28,0xE4,0x29,0x24,0x29,0x24,0x15,0x24,0x13,0x24,0x29,0x14,0x28,0x0C,0x44,/*2*/
"电",0x80,0x00,0x80,0x00,0x80,0x00,0xFC,0x1F,0x84,0x10,0x84,0x10,0x84,0x10,0xFC,0x1F,
0x84,0x10,0x84,0x10,0x84,0x10,0xFC,0x1F,0x84,0x50,0x80,0x40,0x80,0x40,0x00,0x7F,/*3*/
"子",0x00,0x00,0xFE,0x1F,0x00,0x08,0x00,0x04,0x00,0x02,0x80,0x01,0x80,0x00,0xFF,0x7F,
0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xA0,0x00,0x40,0x00,/*4*/
"技",0x08,0x04,0x08,0x04,0x08,0x04,0xC8,0x7F,0x3F,0x04,0x08,0x04,0x08,0x04,0xA8,0x3F,
0x18,0x21,0x0C,0x11,0x0B,0x12,0x08,0x0A,0x08,0x04,0x08,0x0A,0x8A,0x11,0x64,0x60,/*"技",0*/
"术",0x80,0x00,0x80,0x04,0x80,0x08,0x80,0x08,0xFE,0x3F,0xC0,0x01,0xA0,0x02,0xA0,0x02,
0x90,0x04,0x88,0x08,0x84,0x10,0x82,0x20,0x81,0x40,0x80,0x00,0x80,0x00,0x80,0x00,/*"术",1*/
"有",0x40,0x00,0x40,0x00,0xFF,0x7F,0x20,0x00,0x20,0x00,0xF0,0x0F,0x10,0x08,0x18,0x08,
0xF4,0x0F,0x12,0x08,0x11,0x08,0xF0,0x0F,0x10,0x08,0x10,0x08,0x10,0x0A,0x10,0x04,/*"有",2*/
"限",0x00,0x00,0xDE,0x1F,0x52,0x10,0x4A,0x10,0xCA,0x1F,0x46,0x10,0x4A,0x10,0xD2,0x1F,
0x52,0x22,0x52,0x12,0x56,0x0C,0x4A,0x04,0x42,0x08,0x42,0x11,0xC2,0x60,0x42,0x00,/*"限",3*/
"公",0x00,0x01,0x20,0x01,0x20,0x01,0x10,0x02,0x10,0x02,0x08,0x04,0x04,0x08,0x42,0x10,
0x41,0x60,0x20,0x00,0x20,0x02,0x10,0x04,0x08,0x04,0xFC,0x0F,0x08,0x08,0x00,0x00,/*"公",4*/
"司",0x00,0x00,0xFC,0x1F,0x00,0x10,0x00,0x10,0xFE,0x17,0x00,0x10,0x00,0x10,0xF8,0x11,
0x08,0x11,0x08,0x11,0x08,0x11,0x08,0x11,0xF8,0x11,0x08,0x11,0x00,0x14,0x00,0x08,/*"司",5*/
"欢",0x00,0x01,0x00,0x01,0x3F,0x01,0x20,0x3F,0xA0,0x20,0x92,0x10,0x54,0x02,0x28,0x02,
0x08,0x02,0x14,0x05,0x24,0x05,0xA2,0x08,0x81,0x08,0x40,0x10,0x20,0x20,0x10,0x40,/*"欢",6*/
"迎",0x00,0x00,0x04,0x01,0xC8,0x3C,0x48,0x24,0x40,0x24,0x40,0x24,0x4F,0x24,0x48,0x24,
0x48,0x24,0x48,0x2D,0xC8,0x14,0x48,0x04,0x08,0x04,0x14,0x04,0xE2,0x7F,0x00,0x00,/*"迎",7*/
"您",0x90,0x00,0x90,0x00,0x88,0x3F,0x4C,0x20,0x2A,0x12,0x99,0x0A,0x88,0x12,0x48,0x22,
0x28,0x22,0x88,0x02,0x08,0x01,0x40,0x00,0x8A,0x20,0x8A,0x48,0x09,0x48,0xF0,0x0F,/*"您",8*/
"纯",0x08,0x02,0x08,0x02,0x04,0x02,0xC4,0x3F,0x12,0x02,0x5F,0x12,0x48,0x12,0x44,0x12,
0x42,0x12,0xDF,0x1F,0x02,0x12,0x00,0x02,0x18,0x42,0x07,0x42,0x02,0x7C,0x00,0x00,/*"纯",9*/
"色",0x10,0x00,0x10,0x00,0xF8,0x07,0x04,0x04,0x02,0x02,0xFD,0x1F,0x84,0x10,0x84,0x10,
0x84,0x10,0xFC,0x1F,0x04,0x00,0x04,0x40,0x04,0x40,0x04,0x40,0xF8,0x7F,0x00,0x00,/*"色",10*/
"填",0x04,0x02,0x04,0x02,0xE4,0x3F,0x04,0x02,0xC4,0x1F,0x5F,0x10,0xC4,0x1F,0x44,0x10,
0xC4,0x1F,0x44,0x10,0xC4,0x1F,0x5C,0x10,0xF7,0x7F,0x82,0x08,0x40,0x10,0x20,0x20,/*"填",11*/
"充",0x40,0x00,0x80,0x00,0xFF,0x7F,0x20,0x00,0x20,0x00,0x10,0x04,0x08,0x08,0xFC,0x1F,
0x20,0x12,0x20,0x02,0x20,0x02,0x20,0x02,0x10,0x22,0x10,0x22,0x08,0x22,0x06,0x3C,/*"充",12*/
"矩",0x04,0x00,0x84,0x7F,0xBC,0x00,0x8A,0x00,0x89,0x00,0x88,0x3F,0x88,0x20,0xFF,0x20,
0x88,0x20,0x88,0x20,0x88,0x3F,0x94,0x00,0xA4,0x00,0xA2,0x00,0x82,0x7F,0x01,0x00,/*"矩",13*/
"形",0x00,0x00,0xFE,0x21,0x48,0x20,0x48,0x10,0x48,0x08,0x48,0x44,0x48,0x40,0xFF,0x23,
0x48,0x10,0x48,0x08,0x48,0x44,0x48,0x40,0x44,0x20,0x44,0x10,0x42,0x08,0x41,0x06,/*"形",14*/
"画",0x00,0x00,0xFF,0x7F,0x00,0x00,0x00,0x00,0xF2,0x27,0x92,0x24,0x92,0x24,0xF2,0x27,
0x92,0x24,0x92,0x24,0x92,0x24,0xF2,0x27,0x02,0x20,0x02,0x20,0xFE,0x3F,0x00,0x20,/*"画",15*/
"圆",0x00,0x00,0xFE,0x3F,0x02,0x20,0xF2,0x27,0x12,0x24,0xF2,0x27,0x02,0x20,0xFA,0x2F,
0x0A,0x28,0x8A,0x28,0x8A,0x28,0x4A,0x29,0x22,0x22,0x12,0x24,0xFE,0x3F,0x02,0x20,/*"圆",16*/
"图",0x00,0x00,0xFE,0x3F,0x42,0x20,0x42,0x20,0xE2,0x27,0x32,0x22,0x4A,0x21,0x82,0x20,
0x62,0x23,0x1E,0x3C,0xC2,0x20,0x02,0x21,0x62,0x20,0x82,0x21,0xFE,0x3F,0x02,0x20,/*"图",17*/
"片",0x00,0x02,0x08,0x02,0x08,0x02,0x08,0x02,0x08,0x02,0xF8,0x3F,0x08,0x00,0x08,0x00,
0x08,0x00,0xF8,0x07,0x08,0x04,0x08,0x04,0x08,0x04,0x04,0x04,0x04,0x04,0x02,0x04,/*"片",18*/
"显",0x00,0x00,0xF8,0x0F,0x08,0x08,0x08,0x08,0xF8,0x0F,0x08,0x08,0x08,0x08,0xF8,0x0F,
0x20,0x02,0x22,0x22,0x24,0x22,0x28,0x12,0x28,0x0A,0x20,0x02,0xFF,0x7F,0x00,0x00,/*"显",19*/
"示",0x00,0x00,0xFC,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x7F,0x80,0x00,
0x80,0x00,0x88,0x08,0x88,0x10,0x84,0x20,0x82,0x40,0x81,0x40,0xA0,0x00,0x40,0x00,/*"示",20*/
"菜",0x10,0x04,0x10,0x04,0xFF,0x7F,0x10,0x04,0x00,0x08,0x00,0x1F,0xFC,0x00,0x88,0x08,
0x10,0x04,0x80,0x00,0xFE,0x3F,0xA0,0x02,0x90,0x04,0x8C,0x18,0x83,0x60,0x80,0x00,/*"菜",21*/
"单",0x08,0x08,0x10,0x04,0x20,0x02,0xFC,0x1F,0x84,0x10,0x84,0x10,0xFC,0x1F,0x84,0x10,
0x84,0x10,0xFC,0x1F,0x80,0x00,0x80,0x00,0xFF,0x7F,0x80,0x00,0x80,0x00,0x80,0x00,/*"单",22*/
"英",0x10,0x04,0x10,0x04,0xFF,0x7F,0x10,0x04,0x80,0x00,0x80,0x00,0xF8,0x0F,0x88,0x08,
0x88,0x08,0x88,0x08,0xFF,0x7F,0x40,0x01,0x20,0x02,0x10,0x04,0x0C,0x18,0x03,0x60,/*"英",23*/
"文",0x40,0x00,0x80,0x00,0x80,0x00,0xFF,0x7F,0x08,0x08,0x08,0x08,0x10,0x04,0x10,0x04,
0x20,0x02,0x40,0x01,0x80,0x00,0x40,0x01,0x20,0x02,0x10,0x04,0x0C,0x18,0x03,0x60,/*"文",24*/
"综",0x08,0x02,0x08,0x04,0xC4,0x7F,0x44,0x40,0x12,0x00,0x9F,0x3F,0x08,0x00,0x04,0x00,
0xC2,0x7F,0x1F,0x04,0x82,0x14,0x80,0x24,0x58,0x44,0x27,0x44,0x02,0x05,0x00,0x02,/*"综",25*/
"合",0x80,0x00,0x80,0x00,0x40,0x01,0x20,0x02,0x10,0x04,0x0C,0x18,0xF3,0x67,0x00,0x00,
0x00,0x00,0xF8,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0x0F,0x08,0x08,/*"合",26*/
"亮",0x40,0x00,0x80,0x00,0xFE,0x3F,0x00,0x00,0xF8,0x0F,0x08,0x08,0xF8,0x0F,0x00,0x00,
0xFE,0x7F,0x02,0x40,0xF1,0x27,0x10,0x04,0x10,0x04,0x08,0x44,0x04,0x44,0x03,0x78,/*"亮",27*/
"度",0x80,0x00,0x00,0x01,0xFC,0x7F,0x44,0x04,0x44,0x04,0xFC,0x3F,0x44,0x04,0x44,0x04,
0xC4,0x07,0x04,0x00,0xF4,0x0F,0x24,0x08,0x42,0x04,0x82,0x03,0x61,0x0C,0x1C,0x70,/*"度",28*/
"测",0x00,0x20,0xE4,0x23,0x28,0x22,0x28,0x2A,0xA1,0x2A,0xA2,0x2A,0xA2,0x2A,0xA8,0x2A,
0xA8,0x2A,0xA4,0x2A,0xA7,0x2A,0x84,0x20,0x44,0x21,0x44,0x22,0x24,0x28,0x10,0x10,/*"测",29*/
"试",0x00,0x14,0x04,0x24,0x08,0x24,0x08,0x04,0xE0,0x7F,0x00,0x04,0x0F,0x04,0xE8,0x07,
0x88,0x04,0x88,0x08,0x88,0x08,0xA8,0x08,0x98,0x53,0xE8,0x50,0x40,0x60,0x00,0x40,/*"试",30*/
"程",0x10,0x00,0xB8,0x3F,0x8F,0x20,0x88,0x20,0x88,0x20,0xBF,0x3F,0x08,0x00,0x0C,0x00,
0x9C,0x7F,0x2A,0x04,0x2A,0x04,0x89,0x3F,0x08,0x04,0x08,0x04,0xC8,0x7F,0x08,0x00,/*"程",31*/
"序",0x80,0x00,0x00,0x01,0xFC,0x7F,0x04,0x00,0xC4,0x1F,0x04,0x08,0x04,0x05,0x04,0x02,
0xF4,0x7F,0x04,0x42,0x04,0x22,0x04,0x02,0x02,0x02,0x02,0x02,0x81,0x02,0x00,0x01,/*"序",32*/
"调",0x00,0x00,0xE2,0x3F,0x24,0x22,0x24,0x22,0xA0,0x2F,0x20,0x22,0x27,0x22,0xE4,0x3F,
0x24,0x20,0xA4,0x2F,0xA4,0x28,0xB4,0x28,0xAC,0x2F,0x24,0x20,0x20,0x28,0x10,0x10,/*"调",33*/
"光",0x80,0x00,0x84,0x10,0x88,0x10,0x90,0x08,0x90,0x04,0x80,0x00,0xFF,0x7F,0x20,0x02,
0x20,0x02,0x20,0x02,0x20,0x02,0x10,0x42,0x10,0x42,0x08,0x42,0x04,0x7C,0x03,0x00,/*"光",34*/
"版",0x10,0x10,0x12,0x38,0x92,0x07,0x92,0x00,0x92,0x00,0xBE,0x3F,0x82,0x22,0x82,0x22,
0x9E,0x22,0x92,0x14,0x92,0x14,0x92,0x08,0x92,0x08,0x52,0x14,0x52,0x22,0x31,0x41,/*"版",35*/
"权",0x08,0x00,0xC8,0x3F,0x88,0x20,0x88,0x20,0xBF,0x20,0x08,0x11,0x0C,0x11,0x1C,0x11,
0x2A,0x0A,0x2A,0x0A,0x09,0x04,0x08,0x04,0x08,0x0A,0x08,0x11,0x88,0x20,0x68,0x40,/*"权",36*/
"所",0x40,0x10,0xE0,0x38,0x1C,0x07,0x04,0x01,0x04,0x01,0x7C,0x01,0x44,0x7F,0x44,0x11,
0x44,0x11,0x7C,0x11,0x04,0x11,0x04,0x11,0x82,0x10,0x82,0x10,0x41,0x10,0x20,0x10,/*"所",37*/
};


typedef struct 
{
	unsigned char Index[2];	
	unsigned char Msk[72];
}typFNT_GB24; 
const typFNT_GB24 FONT_GB24[]={
"温",0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x30,0xFE,0x0F,0x20,0x02,0x04,0x20,
0x02,0x04,0x80,0x02,0x04,0x40,0xFE,0x07,0x46,0x02,0x04,0x4C,0x02,0x04,0x48,0x02,
0x04,0x20,0xFE,0x07,0x20,0x00,0x00,0xA0,0x00,0x08,0x20,0xFF,0x1F,0x10,0x91,0x08,
0x1E,0x91,0x08,0x18,0x91,0x08,0x18,0x91,0x08,0x18,0x91,0x08,0x18,0x91,0x08,0x18,
0x91,0x68,0xE0,0x6E,0x17,0x00,0x00,0x00,/*"温",0*/
/* (24 X 24 , 宋体 )*/

"压",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0xF0,0xFF,0x3F,0x30,0x00,0x00,0x30,
0x30,0x00,0x30,0x30,0x00,0x30,0x30,0x00,0x30,0x30,0x00,0x30,0x30,0x00,0x30,0x30,
0x00,0xB0,0xFF,0x0F,0x30,0x30,0x00,0x30,0x30,0x00,0x10,0x30,0x01,0x10,0x30,0x02,
0x10,0x30,0x06,0x10,0x30,0x0C,0x08,0x30,0x04,0x08,0x30,0x00,0x04,0x30,0x30,0xE4,
0xCF,0x0F,0x02,0x00,0x00,0x00,0x00,0x00,/*"压",1*/
/* (24 X 24 , 宋体 )*/

"流",0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x60,0x00,0x10,0x40,0x10,0x20,0xFF,0x3F,0x60,
0x20,0x00,0x00,0x20,0x00,0x80,0x10,0x02,0x82,0x08,0x04,0x8C,0x04,0x0C,0x48,0xFE,
0x0B,0x48,0x00,0x08,0x20,0x26,0x06,0x20,0x26,0x02,0x20,0x26,0x02,0x10,0x26,0x02,
0x1E,0x26,0x02,0x18,0x22,0x02,0x18,0x22,0x22,0x18,0x23,0x22,0x18,0x21,0x22,0x98,
0x20,0x7E,0x60,0x20,0x00,0x00,0x00,0x00,/*"流",2*/
/* (24 X 24 , 宋体 )*/

"湿",0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x08,0x10,0xFE,0x1F,0x30,0x06,0x08,0x20,
0x06,0x08,0x80,0x06,0x08,0x42,0xFE,0x0F,0x4C,0x06,0x08,0x58,0x06,0x08,0x48,0xFE,
0x0F,0x20,0x96,0x00,0x20,0x10,0x01,0xA0,0x10,0x31,0x10,0x11,0x31,0x18,0x13,0x19,
0x1C,0x12,0x09,0x18,0x16,0x05,0x08,0x12,0x03,0x08,0x10,0x01,0x0C,0x10,0x01,0x08,
0x10,0x31,0xC0,0xEF,0x0E,0x00,0x00,0x00,/*"湿",3*/
/* (24 X 24 , 宋体 )*/

"一",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x38,0xFC,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"一",4*/
/* (24 X 24 , 宋体 )*/

"体",0x00,0x00,0x00,0x40,0x00,0x00,0xC0,0xC0,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0x60,
0x40,0x00,0x20,0x40,0x30,0x30,0xFF,0x7F,0x30,0x60,0x01,0x28,0x70,0x01,0x28,0x50,
0x01,0x24,0x58,0x02,0x22,0x48,0x02,0x20,0x44,0x04,0x20,0x46,0x0C,0x20,0x42,0x18,
0x20,0x41,0x34,0xA0,0xFE,0x3F,0x60,0x40,0x00,0x20,0x40,0x00,0x20,0x40,0x00,0x20,
0x40,0x00,0x20,0x40,0x00,0x00,0x00,0x00,/*"体",5*/
/* (24 X 24 , 宋体 )*/

"监",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC3,0x00,0x10,0x41,0x00,0x10,0x61,0x00,0x10,
0x61,0x18,0x10,0xA1,0x07,0x10,0x21,0x00,0x10,0x11,0x01,0x10,0x11,0x02,0x10,0x09,
0x06,0x10,0x05,0x04,0x00,0x01,0x04,0x00,0x01,0x00,0x10,0x00,0x04,0xF0,0xFF,0x07,
0x10,0x62,0x04,0x10,0x62,0x04,0x10,0x62,0x04,0x10,0x62,0x04,0x10,0x62,0x24,0xFE,
0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,/*"监",6*/
/* (24 X 24 , 宋体 )*/

"测",0x00,0x00,0x00,0x00,0x00,0x00,0x88,0x00,0x30,0x90,0x7F,0x10,0xB0,0x61,0x10,0xA0,
0x61,0x12,0x80,0x65,0x12,0x82,0x6D,0x12,0xCC,0x6D,0x12,0xC8,0x6D,0x12,0xA8,0x6D,
0x12,0xA0,0x6D,0x12,0xA0,0x6D,0x12,0xA0,0x6D,0x12,0x90,0x65,0x12,0x90,0x65,0x12,
0x9C,0x25,0x12,0x18,0x04,0x12,0x18,0x1A,0x10,0x18,0x22,0x10,0x18,0x61,0x10,0x98,
0x40,0x1C,0x60,0x00,0x18,0x00,0x00,0x00,/*"测",7*/
/* (24 X 24 , 宋体 )*/

"仪",0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x11,0x00,0xC0,0x20,0x00,0x40,0x60,0x00,0x60,
0xC0,0x00,0x20,0x40,0x0C,0x20,0x00,0x0C,0x30,0x04,0x04,0x30,0x04,0x06,0x28,0x08,
0x02,0x28,0x08,0x02,0x24,0x10,0x01,0x22,0x10,0x01,0x20,0xA0,0x00,0x20,0xE0,0x00,
0x20,0xC0,0x00,0x20,0xE0,0x00,0x20,0x30,0x03,0x20,0x18,0x07,0x20,0x0C,0x1E,0x20,
0x02,0x78,0xA0,0x01,0x10,0x00,0x00,0x00,/*"仪",8*/
/* (24 X 24 , 宋体 )*/

"度",0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x10,0x00,0x00,0x30,0x10,0xF0,0xFF,0x3F,0x10,
0x02,0x00,0x10,0x0C,0x03,0x10,0x04,0x11,0x10,0x04,0x39,0xF0,0xFF,0x07,0x10,0x04,
0x01,0x10,0x04,0x01,0x10,0xFC,0x01,0x10,0x00,0x00,0x98,0xFF,0x07,0x18,0x04,0x03,
0x08,0x04,0x01,0x08,0x88,0x01,0x08,0xD0,0x00,0x04,0x60,0x00,0x04,0xD8,0x01,0x02,
0x06,0x7F,0xE2,0x01,0x18,0x00,0x00,0x00,/*"度",9*/
/* (24 X 24 , 宋体 )*/

"：",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x38,0x00,0x00,0x30,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x38,0x00,0x00,0x38,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"：",10*/
/* (24 X 24 , 宋体 )*/

"速",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x08,0x40,0x00,0x10,0x40,0x10,0x30,
0xFF,0x3F,0x10,0x40,0x00,0x00,0x40,0x00,0x00,0xFE,0x0F,0x00,0x46,0x08,0x7E,0x46,
0x08,0x20,0x46,0x08,0x20,0xFE,0x0F,0x20,0x62,0x00,0x20,0xD0,0x00,0x20,0x58,0x07,
0x20,0x44,0x0C,0x20,0x42,0x18,0x20,0x41,0x10,0xD8,0x40,0x00,0x86,0x41,0x00,0x04,
0xFE,0x3F,0x00,0xF8,0x1F,0x00,0x00,0x00,/*"速",11*/
/* (24 X 24 , 宋体 )*/

"量",0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x07,0x60,0x00,0x02,0x60,0x00,0x02,0xE0,
0xFF,0x03,0x60,0x00,0x02,0xE0,0xFF,0x03,0x20,0x00,0x20,0xFE,0xFF,0x7F,0x00,0x00,
0x00,0xE0,0xFF,0x07,0x20,0x18,0x06,0x20,0x18,0x06,0xE0,0xFF,0x07,0x20,0x18,0x06,
0xE0,0xFF,0x07,0x20,0x18,0x02,0x00,0x18,0x04,0xF0,0xFF,0x0F,0x00,0x18,0x00,0x00,
0x18,0x30,0xFC,0xE7,0x0F,0x00,0x00,0x00,/*"量",12*/
/* (24 X 24 , 宋体 )*/

"静",0x00,0x00,0x00,0x00,0x40,0x00,0xC0,0xC0,0x00,0x40,0x64,0x04,0xFC,0xE7,0x0F,0x40,
0x38,0x06,0x40,0x12,0x02,0xF8,0x0D,0x01,0x40,0xF0,0x1F,0x40,0x8C,0x19,0xBC,0x83,
0x19,0x00,0x82,0x39,0xF8,0xFF,0x1F,0x08,0x82,0x19,0xF8,0x83,0x19,0x08,0x82,0x19,
0x08,0xF2,0x1F,0xF8,0x83,0x01,0x08,0x82,0x01,0x08,0x82,0x01,0x08,0x82,0x01,0xC8,
0xB2,0x00,0x88,0xC3,0x00,0x00,0x00,0x00,/*"静",13*/
/* (24 X 24 , 宋体 )*/

"动",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x80,0x00,0x00,0x86,0x00,0xFC,
0x81,0x00,0x00,0x80,0x00,0x00,0x80,0x30,0x00,0xE0,0x3F,0x00,0x8C,0x30,0xFC,0x83,
0x30,0x60,0x80,0x30,0x20,0x80,0x30,0x30,0x81,0x10,0x10,0xC2,0x10,0x08,0x46,0x10,
0x08,0x44,0x10,0xF4,0x2F,0x10,0x1C,0x28,0x10,0x00,0x10,0x10,0x00,0x88,0x18,0x00,
0x04,0x1F,0x00,0x02,0x0C,0x00,0x00,0x00,/*"动",14*/
/* (24 X 24 , 宋体 )*/

"反",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0xF0,0x1F,0xE0,0x0F,0x00,0x20,
0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0xE0,0xFF,0x0F,0x20,0x02,
0x0C,0x20,0x02,0x06,0x20,0x04,0x02,0x20,0x04,0x03,0x20,0x08,0x01,0x20,0x90,0x01,
0x30,0xF0,0x00,0x10,0x60,0x00,0x10,0xF0,0x00,0x08,0x98,0x03,0x08,0x06,0x0E,0x84,
0x01,0x7C,0x72,0x00,0x10,0x00,0x00,0x00,/*"反",15*/
/* (24 X 24 , 宋体 )*/

"吹",0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x20,0x00,0xFC,
0x31,0x00,0x8C,0x11,0x10,0x8C,0xF9,0x3F,0x8C,0x09,0x18,0x8C,0xC9,0x08,0x8C,0xC5,
0x04,0x8C,0xC3,0x00,0x8C,0xC1,0x00,0x8C,0xC1,0x00,0x8C,0x61,0x01,0xFC,0x61,0x01,
0x8C,0x20,0x02,0x0C,0x30,0x02,0x00,0x10,0x04,0x00,0x18,0x0C,0x00,0x0C,0x18,0x00,
0x02,0x30,0x80,0x01,0x00,0x00,0x00,0x00,/*"吹",16*/
/* (24 X 24 , 宋体 )*/

"下",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x38,0xFC,0xFF,0x07,0x00,
0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x18,0x00,0x00,0xE8,
0x00,0x00,0x88,0x03,0x00,0x08,0x07,0x00,0x08,0x0E,0x00,0x08,0x04,0x00,0x08,0x00,
0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,
0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,/*"下",17*/
/* (24 X 24 , 宋体 )*/

"页",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0xFC,0xFF,0x3F,0x00,0x0C,0x00,0x00,
0x04,0x00,0x00,0x04,0x02,0xE0,0xFF,0x07,0x60,0x00,0x02,0x60,0x08,0x02,0x60,0x18,
0x02,0x60,0x08,0x02,0x60,0x08,0x02,0x60,0x08,0x02,0x60,0x08,0x02,0x60,0x08,0x02,
0x60,0x0C,0x02,0x20,0x74,0x00,0x00,0x86,0x01,0x00,0x03,0x07,0x80,0x01,0x0C,0x60,
0x00,0x18,0x1C,0x00,0x10,0x00,0x00,0x00,/*"页",20*/
/* (24 X 24 , 宋体 )*/

"→",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,
0x01,0x00,0x80,0x0F,0xFC,0xFF,0x3F,0x00,0x80,0x07,0x00,0xC0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"→",21*/
/* (24 X 24 , 宋体 )*/

"←",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x03,
0x00,0xF0,0x01,0x00,0xF8,0xFF,0x3F,0xE0,0x01,0x00,0x00,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"←",22*/
/* (24 X 24 , 宋体 )*/

"℃",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x48,0xF8,0x01,0x48,
0x0C,0x16,0x48,0x06,0x18,0x30,0x03,0x18,0x00,0x01,0x10,0x80,0x01,0x10,0x80,0x01,
0x00,0x80,0x01,0x00,0x80,0x01,0x00,0x80,0x01,0x00,0x80,0x01,0x00,0x00,0x01,0x00,
0x00,0x03,0x00,0x00,0x03,0x08,0x00,0x06,0x04,0x00,0x1C,0x03,0x00,0xF0,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"℃",23*/
/* (24 X 24 , 宋体 )*/

"上",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x0C,0x00,0x00,0x0C,0x00,0x00,
0x0C,0x00,0x00,0x0C,0x00,0x00,0x0C,0x00,0x00,0x0C,0x04,0x00,0xFC,0x0F,0x00,0x0C,
0x00,0x00,0x0C,0x00,0x00,0x0C,0x00,0x00,0x0C,0x00,0x00,0x0C,0x00,0x00,0x0C,0x00,
0x00,0x0C,0x00,0x00,0x0C,0x00,0x00,0x0C,0x00,0x00,0x0C,0x10,0xFE,0xFF,0x3F,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"上",0*/
/* (24 X 24 , 宋体 )*/

"设",0x00,0x00,0x00,0x00,0x00,0x00,0x10,0xF8,0x03,0x20,0x18,0x06,0x60,0x18,0x02,0x60,
0x18,0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x04,0x02,0x7E,0x06,0x7E,0x20,0x01,
0x04,0xA0,0xFC,0x0F,0x20,0x08,0x04,0x20,0x10,0x06,0x20,0x10,0x06,0x20,0x20,0x02,
0x20,0x22,0x01,0x20,0xC1,0x01,0xA0,0xC0,0x00,0x60,0x60,0x03,0x60,0x18,0x0E,0x00,
0x06,0x38,0x80,0x01,0x00,0x00,0x00,0x00,/*"设",0*/
/* (24 X 24 , 宋体 )*/

"置",0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0x0F,0x10,0x42,0x08,0x10,0x42,0x08,0x10,
0x42,0x08,0xF0,0xFF,0x0F,0x10,0x08,0x08,0x00,0x08,0x08,0xF8,0xFF,0x1F,0x00,0x08,
0x02,0xC0,0xFF,0x07,0x40,0x00,0x02,0x40,0x00,0x02,0xC0,0xFF,0x03,0x40,0x00,0x02,
0xC0,0xFF,0x03,0x40,0x00,0x02,0xC0,0xFF,0x03,0x40,0x00,0x02,0x40,0x00,0x22,0xFE,
0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,/*"置",1*/
/* (24 X 24 , 宋体 )*/

"备",0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x03,0x00,0x00,0xFF,0x03,0x80,0x01,0x01,0x80,
0x82,0x01,0x40,0xC2,0x00,0x60,0x64,0x00,0x30,0x3C,0x00,0x08,0x18,0x00,0x00,0x3C,
0x00,0x00,0xC2,0x00,0x80,0x81,0x07,0xE0,0xFF,0x7F,0x5C,0x10,0x12,0x40,0x10,0x02,
0x40,0x10,0x02,0xC0,0xFF,0x03,0x40,0x10,0x02,0x40,0x10,0x02,0xC0,0xFF,0x03,0x40,
0x00,0x02,0x40,0x00,0x02,0x00,0x00,0x00,/*"备",2*/
/* (24 X 24 , 宋体 )*/

"状",0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x40,0x00,0x80,0x40,0x02,0x80,0x40,0x0C,0x80,
0x40,0x18,0x84,0x40,0x18,0x88,0x40,0x10,0x98,0x40,0x10,0x90,0xFF,0x3F,0x80,0xC0,
0x00,0x80,0x40,0x01,0xC0,0x60,0x01,0xA0,0x60,0x01,0x90,0x20,0x01,0x8C,0x20,0x02,
0x86,0x30,0x02,0x80,0x10,0x06,0x80,0x18,0x0C,0x80,0x08,0x1C,0x80,0x04,0x38,0x80,
0x02,0x30,0x40,0x01,0x00,0x00,0x00,0x00,/*"状",3*/
/* (24 X 24 , 宋体 )*/

"态",0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x0C,0x10,0xFC,
0xFF,0x3F,0x00,0x14,0x00,0x00,0x24,0x00,0x00,0x26,0x00,0x00,0x42,0x00,0x00,0x83,
0x01,0x80,0x0C,0x07,0x60,0x18,0x3E,0x18,0x10,0x18,0x06,0x00,0x00,0x00,0x0C,0x00,
0x90,0x19,0x04,0x90,0x10,0x18,0x98,0x10,0x19,0x9C,0x00,0x11,0x8C,0x00,0x11,0x80,
0x01,0x01,0x00,0xFF,0x01,0x00,0x00,0x00,/*"态",4*/
/* (24 X 24 , 宋体 )*/

"手",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0xF0,0x0F,0xF8,0x0F,0x00,0x00,
0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x04,0xF8,0xFF,0x0F,0x00,0x08,0x00,0x00,0x08,
0x00,0x00,0x08,0x00,0x00,0x08,0x10,0xFE,0xFF,0x3F,0x00,0x08,0x00,0x00,0x08,0x00,
0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x80,0x09,0x00,0x00,
0x0E,0x00,0x00,0x04,0x00,0x00,0x00,0x00,/*"手",5*/
/* (24 X 24 , 宋体 )*/

"零",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0xE0,0xFF,0x07,0x00,0x18,0x00,0xF8,
0xFF,0x3F,0x08,0x18,0x30,0xEC,0xD9,0x17,0x0C,0x18,0x08,0x00,0x08,0x00,0xE0,0xD9,
0x07,0x00,0x1C,0x00,0x00,0x23,0x00,0x80,0xC1,0x00,0x60,0x18,0x7F,0x1C,0x10,0x38,
0xE0,0xFF,0x01,0x00,0x80,0x01,0x00,0x60,0x00,0x00,0x17,0x00,0x00,0x38,0x00,0x00,
0x60,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,/*"零",6*/
/* (24 X 24 , 宋体 )*/

"点",0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x18,0x00,0x00,0x08,0x00,0x00,0x08,0x08,0x00,
0xF8,0x1F,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0xC0,0xFF,0x07,0x40,0x00,
0x06,0x40,0x00,0x06,0x40,0x00,0x06,0x40,0x00,0x06,0x40,0x00,0x06,0xC0,0xFF,0x07,
0x40,0x00,0x06,0x00,0x00,0x00,0x00,0x21,0x04,0x10,0x42,0x18,0x10,0xC6,0x10,0x18,
0x86,0x30,0x0C,0x00,0x00,0x00,0x00,0x00,/*"点",7*/
/* (24 X 24 , 宋体 )*/

"校",0x00,0x00,0x00,0x20,0x20,0x00,0x60,0xC0,0x00,0x20,0x80,0x00,0x20,0x80,0x00,0x20,
0xFF,0x3F,0xFE,0x03,0x00,0x20,0x20,0x02,0x20,0x30,0x0C,0xF0,0x18,0x18,0xB0,0x09,
0x30,0x30,0x0D,0x34,0x38,0x12,0x0E,0x28,0x10,0x06,0x28,0x10,0x03,0x24,0x20,0x03,
0x22,0xA0,0x01,0x20,0xC0,0x00,0x20,0xC0,0x00,0x20,0xE0,0x03,0x20,0x18,0x0E,0x20,
0x06,0x7C,0xA0,0x01,0x10,0x00,0x00,0x00,/*"校",8*/
/* (24 X 24 , 宋体 )*/

"准",0x00,0x00,0x00,0x00,0x88,0x00,0x00,0x98,0x01,0x0C,0x0C,0x01,0x18,0x04,0x11,0x98,
0xFC,0x3F,0x90,0x86,0x01,0x40,0x86,0x01,0x40,0x85,0x01,0x20,0x85,0x01,0xA0,0xFC,
0x1F,0x60,0x84,0x01,0x10,0x84,0x01,0x18,0x84,0x01,0x1C,0x84,0x11,0x18,0xFC,0x3F,
0x08,0x84,0x01,0x08,0x84,0x01,0x08,0x84,0x01,0x08,0x84,0x01,0x08,0xFC,0x3F,0x00,
0x04,0x00,0x00,0x04,0x00,0x00,0x00,0x00,/*"准",9*/
/* (24 X 24 , 宋体 )*/

"平",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xF8,0xFF,0x1F,0x00,0x18,0x00,0x00,
0x18,0x04,0x20,0x18,0x06,0xC0,0x18,0x02,0x80,0x18,0x01,0x80,0x98,0x00,0x00,0x58,
0x10,0xFE,0xFF,0x3F,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,
0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,
0x18,0x00,0x00,0x08,0x00,0x00,0x00,0x00,/*"平",10*/
/* (24 X 24 , 宋体 )*/

"滑",0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x04,0x30,0xFC,0x0F,0x20,0x04,0x04,0x20,
0xFC,0x04,0x00,0x44,0x04,0x46,0x45,0x04,0x4C,0xFF,0x3F,0xC8,0x01,0x30,0xA8,0x00,
0x10,0x20,0xFC,0x07,0x20,0x04,0x04,0x20,0x04,0x04,0x10,0xFC,0x07,0x10,0x04,0x04,
0x1E,0x04,0x04,0x18,0xFC,0x07,0x18,0x04,0x04,0x18,0x04,0x04,0x18,0x04,0x04,0x18,
0x84,0x07,0x00,0x04,0x02,0x00,0x00,0x00,/*"滑",11*/
/* (24 X 24 , 宋体 )*/

"时",0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x06,0x00,0x00,0x02,0xFC,0x01,0x02,0x84,
0x01,0x02,0x84,0x01,0x02,0x84,0xFF,0x3F,0x84,0x01,0x02,0x84,0x01,0x02,0xFC,0x01,
0x02,0x84,0x09,0x02,0x84,0x31,0x02,0x84,0x31,0x02,0x84,0x21,0x02,0x84,0x01,0x02,
0x84,0x01,0x02,0xFC,0x01,0x02,0x84,0x01,0x02,0x84,0x00,0x02,0x00,0x20,0x02,0x00,
0xC0,0x03,0x00,0x00,0x01,0x00,0x00,0x00,/*"时",12*/
/* (24 X 24 , 宋体 )*/

"间",0x00,0x00,0x00,0x20,0x00,0x00,0x60,0x00,0x00,0xC0,0xFE,0x1F,0xC8,0x00,0x10,0x18,
0x00,0x10,0x08,0x00,0x10,0x08,0xFF,0x10,0x08,0x41,0x10,0x08,0x41,0x10,0x08,0x41,
0x10,0x08,0x41,0x10,0x08,0x7F,0x10,0x08,0x41,0x10,0x08,0x41,0x10,0x08,0xC1,0x10,
0x08,0xFF,0x10,0x08,0xC1,0x10,0x08,0x00,0x10,0x08,0x00,0x10,0x08,0x00,0x19,0x08,
0x00,0x1E,0x08,0x00,0x0C,0x00,0x00,0x00,/*"间",13*/
/* (24 X 24 , 宋体 )*/

"烟",0x00,0x00,0x00,0x20,0x00,0x00,0x60,0x00,0x00,0x20,0xFC,0x3F,0x20,0x84,0x20,0x20,
0x85,0x21,0x20,0x87,0x21,0xA0,0x84,0x21,0x60,0x84,0x25,0x24,0xF4,0x2F,0x24,0x84,
0x20,0x20,0x84,0x20,0x20,0x84,0x20,0x20,0x84,0x21,0x20,0x84,0x22,0x60,0xC4,0x26,
0xB0,0x44,0x24,0x10,0x25,0x24,0x10,0x25,0x24,0x08,0xFC,0x3F,0x08,0x04,0x20,0x04,
0x04,0x20,0x02,0x04,0x00,0x00,0x00,0x00,/*"烟",14*/
/* (24 X 24 , 宋体 )*/

"道",0x00,0x00,0x00,0x00,0x04,0x00,0x18,0x08,0x06,0x30,0x10,0x02,0x20,0x10,0x01,0x80,
0xFF,0x3F,0x00,0x40,0x00,0x00,0x20,0x00,0x20,0xFC,0x0F,0x7E,0x04,0x0C,0x20,0x04,
0x0C,0x20,0xFC,0x0F,0x20,0x04,0x0C,0x20,0x04,0x0C,0x20,0xFC,0x0F,0x20,0x04,0x0C,
0x20,0x04,0x0C,0x20,0xFC,0x0F,0x30,0x04,0x0C,0x4C,0x00,0x00,0x86,0x01,0x00,0x00,
0x3E,0x78,0x00,0xF0,0x1F,0x00,0x00,0x00,/*"道",15*/
/* (24 X 24 , 宋体 )*/

"截",0x00,0x00,0x00,0x00,0x81,0x00,0x00,0x83,0x04,0x00,0x81,0x08,0xF8,0xBF,0x18,0x00,
0x81,0x10,0x00,0x81,0x20,0xFE,0xFF,0x3F,0x40,0x81,0x00,0xE0,0x82,0x00,0x20,0x86,
0x00,0xF0,0xFF,0x19,0x30,0x82,0x19,0x38,0x02,0x09,0xF4,0x3F,0x09,0x32,0x02,0x05,
0x30,0x02,0x07,0xF0,0x1F,0x02,0x30,0x22,0x27,0xF0,0xBF,0x2D,0x30,0xC0,0x38,0x30,
0x20,0x30,0x10,0x18,0x40,0x00,0x00,0x00,/*"截",16*/
/* (24 X 24 , 宋体 )*/

"面",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0xFE,0xFF,0x7F,0x00,0x0C,0x00,0x00,
0x04,0x00,0x00,0x04,0x00,0xF8,0xFF,0x1F,0x18,0x42,0x18,0x18,0x42,0x18,0x18,0x42,
0x18,0x18,0x7E,0x18,0x18,0x42,0x18,0x18,0x42,0x18,0x18,0x42,0x18,0x18,0x42,0x18,
0x18,0x7E,0x18,0x18,0x42,0x18,0x18,0x42,0x18,0x18,0x42,0x18,0xF8,0xFF,0x1F,0x18,
0x00,0x18,0x08,0x00,0x08,0x00,0x00,0x00,/*"面",17*/
/* (24 X 24 , 宋体 )*/

"积",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0xC0,0x03,0x00,0x7C,0xF0,0x3F,0x40,
0x10,0x10,0x40,0x10,0x10,0x40,0x12,0x10,0xFE,0x17,0x10,0x60,0x10,0x10,0x60,0x10,
0x10,0xE0,0x10,0x10,0x70,0xF3,0x1F,0x50,0x12,0x10,0x48,0x02,0x00,0x48,0x40,0x02,
0x44,0x40,0x04,0x42,0x20,0x08,0x40,0x30,0x10,0x40,0x18,0x30,0x40,0x0C,0x20,0x40,
0x02,0x20,0x40,0x01,0x00,0x00,0x00,0x00,/*"积",18*/
/* (24 X 24 , 宋体 )*/

"频",0x00,0x00,0x00,0x80,0x00,0x20,0x80,0xF8,0x7F,0x80,0x00,0x03,0xB0,0x00,0x01,0x90,
0x07,0x01,0x90,0xE0,0x3F,0x90,0x20,0x20,0x90,0x20,0x20,0xFE,0x2F,0x22,0x80,0x20,
0x22,0x80,0x20,0x22,0x90,0x20,0x22,0x90,0x24,0x22,0x88,0x2C,0x23,0x88,0x26,0x23,
0x04,0x22,0x01,0x02,0x01,0x01,0x80,0x81,0x0D,0xC0,0x80,0x30,0x20,0x60,0x30,0x18,
0x10,0x60,0x04,0x0C,0x00,0x00,0x00,0x00,/*"频",19*/
/* (24 X 24 , 宋体 )*/

"次",0x00,0x00,0x00,0x00,0x30,0x00,0x08,0x30,0x00,0x10,0x30,0x00,0x30,0x10,0x00,0x20,
0x19,0x00,0xA0,0xF8,0x3F,0x80,0x08,0x10,0x80,0x44,0x08,0x40,0x44,0x04,0x40,0xC2,
0x00,0x20,0xC1,0x00,0xA0,0xE0,0x00,0x10,0xE0,0x00,0x1C,0x20,0x01,0x18,0x20,0x01,
0x10,0x30,0x02,0x10,0x10,0x02,0x18,0x08,0x04,0x18,0x04,0x0C,0x18,0x02,0x38,0x80,
0x01,0x70,0x40,0x00,0x00,0x00,0x00,0x00,/*"次",20*/
/* (24 X 24 , 宋体 )*/

"参",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0xC3,0x00,0x80,0x00,0x03,0x60,
0xF0,0x07,0xE0,0x0F,0x0C,0x00,0x06,0x08,0xFE,0xFF,0x3F,0x00,0x43,0x00,0x80,0x91,
0x00,0xC0,0x18,0x01,0x60,0x0C,0x06,0x10,0x46,0x7C,0x0C,0xE1,0x10,0xC2,0x30,0x00,
0x30,0x18,0x00,0x00,0x06,0x06,0x80,0x81,0x03,0x60,0xE0,0x00,0x00,0x38,0x00,0x00,
0x07,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,/*"参",21*/
/* (24 X 24 , 宋体 )*/

"数",0x00,0x00,0x00,0x80,0x80,0x00,0x88,0x88,0x01,0x90,0x88,0x00,0xB0,0xC4,0x00,0x90,
0x42,0x00,0xFC,0xDF,0x3F,0xC0,0x60,0x08,0xE0,0x63,0x08,0xA0,0x64,0x08,0x90,0x5C,
0x0C,0x8C,0x50,0x0C,0x82,0x80,0x0C,0xC0,0x80,0x04,0xFE,0x8F,0x04,0x60,0x84,0x07,
0x20,0x04,0x03,0x70,0x02,0x03,0x80,0x87,0x07,0x80,0xCD,0x0C,0x60,0x68,0x38,0x18,
0x18,0x30,0x06,0x04,0x00,0x00,0x00,0x00,/*"数",22*/
/* (24 X 24 , 宋体 )*/

};


typedef struct 
{
	unsigned char Index[2];	
	unsigned char Msk[128];
}typFNT_GB32; 
const typFNT_GB32 FONT_GB32[]={
"思",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x07,
0xE0,0xFF,0xFF,0x07,0xE0,0x80,0x03,0x06,0xE0,0x80,0x03,0x06,0xE0,0x80,0x03,0x06,
0xE0,0x80,0x03,0x06,0xE0,0xFF,0xFF,0x07,0xE0,0xFF,0xFF,0x07,0xE0,0x80,0x03,0x06,
0xE0,0x80,0x03,0x06,0xE0,0x80,0x03,0x06,0xE0,0x80,0x03,0x06,0xE0,0xFF,0xFF,0x07,
0xE0,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,
0xE0,0x8C,0x03,0x06,0x70,0x0C,0x07,0x0E,0x70,0x0C,0x0E,0x1E,0x38,0x0C,0x06,0x1C,
0x38,0x0C,0x00,0x38,0x1C,0x0C,0x80,0x31,0x1C,0x0C,0xC0,0x11,0x0C,0x0C,0xC0,0x01,
0x00,0xFC,0xFF,0x01,0x00,0xFC,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"思",0*/
"迈",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,
0x70,0xF8,0xFF,0x3F,0xE0,0xF8,0xFF,0x3F,0xE0,0x00,0x06,0x00,0xC0,0x01,0x06,0x00,
0xC0,0x01,0x06,0x00,0x40,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0xFE,0x0F,
0xFE,0x01,0xFE,0x0F,0xFE,0x01,0x06,0x0E,0xFE,0x01,0x07,0x0E,0xC0,0x01,0x07,0x0E,
0xC0,0x01,0x07,0x0E,0xC0,0x01,0x03,0x0E,0xC0,0x81,0x03,0x0E,0xC0,0x81,0x03,0x0E,
0xC0,0xC1,0x01,0x0E,0xC0,0xC1,0x01,0x06,0xC0,0xE1,0x00,0x06,0xC0,0x71,0x00,0x07,
0xC0,0x79,0xE0,0x07,0xC0,0x31,0xE0,0x03,0xF0,0x03,0x00,0x00,0x78,0x1F,0x00,0x00,
0x3C,0xFC,0xFF,0x7F,0x1C,0xF0,0xFF,0x3F,0x08,0x00,0xFE,0x3F,0x00,0x00,0x00,0x00,/*"迈",1*/
"微",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xE0,0xC0,0x00,0xC0,0x60,0xC0,0x01,
0xE0,0x60,0xC0,0x01,0x70,0x6C,0xC6,0x00,0x38,0x6C,0xC6,0x00,0x1C,0x6C,0xC6,0x00,
0x1C,0x6C,0xE6,0x00,0x88,0x6C,0xE6,0x3F,0xC0,0xFD,0xE7,0x3F,0xC0,0xFC,0x67,0x0C,
0xE0,0x00,0x30,0x0C,0x60,0x00,0x30,0x0C,0x70,0xFE,0x7F,0x0C,0x78,0xFE,0x7F,0x0C,
0x7C,0x00,0x7C,0x0C,0x6E,0x00,0x60,0x0E,0x64,0xFC,0x61,0x06,0x60,0xFC,0x61,0x06,
0x60,0x8C,0xC1,0x06,0x60,0x8C,0xC1,0x07,0x60,0x8C,0xC1,0x03,0x60,0x8C,0x8D,0x03,
0x60,0x8C,0xCF,0x03,0x60,0x8C,0xE7,0x03,0x60,0x8E,0xF1,0x0F,0x60,0x07,0x78,0x1E,
0x60,0x06,0x1E,0x3C,0x60,0x00,0x0F,0x18,0x60,0x00,0x04,0x00,0x00,0x00,0x00,0x00,/*"微",2*/
"电",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0xE0,0x00,0x00,
0x00,0xE0,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0xE0,0x00,0x00,0xF8,0xFF,0xFF,0x03,
0xF8,0xFF,0xFF,0x03,0x38,0xE0,0x80,0x03,0x38,0xE0,0x80,0x03,0x38,0xE0,0x80,0x03,
0x38,0xE0,0x80,0x03,0x38,0xE0,0x80,0x03,0xF8,0xFF,0xFF,0x03,0xF8,0xFF,0xFF,0x03,
0x38,0xE0,0x80,0x03,0x38,0xE0,0x80,0x03,0x38,0xE0,0x80,0x03,0x38,0xE0,0x80,0x03,
0x38,0xE0,0x80,0x03,0xF8,0xFF,0xFF,0x03,0xF8,0xFF,0xFF,0x03,0x38,0xE0,0x00,0x00,
0x38,0xE0,0x00,0x00,0x38,0xE0,0x00,0x00,0x00,0xE0,0x00,0x70,0x00,0xE0,0x00,0x38,
0x00,0xE0,0x01,0x38,0x00,0xC0,0xFF,0x3F,0x00,0x80,0xFF,0x1F,0x00,0x00,0x00,0x00,/*"电",3*/
"子",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xE0,0xFF,0xFF,0x07,0xE0,0xFF,0xFF,0x07,0x00,0x00,0xC0,0x03,0x00,0x00,0xF0,0x00,
0x00,0x00,0x3C,0x00,0x00,0x00,0x1E,0x00,0x00,0x80,0x07,0x00,0x00,0x80,0x03,0x00,
0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0xFC,0xFF,0xFF,0x3F,
0xFC,0xFF,0xFF,0x3F,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,
0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,
0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0xC0,0x01,0x00,
0x00,0xF8,0x01,0x00,0x00,0xF8,0x01,0x00,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00,/*"子",4*/
};

#endif


