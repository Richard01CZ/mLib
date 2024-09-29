// Datablock.h
//
// CDatablock Definition file
//
// CDatablock class is a part of the MafiaLib Engine
//
// Copyright (c) 2003 ASM

#include <windows.h>
#include "helper.h"

#pragma once

// Datablock constants

// Scene2 General Property Datablocks
const short GENERAL_SCENE2_MAIN=0x4C53;						//0x534C
const short GENERAL_SCENE2_UNK_1=0xAFFF;					//0xFFAF
const short GENERAL_SCENE2_UNK_2=0x3200;					//0x0032
const short GENERAL_SCENE2_CAMERA=0x3010;					//0x1030
const short GENERAL_SCENE2_VIEW=0x3011;						//0x1130
const short GENERAL_SCENE2_VIEW_SPECIAL=0x3211;				//0x1132

// Neutral Datablocks
const short DATABLOCK_COMMENT=0x0001;						//0x0100
const short DATABLOCK_UNKNOWN=0xFFFF;						//makes it easier to find unknown datablocks
const short DATABLOCK_NOMATTER=0x0000;
const short DATABLOCK_PROTECTION1=0xA922;
const short DATABLOCK_PROTECTION2=0x3A33;

// Object Declaration Datablock
const short NORMAL_OBJECT_OR_SECTOR_NAME=0x0010;			//0x1000
const short NORMAL_OBJECT_POS=0x0020;						//0x2000
const short NORMAL_OBJECT_ROT=0x0022;						//0x2200
const short NORMAL_OBJECT_DIMENSION=0x002D;					//0x2D00
const short NORMAL_OBJECT_ADDTRANSLATION=0x002C;			//0x2C00
const short NORMAL_OBJECT_SECTOR=0x4020;					//0x2040
const short NORMAL_OBJECT_MODEL=0x2012;						//0x1220
const short NORMAL_OBJECT_MAIN=0x4010;						//0x1040	//Big Datablock for new Object
const short NORMAL_OBJECT_TYPE=0x4011;						//0x1140
const short NORMAL_OBJECT_TYPE_06_DATA=0x4050;				//0x5040
const short NORMAL_LENSFLARE_OR_LIGHTMAP_OR_GLOW=0x4090;	//0x9040
const short NORMAL_LIGHTMAP_DATA=0x40A0;					//0xA040
const short NORMAL_LIGHTNING_MAIN=0x4040;					//0x4040
const short NORMAL_LIGHTNING_TYPE=0x4041;					//0x4140
const short NORMAL_LIGHTNING_COLOR=0x0026;					//0x2600
const short NORMAL_LIGHTNING_POWER=0x4042;					//0x4240
const short NORMAL_LIGHTNING_UNK_1=0x4043;					//0x4340
const short NORMAL_LIGHTNING_RANGE=0x4044;					//0x4440
const short NORMAL_LIGHTNING_FLAGS=0x4045;					//0x4540
const short NORMAL_LIGHTNING_SECTOR=0x4046;					//0x4640
const short NORMAL_MAIN=0x4000;								//0x0040 //1st main buffer with object defs
const short NORMAL_SECTOR_MAIN=0xB401;						//0x01B4
const short NORMAL_SECTOR_UNK_1=0x000B;						//0x0B00
const short NORMAL_SECTOR_UNK_2=0x0000;						//0x0000
const short NORMAL_SECTOR_UNK_3=0x0006;						//0x0600
const short NORMAL_SECTOR_UNK_4=0x0007;						//0x0700
const short NORMAL_SECTOR_UNK_5=0x0004;						//0x0400
const short NORMAL_SECTOR_UNK_6=0x0005;						//0x0500
const short NORMAL_SECTOR_UNK_7=0x0008;						//0x0800
const short NORMAL_SECTOR_UNK_8=0x0002;						//0x0200
const short NORMAL_SECTOR_UNK_9=0x0003;						//0x0300
const short NORMAL_SECTOR_UNK_10=0x000A;					//0x0A00
const short NORMAL_SECTOR_UNK_11=0x000C;					//0x0C00
const short NORMAL_SECTOR_UNK_12=0x000D;					//0x0D00
const short NORMAL_SECTOR_UNK_13=0x000E;					//0x0E00
const short NORMAL_SECTOR_UNK_14=0x000F;					//0x0F00
const short NORMAL_SECTOR_UNK_15=0x0011;					//0x1100
// #define NORMAL_SECTOR_UNK_16							0x1000  //senseless since 0x1000=NORMAL_OBJECT_SCETOR_NAME
const short NORMAL_SECTOR_UNK_17=0x0012;		//0x1200
const short NORMAL_SECTOR_UNK_18=0x0013;		//0x1300
const short NORMAL_SECTOR_UNK_19=0x0009;		//0x0900
const short NORMAL_SECTOR_UNK_20=0x0015;		//0x1500
const short NORMAL_SECTOR_UNK_21=0x0014;		//0x1400
const short NORMAL_SOUND_MAIN=0x4060;		//0x6040
const short NORMAL_SOUND_VOLUME=0x4061;		//0x6140
const short NORMAL_SOUND_HMTTP=0x4062;		//0x6240 //How many times to play
const short NORMAL_SOUND_ATBEGIN=0x4063;		//0x6340
const short NORMAL_SOUND_UNK_1=0x4064;		//0x6440
const short NORMAL_SOUND_UNK_2=0x4068;		//0x6840
const short NORMAL_SOUND_UNK_3=0x4067;		//0x6740 //this was the very small datablock
const short NORMAL_SOUND_UNK_4=0xB800;		//0x00B8
const short NORMAL_SOUND_SECTOR=0xB200;		//0x00B2
const short NORMAL_LENSFLARE_OR_GLOW_MAIN=0xB110;		//0x10B1
const short NORMAL_LENSFLARE_OR_GLOW_SIZE=0xB112;		//0x12B1
const short NORMAL_LENSFLARE_OR_GLOW_COLORS=0xB113;		//0x13B1
const short NORMAL_LENSFLARE_OR_GLOW_WAIT=0xB114;		//0x14B1
const short NORMAL_LENSFLARE_OR_GLOW_CSPEED=0xB115;		//0x15B1
const short NORMAL_LENSFLARE_OR_GLOW_DSPEED=0xB116;		//0x16B1
const short NORMAL_LENSFLARE_OR_GLOW_UNK_1=0xB117;		//0x17B1

// Object Definition Datablock
const short  SPECIAL_MAIN=0xAE20;		//0x20AE //2nd big datablock
const short  SPECIAL_OBJECT_MAIN=0xAE21;		//0x21AE
const short  SPECIAL_OBJECT_NAME=0xAE23;		//0x23AE
const short  SPECIAL_OBJECT_TYPE=0xAE22;		//0x22AE
const short  SPECIAL_OBJECT_DATA=0xAE24;		//0x24AE

// Additional Datablock
const short  ADDITIONAL_MAIN=0xAE50;		//0x50AE
const short  ADDITIONAL_OBJECT=0xAE51;		//0x51AE //big buffer which contains name and script of object in (unusual)

// Unknown Datablock
const short UNKNOWN_DATABLOCK_MAIN=0xAE02;
const short UNKNOWN_DATABLOCK_NAME=0xAE03;

// General Object Types
#define GENERAL_OBJECT_TYPE_UNKNOWN						0xEEEEEEEE

// Object Types (Declaration Block)
#define NORMAL_OBJECT_TYPE_LIGHT						0x02
#define NORMAL_OBJECT_TYPE_CAMERA						0x03
#define NORMAL_OBJECT_TYPE_SOUND						0x04
#define NORMAL_OBJECT_TYPE_06							0x06	
#define NORMAL_OBJECT_TYPE_MODEL						0x09
#define NORMAL_OBJECT_TYPE_OCCLUDER						0x0C

// Object Types (IMAGINARY - NOT REAL/PROVEN)
#define NORMAL_OBJECT_TYPE_SECTOR						0x99
#define NORMAL_OBJECT_TYPE_LIGHTMAP						0x9A
#define ADDITIONAL_OBJECT_TYPE_SCRIPT					0x9B
#define UNKNOWN_OBJECT_TYPE_UNKNOWN						0x9C

// Object Types (Declaration Block) - for AddObject and EditObject funcs
#define S_NORMAL_OBJECT_TYPE_LIGHT						0xF2
#define S_NORMAL_OBJECT_TYPE_CAMERA						0xF3
#define S_NORMAL_OBJECT_TYPE_SOUND						0xF4
#define S_NORMAL_OBJECT_TYPE_06							0xF6	// S_ constants necessary to avoid case problems during compilation
#define S_NORMAL_OBJECT_TYPE_MODEL						0xF9
#define S_NORMAL_OBJECT_TYPE_OCCLUDER					0xFC

// Object Types (Definition Block)
#define SPECIAL_OBJECT_TYPE_VISOBJ						0x01
#define SPECIAL_OBJECT_TYPE_PLAYER						0x02
#define SPECIAL_OBJECT_TYPE_CAR							0x04
#define SPECIAL_OBJECT_TYPE_SCRIPT						0x05
#define SPECIAL_OBJECT_TYPE_DOOR						0x06
#define SPECIAL_OBJECT_TYPE_PUB_VEHICLE					0x08
#define SPECIAL_OBJECT_TYPE_TRAFFIC						0x0C
#define SPECIAL_OBJECT_TYPE_CHARACTER					0x1B
#define SPECIAL_OBJECT_TYPE_PEDS						0x12
#define SPECIAL_OBJECT_TYPE_SHOOTABLE					0x23
#define SPECIAL_OBJECT_TYPE_DOG							0x15
#define SPECIAL_OBJECT_TYPE_PLANE						0x16
#define SPECIAL_OBJECT_TYPE_RAIL_ROUTE					0x18
#define SPECIAL_OBJECT_TYPE_PUMPER						0x19
#define SPECIAL_OBJECT_TYPE_RACECAM						0x1C
#define SPECIAL_OBJECT_TYPE_WAGONS						0x1E
#define SPECIAL_OBJECT_TYPE_CLOCKS						0x22
#define SPECIAL_OBJECT_TYPE_TRUCKBACKSEAT				0x24

// Light Types
#define NORMAL_LIGHT_TYPE_POINT							0x01
#define NORMAL_LIGHT_TYPE_DIRECTIONAL					0x03
#define NORMAL_LIGHT_TYPE_AMBIENT						0x04
#define NORMAL_LIGHT_TYPE_FOG							0x05
#define NORMAL_LIGHT_TYPE_POINT_AMBIENT					0x06
#define NORMAL_LIGHT_TYPE_LAYERED_FOG					0x08

// other stuff

const short SCENE2_EOF=0xFAFA;
const short SCENE2_NULL=0;

#pragma once

class CDatablock
{
public:
	CDatablock(void);
	~CDatablock(void);
	// Reads a long
	long GetLong(HANDLE hFile);
	// Reads a short
	short GetShort(HANDLE hFile);
	// Reads out a float
	float GetFloat(HANDLE hFile);
	// Looks for a particular datablock in Scene2.bin file and, if found, returns the offset of the biginning of the datablock
	unsigned long FindDatablock(HANDLE hFile, short nType);
	// Creates a new datablock
	void CreateDatablock(HANDLE hFile, short nType, long lSize);

private:
	CHelper myHelper;
public:
	// Finds subdatablocks like 22AE, 23AE, etc.....
	long FindSubDatablock(HANDLE hFile, short nType, long lBeginOffset);
	// Reads a byte
	char GetByte(HANDLE hFile);
	// Looks for datablocks belonging to objects
	long FindObjectDatablock(HANDLE hFile, short nType, long lBeginOffset);
	// Creates a datablock in file specified by hFile and fills it with random data
	int CreateProtectionDatablock(HANDLE hFile, short nProtDatablockType, unsigned long lSize);
};
