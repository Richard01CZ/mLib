// Scene2.cpp
//
// CScene2 class implementation file
//
// CScene2 class is a part of the MafiaLib Engine
//
// Copyright (c) 2003, 2004 ASM

// #include "StdAfx.h"
#include "scene2.h"
#include "Helper.h"
#include "datablock.h"
#include <stdlib.h>
#include <conio.h>
#include <time.h>

CHelper myHelper;
CDatablock thisDatablock;

// Build
#define MAFIALIB_BUILD			1308

// Version (Major)
// #define MAFIALIB_MAJ_VERSION	1

// Version (Minor)
// #define MAFIALIB_SUB_VERSION	3

// Version (All)
#define MAFIALIB_VERSION	1.30

//#define PRIVATE		1
//#define PUBLIC		2

#ifdef PRIVATE
	#define COPYRIGHT "MafiaLib Engine v"MAFIALIB_VERSION" Copyright (c) 2003, 2004 ASM - BUILD "MAFIALIB_BUILD" Private"
#elif PUBLIC
	#define COPYRIGHT "MafiaLib Engine v"MAFIALIB_VERSION" Copyright (c) 2003, 2004 ASM - BUILD "MAFIALIB_BUILD" Public"
#endif

// protection datablocks
#define PROTBLOCKNUM	2
const short nProtBlocks[PROTBLOCKNUM]=		// this is a list for all protection datablocks which can be used by the function.
{
	DATABLOCK_PROTECTION1,
	DATABLOCK_PROTECTION2
};

// random number
#define random(min,max) (rand()%((int)(((max)+1)-min)+min))

// Predefinied Objects

// car object - name: <tmp>
#define CARDATASIZE		35
unsigned char cardata[CARDATASIZE] = {
	0x21, 0xAE, 0x23, 0x00, 0x00, 0x00, 0x23, 0xAE, 0x0C, 0x00, 0x00, 0x00, 0x3C, 0x74, 0x6D, 0x70, 
	0x3E, 0x00, 0x22, 0xAE, 0x0A, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x24, 0xAE, 0x07, 0x00, 
	0x00, 0x00, 0x01
};

#define SCRIPTDATASIZE	47
unsigned char scriptdata[SCRIPTDATASIZE] = {
	0x21, 0xAE, 0x2F, 0x00, 0x00, 0x00, 0x23, 0xAE, 0x0B, 0x00, 0x00, 0x00, 0x3C, 0x73, 0x74, 0x3E, 
	0x00, 0x22, 0xAE, 0x0A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x24, 0xAE, 0x14, 0x00, 0x00, 
	0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define INITSCRIPTDATASIZE	46
unsigned char initscriptdata[INITSCRIPTDATASIZE] = {
	0x51, 0xAE, 0x2E, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x00, 0x00, 0x00, 0x3C, 0x69, 0x6E, 0x69, 0x74, 
	0x2D, 0x74, 0x6D, 0x70, 0x3E, 0x15, 0x00, 0x00, 0x00, 0x59, 0x6F, 0x75, 0x72, 0x20, 0x73, 0x63, 
	0x72, 0x69, 0x70, 0x74, 0x20, 0x67, 0x6F, 0x65, 0x73, 0x20, 0x68, 0x65, 0x72, 0x65
};

CScene2::CScene2(void)
{		
}

CScene2::~CScene2(void)
{
}

// Function	: LoadScene2Bin
//
// Purpose	: Loads a scene2.bin file for reading

HANDLE CScene2::LoadScene2Bin(char * szFilename)
{
//	HANDLE hFile=CreateFile(szFilename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_WRITE|FILE_SHARE_READ|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	HANDLE hFile=CreateFile(szFilename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	return(hFile);
}

// Function	: CloseScene2Bin
//
// Purpose	: Closes an open scene2.bin file
void CScene2::CloseScene2Bin(HANDLE hFile)
{
	if(hFile!=0 && hFile!=INVALID_HANDLE_VALUE)
		CloseHandle(hFile);
}

// Function	: IsScene2Bin
//
// Purpose	: Checks if opened file is a scene2.bin file
bool CScene2::IsScene2Bin(HANDLE hFile)
{
	short nSig;
	unsigned long lBytesRead;

	SetFilePointer(hFile,0,0,NULL);
	ReadFile(hFile,(char *)&nSig,sizeof(nSig),&lBytesRead,NULL);
	if(nSig==GENERAL_SCENE2_MAIN)
		return true;
	return false;
}

// Function	: GetSize
//
// Purpose	: Reads size of scene2.bin file found in header

unsigned long CScene2::GetFileSize(HANDLE hFile)
{
	long lSize=0;
	unsigned long lBytesRead;

	SetFilePointer(hFile,2,0,NULL);
	ReadFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesRead,NULL);
	return(lSize);
}

// Reads out the distance between the cam and the player
float CScene2::GetCamDist(HANDLE hFile)
{
	thisDatablock.FindDatablock(hFile,GENERAL_SCENE2_CAMERA);
	SetFilePointer(hFile,4,0,FILE_CURRENT);
	return(thisDatablock.GetFloat(hFile));
}

// Reads out the viewdistance
float CScene2::GetViewDist(HANDLE hFile)
{
	thisDatablock.FindDatablock(hFile,GENERAL_SCENE2_VIEW);
	SetFilePointer(hFile,4,0,FILE_CURRENT);
	return(thisDatablock.GetFloat(hFile));
}

// Used for modifying the camera distance
long CScene2::SetCamDist(HANDLE hFile, float fCamDist)
{
	unsigned long lBytesWritten=0;
	thisDatablock.FindDatablock(hFile,GENERAL_SCENE2_CAMERA);
	SetFilePointer(hFile,4,0,FILE_CURRENT);
	WriteFile(hFile,(char *)&fCamDist,sizeof(fCamDist),&lBytesWritten,NULL);
	return(lBytesWritten);
}

// Used for modifying the viewdistance
long CScene2::SetViewDist(HANDLE hFile, float fViewDist)
{
	unsigned long lBytesWritten=0;
	thisDatablock.FindDatablock(hFile,GENERAL_SCENE2_VIEW);
	SetFilePointer(hFile,4,0,FILE_CURRENT);
	WriteFile(hFile,(char *)&fViewDist,sizeof(fViewDist),&lBytesWritten,NULL);
	return(lBytesWritten);
}



// Used for reading the (probably) wolrd dimension
float CScene2::GetWorldDim(HANDLE hFile, bool bReadFirst)
{
	thisDatablock.FindDatablock(hFile,GENERAL_SCENE2_VIEW_SPECIAL);
	if(bReadFirst)
		SetFilePointer(hFile,4,0,FILE_CURRENT);
	else
		SetFilePointer(hFile,8,0,FILE_CURRENT);
	return(thisDatablock.GetFloat(hFile));
}

// Writes the world dimension
long CScene2::SetWorldDim(HANDLE hFile, float fDim1, float fDim2)
{
	unsigned long lBytesWritten=0;
	thisDatablock.FindDatablock(hFile,GENERAL_SCENE2_VIEW_SPECIAL);
	SetFilePointer(hFile,4,0,FILE_CURRENT);
	WriteFile(hFile,(char *)&fDim1,sizeof(fDim1),&lBytesWritten,NULL);
	WriteFile(hFile,(char *)&fDim2,sizeof(fDim2),&lBytesWritten,NULL);
	return(lBytesWritten);
}

// Used for finding objects
long CScene2::FindObject(HANDLE hFile, char * szObjectName, short nDatablockType)
{
	CDatablock thisDatablock;
	short nType=DATABLOCK_UNKNOWN;
	long lMainSize=0, lSubSize=0, lObjectOffsetMain=0, lObjectOffsetDummy=0;
	unsigned long lBytesRead=0;
	char szObjectNameDummy[256];	// 256 chars max

	// first check if nDatablockType is set correctly
	if((nDatablockType==NORMAL_MAIN)||(nDatablockType==SPECIAL_MAIN)||(nDatablockType==ADDITIONAL_MAIN)||(nDatablockType==0))
	{
		// First set file pointer to 6 bytes after file begin
		SetFilePointer(hFile,6,0,FILE_BEGIN);
		lObjectOffsetDummy=6;
	
		while(1)
		{
			nType=thisDatablock.GetShort(hFile);
			if(nType==SCENE2_EOF)
				return(-1);
			lMainSize=thisDatablock.GetLong(hFile);

			switch(nType)
			{
				case NORMAL_OBJECT_MAIN:
					// first check if the object is to be searched in the NORMAL_MAIN
					// datablock
					if((nDatablockType==SPECIAL_MAIN) || (nDatablockType==ADDITIONAL_MAIN))
					{
						SetFilePointer(hFile,(lMainSize-6),0,FILE_CURRENT);
						lObjectOffsetDummy+=lMainSize;
						break;
					}
					// First save offset of object
					lObjectOffsetMain=lObjectOffsetDummy;
					lObjectOffsetDummy=lObjectOffsetMain+lMainSize;
	
					// if it was one of the object main buffers, the name is situated directly
					// after the main datablock. However we check if the upcoming datablock
					// is the name datablock
					nType=thisDatablock.GetShort(hFile);
					if((nType==NORMAL_OBJECT_OR_SECTOR_NAME) || (nType==SPECIAL_OBJECT_NAME))
					{
						
						lSubSize=thisDatablock.GetLong(hFile);
						lSubSize-=6;	// to get effective size
						ReadFile(hFile,szObjectNameDummy,lSubSize,&lBytesRead,NULL);
						if(!strcmp(szObjectNameDummy,szObjectName))
							return(lObjectOffsetMain);
						// Put file pointer to next object
						SetFilePointer(hFile,(lMainSize-(lSubSize+6)-6),0,FILE_CURRENT);
					}
					else
					{
						SetFilePointer(hFile,10,0,FILE_CURRENT);
						lSubSize=thisDatablock.GetLong(hFile);
						lSubSize-=6;	// to get effective size
						ReadFile(hFile,szObjectNameDummy,lSubSize,&lBytesRead,NULL);
						if(!strcmp(szObjectNameDummy,szObjectName))
							return(lObjectOffsetMain);
						// Put file pointer to next object
						SetFilePointer(hFile,(lMainSize-(lSubSize+6)-16),0,FILE_CURRENT);
					}
					break;

				case SPECIAL_OBJECT_MAIN:
					// first check if the object is to be searched in the SPECIAL_MAIN
					// datablock
					if((nDatablockType==NORMAL_MAIN) || (nDatablockType==ADDITIONAL_MAIN))
					{
						SetFilePointer(hFile,(lMainSize-6),0,FILE_CURRENT);
						lObjectOffsetDummy+=lMainSize;
						break;
					}
					// First save offset of object
					lObjectOffsetMain=lObjectOffsetDummy;
					lObjectOffsetDummy=lObjectOffsetMain+lMainSize;

					// if it was one of the object main buffers, the name is situated directly
					// after the main datablock. However we check if the upcoming datablock
					// is the name datablock
					nType=thisDatablock.GetShort(hFile);
					if((nType==NORMAL_OBJECT_OR_SECTOR_NAME) || (nType==SPECIAL_OBJECT_NAME))
					{
					
						lSubSize=thisDatablock.GetLong(hFile);
						lSubSize-=6;	// to get effective size
						ReadFile(hFile,szObjectNameDummy,lSubSize,&lBytesRead,NULL);
						if(!strcmp(szObjectNameDummy,szObjectName))
							return(lObjectOffsetMain);
						// Put file pointer to next object
						SetFilePointer(hFile,(lMainSize-(lSubSize+6)-6),0,FILE_CURRENT);
					}
					else
					{
						SetFilePointer(hFile,10,0,FILE_CURRENT);
						lSubSize=thisDatablock.GetLong(hFile);
						lSubSize-=6;	// to get effective size
						ReadFile(hFile,szObjectNameDummy,lSubSize,&lBytesRead,NULL);
						if(!strcmp(szObjectNameDummy,szObjectName))
							return(lObjectOffsetMain);
						// Put file pointer to next object
						SetFilePointer(hFile,(lMainSize-(lSubSize+6)-16),0,FILE_CURRENT);
					}
					break;

				case ADDITIONAL_OBJECT:
					// first check if the object is to be searched in the ADDITIONAL_MAIN
					// datablock
					if((nDatablockType==NORMAL_MAIN)||(nDatablockType==SPECIAL_MAIN))
					{
						SetFilePointer(hFile,(lMainSize-6),0,FILE_CURRENT);
						lObjectOffsetDummy+=lMainSize;
						break;
					}
					// First save offset of object
					lObjectOffsetMain=lObjectOffsetDummy;
					lObjectOffsetDummy=lObjectOffsetMain+lMainSize;
					
					SetFilePointer(hFile,1,0,FILE_CURRENT);
					lSubSize=thisDatablock.GetLong(hFile);
					ReadFile(hFile,szObjectNameDummy,lSubSize,&lBytesRead,NULL);
					if(!myHelper.Compare(szObjectNameDummy,szObjectName,strlen(szObjectName)-1))
						return(lObjectOffsetMain);
					// Put file pointer to next object
					SetFilePointer(hFile,lMainSize-lSubSize-11,0,FILE_CURRENT);
					break;

				case NORMAL_MAIN:
				case SPECIAL_MAIN:
				case ADDITIONAL_MAIN:
					lObjectOffsetDummy+=6;
					break;

				default:
					SetFilePointer(hFile,(lMainSize-6),0,FILE_CURRENT);
					lObjectOffsetDummy+=lMainSize;
					break;
			}
		}
	}
	else
	{
		return(-1);
	}
	return(-1);
}

// Reads out all object properties
bool CScene2::ReadObject(HANDLE hFile, long lOffset, ObjectInfo_t * Scene2ObjectInfo_t, bool bUseFilter)
{
	CDatablock thisDatablock;
	short nType=DATABLOCK_UNKNOWN;
	long lSize=0,
		lDummySize=0;
	short dummy=0;
	unsigned long lBytesRead=0;
	int cur=0, i;

	Scene2ObjectInfo_t->bUseModel=false;

	// critical initalization ;)
	// init sSectorName with 0
	Scene2ObjectInfo_t->sSectorName[0]=0;

	// init sModelName with 0
	Scene2ObjectInfo_t->sModelName[0]=0;

	// init bSector with 0
	Scene2ObjectInfo_t->bSector=false;

	// init bExtended with 0
	Scene2ObjectInfo_t->bExtended=false;

	SetFilePointer(hFile,(lOffset+6),0,FILE_BEGIN);

	while(1)
	{
		nType=thisDatablock.GetShort(hFile);
		if((nType==SCENE2_EOF) || (nType==SCENE2_NULL))
			return false;

		lSize=thisDatablock.GetLong(hFile);
		if(!lSize)
			return false;

		switch(nType)
		{
			case NORMAL_OBJECT_MAIN:
			case SPECIAL_OBJECT_MAIN:
			case NORMAL_MAIN:
			case SPECIAL_MAIN:
//			case DATABLOCK_COMMENT:
			case UNKNOWN_DATABLOCK_MAIN:
			case ADDITIONAL_MAIN:
				return true;

			// a normal object
			case NORMAL_OBJECT_TYPE:
			case SPECIAL_OBJECT_TYPE:
				Scene2ObjectInfo_t->lType=thisDatablock.GetLong(hFile);
				break;

			case ADDITIONAL_OBJECT:
				SetFilePointer(hFile,1,0,FILE_CURRENT);
				lDummySize=thisDatablock.GetLong(hFile);
				ReadFile(hFile,Scene2ObjectInfo_t->sName,lDummySize,&lBytesRead,NULL);
				break;

			case NORMAL_OBJECT_OR_SECTOR_NAME:
			case SPECIAL_OBJECT_NAME:
				lSize-=6;
				ReadFile(hFile,Scene2ObjectInfo_t->sName,lSize,&lBytesRead,NULL);
				break;

			case NORMAL_OBJECT_POS:
				for(i=0;i<3;i++)
					Scene2ObjectInfo_t->fPos[i]=thisDatablock.GetFloat(hFile);
				break;

			case NORMAL_OBJECT_ROT:
				for(i=0;i<4;i++)
					Scene2ObjectInfo_t->fRot[i]=thisDatablock.GetFloat(hFile);
				break;

			case NORMAL_OBJECT_DIMENSION:
				for(i=0;i<3;i++)
					Scene2ObjectInfo_t->fSize[i]=thisDatablock.GetFloat(hFile);
				break;

			case NORMAL_OBJECT_ADDTRANSLATION:
				for(i=0;i<3;i++)
					Scene2ObjectInfo_t->fAddTrans[i]=thisDatablock.GetFloat(hFile);
				break;

			case NORMAL_OBJECT_SECTOR:
				SetFilePointer(hFile,2,0,FILE_CURRENT);
				lSize=thisDatablock.GetLong(hFile);
				lSize-=6;
				ReadFile(hFile,Scene2ObjectInfo_t->sSectorName,lSize,&lBytesRead,NULL);
				Scene2ObjectInfo_t->bSector=true;
				break;

			case NORMAL_OBJECT_MODEL:
				Scene2ObjectInfo_t->bUseModel=true;
				lSize-=6;
				ReadFile(hFile,Scene2ObjectInfo_t->sModelName,lSize,&lBytesRead,NULL);
				break;

			case NORMAL_SECTOR_MAIN:
				Scene2ObjectInfo_t->lType=NORMAL_OBJECT_TYPE_SECTOR;
				// in sectors we read all data directly till we come to a new object to avoid problems
//				SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);	// we still need it as long as there's no code here
				// read out sector data
				while(1)
				{
					nType=thisDatablock.GetShort(hFile);
					if((nType==SCENE2_EOF)||
						(nType==NORMAL_OBJECT_MAIN)||
						(nType==SPECIAL_OBJECT_MAIN)||
						(nType==NORMAL_MAIN)||
						(nType==SPECIAL_MAIN)||
						(nType==UNKNOWN_DATABLOCK_MAIN))
					{
						SetFilePointer(hFile,-2,0,FILE_CURRENT);
						break;
					}

					lSize=thisDatablock.GetLong(hFile);

					switch(nType)
					{
						case NORMAL_SECTOR_UNK_1:
							Scene2ObjectInfo_t->fSectorVal1=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_SECTOR_UNK_2:
							Scene2ObjectInfo_t->fSectorVal2=thisDatablock.GetFloat(hFile);
							break;
						case DATABLOCK_COMMENT:
							Scene2ObjectInfo_t->fSectorVal3=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_SECTOR_UNK_3:
							Scene2ObjectInfo_t->fSectorVal4=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_SECTOR_UNK_4:
							Scene2ObjectInfo_t->lSectorVal5=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_5:
							Scene2ObjectInfo_t->fSectorVal6=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_SECTOR_UNK_6:
							Scene2ObjectInfo_t->lSectorVal7=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_7:
							Scene2ObjectInfo_t->fSectorVal8=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_SECTOR_UNK_8:
							Scene2ObjectInfo_t->lSectorVal9=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_9:
							Scene2ObjectInfo_t->fSectorVal10=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_SECTOR_UNK_10:
							Scene2ObjectInfo_t->fSectorVal11=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_SECTOR_UNK_11:
							Scene2ObjectInfo_t->lSectorVal12=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_12:
							Scene2ObjectInfo_t->lSectorVal13=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_13:
							Scene2ObjectInfo_t->lSectorVal14=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_14:
							Scene2ObjectInfo_t->lSectorVal15=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_15:
							Scene2ObjectInfo_t->fSectorVal16=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_OBJECT_OR_SECTOR_NAME:
							Scene2ObjectInfo_t->lSectorVal17=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_17:
							Scene2ObjectInfo_t->fSectorVal18=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_SECTOR_UNK_18:
							Scene2ObjectInfo_t->fSectorVal19=thisDatablock.GetFloat(hFile);
							break;
						case NORMAL_SECTOR_UNK_19:
							Scene2ObjectInfo_t->lSectorVal20=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_20:
							for(i=0;i<3;i++)
								Scene2ObjectInfo_t->lSectorVal21[i]=thisDatablock.GetLong(hFile);
							break;
						case NORMAL_SECTOR_UNK_21:
							for(i=0;i<3;i++)
								Scene2ObjectInfo_t->lSectorVal22[i]=thisDatablock.GetLong(hFile);
							break;
					}
				}
				break;

			case NORMAL_LIGHTMAP_DATA:
				Scene2ObjectInfo_t->lType=NORMAL_OBJECT_TYPE_LIGHTMAP;
				SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
				break;

			case NORMAL_LIGHTNING_MAIN:
				// do nothing :p
				break;

			case NORMAL_LIGHTNING_TYPE:
				Scene2ObjectInfo_t->lLightType=thisDatablock.GetLong(hFile);
				break;

			case NORMAL_LIGHTNING_COLOR:
				Scene2ObjectInfo_t->fRed=thisDatablock.GetFloat(hFile);
				Scene2ObjectInfo_t->fGreen=thisDatablock.GetFloat(hFile);
				Scene2ObjectInfo_t->fBlue=thisDatablock.GetFloat(hFile);
				break;

			case NORMAL_LIGHTNING_POWER:
				Scene2ObjectInfo_t->fPower=thisDatablock.GetFloat(hFile);
				break;

			case NORMAL_LIGHTNING_UNK_1:
				Scene2ObjectInfo_t->fLightUnk1=thisDatablock.GetFloat(hFile);
				Scene2ObjectInfo_t->fLightUnk2=thisDatablock.GetFloat(hFile);
				break;

			case NORMAL_LIGHTNING_RANGE:
				Scene2ObjectInfo_t->fLightNear=thisDatablock.GetFloat(hFile);
				Scene2ObjectInfo_t->fLightFar=thisDatablock.GetFloat(hFile);
				break;

			case NORMAL_LIGHTNING_FLAGS:
				Scene2ObjectInfo_t->lLightFlags=thisDatablock.GetLong(hFile);
				break;

			case NORMAL_LIGHTNING_SECTOR:
				lSize-=6;
				ReadFile(hFile,&Scene2ObjectInfo_t->sLightSectors[cur],lSize,&lBytesRead,NULL);
				cur+=lSize;
//				Scene2ObjectInfo_t->sLightSectors[cur]=';';
//				cur++;
				break;

			case NORMAL_OBJECT_TYPE_06_DATA:
				for(i=0;i<6;i++)
					Scene2ObjectInfo_t->fVal[i]=thisDatablock.GetFloat(hFile);
				Scene2ObjectInfo_t->bExtended=true;
				break;

			case SPECIAL_OBJECT_DATA:
				if(bUseFilter==true)
				{
					SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
					break;
				}
				// ok, now we have to differenciate
				switch(Scene2ObjectInfo_t->lType)
				{
					case SPECIAL_OBJECT_TYPE_SHOOTABLE:
						SetFilePointer(hFile,2,0,FILE_CURRENT);
						Scene2ObjectInfo_t->fMovVal1=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fMovVal2=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fWeight=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fMovVal3=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fMovVal4=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->lSound=thisDatablock.GetLong(hFile);
						SetFilePointer(hFile,1,0,FILE_CURRENT);
						Scene2ObjectInfo_t->lMovVal5=thisDatablock.GetLong(hFile);
						break;

					case SPECIAL_OBJECT_TYPE_RACECAM:
						Scene2ObjectInfo_t->lRaceCamVal1=thisDatablock.GetLong(hFile);
						Scene2ObjectInfo_t->fRaceCamVal2=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fRaceCamVal3=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fRaceCamVal4=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->lRaceCamVal5=thisDatablock.GetLong(hFile);
						Scene2ObjectInfo_t->lRaceCamVal6=thisDatablock.GetLong(hFile);
						break;

					case SPECIAL_OBJECT_TYPE_PEDS:
						SetFilePointer(hFile,5,0,FILE_CURRENT);
						// Read global stuff
						for(i=0;i<5;i++)
							Scene2ObjectInfo_t->fPedVal[i]=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->lGlobalPedDensity=thisDatablock.GetLong(hFile);
						Scene2ObjectInfo_t->lNumOfPeds=thisDatablock.GetLong(hFile);
						// Read Ped Model Names
						for(i=0;i<Scene2ObjectInfo_t->lNumOfPeds;i++)
						{
							lSize=myHelper.ReadTillChar(hFile,Scene2ObjectInfo_t->sPedname[i],0xCD);
							SetFilePointer(hFile,(16-lSize),0,FILE_CURRENT);
						}
						// Read Ped Densities
						for(i=0;i<Scene2ObjectInfo_t->lNumOfPeds;i++)
						{
							Scene2ObjectInfo_t->lLocalPedDensity[i]=thisDatablock.GetLong(hFile);
							Scene2ObjectInfo_t->bDel[i]=false;
						}
						break;

					case SPECIAL_OBJECT_TYPE_TRAFFIC:
						SetFilePointer(hFile,4,0,FILE_CURRENT);
						for(i=0;i<3;i++)
							Scene2ObjectInfo_t->fTrafficVal[i]=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->lDensity=thisDatablock.GetLong(hFile);
						Scene2ObjectInfo_t->lNumOfCars=thisDatablock.GetLong(hFile);
						// ignore upcoming stuff <-- Ignore this
//						SetFilePointer(hFile,(lSize-(6+(strlen(Scene2ObjectInfo_t->sName)+7)+40),0,FILE_CURRENT);
						// read out cars
						for(i=0;i<Scene2ObjectInfo_t->lNumOfCars;i++)
						{
							lSize=myHelper.ReadTillChar(hFile,Scene2ObjectInfo_t->sCarname[i],0)+1;
							SetFilePointer(hFile,(20-lSize),0,FILE_CURRENT);
							Scene2ObjectInfo_t->fDensity[i]=thisDatablock.GetFloat(hFile);
							Scene2ObjectInfo_t->lColor[i]=thisDatablock.GetLong(hFile);
							Scene2ObjectInfo_t->bPolice[i]=thisDatablock.GetByte(hFile);
							Scene2ObjectInfo_t->bFlag2[i]=thisDatablock.GetByte(hFile);
							Scene2ObjectInfo_t->bGangster[i]=thisDatablock.GetByte(hFile);
							Scene2ObjectInfo_t->bFlag4[i]=thisDatablock.GetByte(hFile);
							Scene2ObjectInfo_t->bDel[i]=false;
						}
						break;

					case SPECIAL_OBJECT_TYPE_DOOR:
						// don't read stupid 00
						SetFilePointer(hFile,5,0,FILE_CURRENT);
						Scene2ObjectInfo_t->bOpen1=thisDatablock.GetByte(hFile);
						Scene2ObjectInfo_t->bOpen2=thisDatablock.GetByte(hFile);
						Scene2ObjectInfo_t->fMoveAngle=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->bOpen=thisDatablock.GetByte(hFile);
						Scene2ObjectInfo_t->bLocked=thisDatablock.GetByte(hFile);
						Scene2ObjectInfo_t->fCloseSpeed=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fOpenSpeed=thisDatablock.GetFloat(hFile);
						// read sounds
						lSize=myHelper.ReadTillChar(hFile,Scene2ObjectInfo_t->sOpenSound,0);
						SetFilePointer(hFile,(16-lSize-1),0,FILE_CURRENT);
						lSize=myHelper.ReadTillChar(hFile,Scene2ObjectInfo_t->sCloseSound,0);
						SetFilePointer(hFile,(16-lSize-1),0,FILE_CURRENT);
						lSize=myHelper.ReadTillChar(hFile,Scene2ObjectInfo_t->sLockedSound,0);
						SetFilePointer(hFile,(16-lSize-1),0,FILE_CURRENT);
						// door flag
						Scene2ObjectInfo_t->bDoorFlag=thisDatablock.GetByte(hFile);
						break;

					case SPECIAL_OBJECT_TYPE_CHARACTER:
					case SPECIAL_OBJECT_TYPE_PLAYER:
						SetFilePointer(hFile,1,0,FILE_CURRENT);
						Scene2ObjectInfo_t->cBehaviour=thisDatablock.GetByte(hFile);
						SetFilePointer(hFile,3,0,FILE_CURRENT);
						Scene2ObjectInfo_t->cVoice=thisDatablock.GetByte(hFile);
						SetFilePointer(hFile,3,0,FILE_CURRENT);
						Scene2ObjectInfo_t->fStrenght=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fEnergy=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fEnergy_HANDL=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fEnergy_HANDR=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fEnergy_LEGL=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fEnergy_LEGR=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fReactions=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fSpeed=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fAggr=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fInt=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fShooting=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fSight=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fHearing=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fDriving=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fMass=thisDatablock.GetFloat(hFile);
						Scene2ObjectInfo_t->fBehaviour=thisDatablock.GetFloat(hFile);

						dummy=thisDatablock.GetShort(hFile);
						if(dummy==0x0101)
						{
							SetFilePointer(hFile,6,0,FILE_CURRENT);
							lSize=thisDatablock.GetLong(hFile);
							SetFilePointer(hFile,(lSize+1),0,FILE_CURRENT);
						}
						else
							SetFilePointer(hFile,-2,0,FILE_CURRENT);
						break;

					default:
						SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
						break;
				}
				break;

			default:
				SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
				break;
		}
	}
	return true;
}

// Deletes an object
int CScene2::DeleteObject(HANDLE hFile, char * szObjectName, short nDatablockType)
{
	CDatablock thisDatablock;
	CHelper myHelper;
	unsigned long /*lBytesRead=0,*/ lBytesWritten=0;
	long lObjectOffset=0,			// offset of the main datablock of the object to delete
		lNormalMainOffset=0,		// offset of NORMAL_MAIN
		lSpecialMainOffset=0,		// offset of SPECIAL_MAIN
		lAdditionalMainOffset=0,	// offset of ADDITIONAL_MAIN
		lObjDelSize=0,				// size of the datablock of the object to delete
		lMainSize=0,
		lDummyOffset=0;				// used for BufferDataFM

	// At first get some basic values
	lNormalMainOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);
	lSpecialMainOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
	lAdditionalMainOffset=thisDatablock.FindDatablock(hFile,ADDITIONAL_MAIN);

	lDummyOffset=lObjectOffset=FindObject(hFile,szObjectName,nDatablockType);
	if(lObjectOffset==-1)
		return(-1);

	// Set file pointer to datablock size
	SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);

	lObjDelSize=thisDatablock.GetLong(hFile);

	lDummyOffset+=lObjDelSize;

	SetFilePointer(hFile,(lObjDelSize-6),0,FILE_CURRENT);
//	myHelper.BufferData(hFile,"buffer.tmp",-1);
	myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
	SetFilePointer(hFile,-(lObjDelSize),0,FILE_CURRENT);
	myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

	SetEndOfFile(hFile);

	// Now change important sizes
	if((lObjectOffset>lNormalMainOffset)&&(lObjectOffset<lSpecialMainOffset))
	{
		SetFilePointer(hFile,lNormalMainOffset+2,0,FILE_BEGIN);
		lMainSize=thisDatablock.GetLong(hFile);
		lMainSize-=lObjDelSize;
		SetFilePointer(hFile,lNormalMainOffset+2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lMainSize,sizeof(lMainSize),&lBytesWritten,NULL);
	}
	else
	if((lObjectOffset>lSpecialMainOffset)&&(lObjectOffset<lAdditionalMainOffset))
	{
		SetFilePointer(hFile,lSpecialMainOffset+2,0,FILE_BEGIN);
		lMainSize=thisDatablock.GetLong(hFile);
		lMainSize-=lObjDelSize;
		SetFilePointer(hFile,lSpecialMainOffset+2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lMainSize,sizeof(lMainSize),&lBytesWritten,NULL);
	}
	else
	if(lObjectOffset>lAdditionalMainOffset)
	{
		SetFilePointer(hFile,lAdditionalMainOffset+2,0,FILE_BEGIN);
		lMainSize=thisDatablock.GetLong(hFile);
		lMainSize-=lObjDelSize;
		SetFilePointer(hFile,lAdditionalMainOffset+2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lMainSize,sizeof(lMainSize),&lBytesWritten,NULL);
	}

	SetFilePointer(hFile,2,0,FILE_BEGIN);
	lMainSize=thisDatablock.GetLong(hFile);
	lMainSize-=lObjDelSize;
	SetFilePointer(hFile,2,0,FILE_BEGIN);
	WriteFile(hFile,(char *)&lMainSize,sizeof(lMainSize),&lBytesWritten,NULL);
	return 0;
}

// Exports an object
long CScene2::ExportObject(HANDLE hFile, char * szFileName, char * szObjectName, short nDatablockType)
{
	CDatablock thisDatablock;
	CHelper thisHelper;

	long lObjectOffset=0,
		lObjDatablockSize=0,
		lDummyOffset=0;

	lDummyOffset=lObjectOffset=FindObject(hFile,szObjectName,nDatablockType);
	if(lObjectOffset==-1)
		return(-1);

	SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
	lObjDatablockSize=thisDatablock.GetLong(hFile);
	SetFilePointer(hFile,-6,0,FILE_CURRENT);
	myHelper.BufferDataFM(hFile,szFileName,lDummyOffset,lObjDatablockSize);
	return 0;
//	return(myHelper.BufferData(hFile,szFileName,(lObjDatablockSize)));
}

// Imports an object
long CScene2::ImportObject(HANDLE hFile, char * szFileName)
{
	CDatablock thisDatablock;
	CHelper thisHelper;
	short nDatablockType=DATABLOCK_UNKNOWN;
	long lObjDatablockSize=0,		// size of datablock of object to add
		lNormalMainOffset=0,		// offset of NORMAL_MAIN
		lNormalMainSize=0,			// size of NORMAL_MAIN
		lSpecialMainOffset=0,		// offset of SPECIAL_MAIN
		lSpecialMainSize=0,			// size of SPECIAL_MAIN
		lAddMainOffset=0,			// offset of ADDITIONAL_MAIN
		lAddMainSize=0,				// size ADDITIONAL_MAIN
		lScene2MainSize=0,			// size of SCENE2_MAIN
		lDummyOffset=0;				// used for BufferDataFM
	unsigned long lBytesWritten=0;
	HANDLE hDummy=CreateFile(szFileName,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if(hDummy==INVALID_HANDLE_VALUE)
		return(-1);

	nDatablockType=thisDatablock.GetShort(hDummy);
	lObjDatablockSize=thisDatablock.GetLong(hDummy);
	// Ok, we can close the file
	CloseHandle(hDummy);

	switch(nDatablockType)
	{
		case NORMAL_OBJECT_MAIN:
			// Find NORMAL_MAIN
			lDummyOffset=lNormalMainOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);
			// Go to end of NORMAL_MAIN
			SetFilePointer(hFile,(lNormalMainOffset+2),0,FILE_BEGIN);
			lNormalMainSize=thisDatablock.GetLong(hFile);
			lDummyOffset+=lNormalMainSize;
			SetFilePointer(hFile,(lNormalMainSize-6),0,FILE_CURRENT);
			// Buffer all data
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
			// Paste object
			myHelper.PasteBufferedDataFM(hFile,szFileName,false);
			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// Now change size of NORMAL_MAIN
			lNormalMainSize+=lObjDatablockSize;
			SetFilePointer(hFile,(lNormalMainOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lNormalMainSize,sizeof(lNormalMainSize),&lBytesWritten,NULL);
			break;

		case SPECIAL_OBJECT_MAIN:
			// Find SPECIAL_MAIN
			lDummyOffset=lSpecialMainOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			// Go to end of SPECIAL_MAIN
			SetFilePointer(hFile,(lSpecialMainOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			lDummyOffset+=lSpecialMainSize;
			SetFilePointer(hFile,(lSpecialMainSize-6),0,FILE_CURRENT);
			// Buffer all data
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
			// Paste object
			myHelper.PasteBufferedDataFM(hFile,szFileName,false);
			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// Now change size of SPECIAL_MAIN
			lSpecialMainSize+=lObjDatablockSize;
			SetFilePointer(hFile,(lSpecialMainOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);
			break;

		case ADDITIONAL_OBJECT:
			// Find ADDITIONAL_MAIN
			lDummyOffset=lAddMainOffset=thisDatablock.FindDatablock(hFile,ADDITIONAL_MAIN);
			// Go to end of ADDITIONAL_MAIN
			SetFilePointer(hFile,(lAddMainOffset+2),0,FILE_BEGIN);
			lAddMainSize=thisDatablock.GetLong(hFile);
			lDummyOffset+=lAddMainSize;
			SetFilePointer(hFile,(lAddMainSize-6),0,FILE_CURRENT);
			// Buffer all data
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
			// Paste object
			myHelper.PasteBufferedDataFM(hFile,szFileName,false);
			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// Now change size of ADDITIONAL_MAIN
			lAddMainSize+=lObjDatablockSize;
			SetFilePointer(hFile,(lAddMainOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lAddMainSize,sizeof(lAddMainSize),&lBytesWritten,NULL);
			break;

		default:
			// if MFC
			// AfxMessageBox("CScene2::ImportObject(): MPE file does not contain a valid object",MB_OK|MB_ICONWARNING);
			// if not MFC
			MessageBox(NULL,"CScene2::ImportObject(): MPE file does not contain a valid object","Muped",MB_OK|MB_ICONWARNING);
			break;
	}

	// At the end change size of SCENE2_MAIN
	SetFilePointer(hFile,2,0,FILE_BEGIN);
	lScene2MainSize=thisDatablock.GetLong(hFile);
	lScene2MainSize+=lObjDatablockSize;
	SetFilePointer(hFile,2,0,FILE_BEGIN);
	WriteFile(hFile,(char *)&lScene2MainSize,sizeof(lScene2MainSize),&lBytesWritten,NULL);
	return 1;
}

/*
// Used to find the script section of a special object
long CScene2::FindScript(HANDLE hFile, char * szObjectName)
{
	CHelper myHelper;
	long lObjectOffset=FindObject(hFile,szObjectName,SPECIAL_MAIN),
		lObjDatablockSize=0;
	char szDummy[6];
	char sig_script[6]={0x01,0x01,0x00,0x00,0x00,0x01};
	char sig_script_alt[6]={0x01,0x00,0x00,0x00,0x00,0x01};
	char sig_char[6]={0x01,0x01,0x00,0x00,0x00,0x00};
	unsigned long lBytesRead=0;
	CDatablock thisDatablock;
	int i=6;
	
/*	DuplicateHandle(GetCurrentProcess(), hFile,
        GetCurrentProcess(), &hDummy , 0,
        FALSE,
        DUPLICATE_SAME_ACCESS);


	// ok now we know where to find the object so let's start with looking for the
	// 01 01 00 00 00 signature
	SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
	lObjDatablockSize=thisDatablock.GetLong(hFile);
	while(i!=lObjDatablockSize)
	{
		ReadFile(hFile,&szDummy,1,&lBytesRead,NULL);
		i++;
		if(szDummy[0]==1)
		{
			SetFilePointer(hFile,-1,0,FILE_CURRENT);

			ReadFile(hFile,&szDummy,6,&lBytesRead,NULL);
			// compare with first script signature
			if(!myHelper.Compare(szDummy,sig_script,6))
				return(lObjectOffset+i-1);
			// compare with second script signature
			if(!myHelper.Compare(szDummy,sig_script_alt,6))
				return(lObjectOffset+i-1);
			// compare with char script signature
			if(!myHelper.Compare(szDummy,sig_char,6))
				return(lObjectOffset+i-1);
			SetFilePointer(hFile,-5,0,FILE_CURRENT);
		}
	}
	return(-1);
}
*/

// Extracts the script of an object in SPECIAL_MAIN datablock
long CScene2::ExtractObjectScript(HANDLE hFile, char * szObjectName, short nDatablockType, char * szFileName)
{
	ObjectInfo_t Scene2ObjectInfo_t;
	CDatablock thisDatablock;
	CHelper myHelper;
	long lObjectOffset=0,		// offset of our object
		lScriptSize=0,			// the size of the script to extract
		lDummyOffset=0;			// for BufferDataFM
//	unsigned long lBytesRead=0;

	if(nDatablockType==SPECIAL_MAIN)
	{
		lObjectOffset=FindObject(hFile,szObjectName,SPECIAL_MAIN);
		ReadObject(hFile,lObjectOffset,&Scene2ObjectInfo_t,true);
		if(Scene2ObjectInfo_t.lType==SPECIAL_OBJECT_TYPE_SCRIPT)
		{
			// Object is a script object so we have:
			// 01 01 00 00 00 01 00 00 00 00 <size> <script>
			lDummyOffset=lObjectOffset=FindScript(hFile,szObjectName);
			if(lObjectOffset==-1)
				// no script was found
				return(-1);
			SetFilePointer(hFile,(lObjectOffset+10),0,FILE_BEGIN);	// to get to size of script
			lDummyOffset+=14;
			lScriptSize=thisDatablock.GetLong(hFile);
			// first check if it is really a script
			if(lScriptSize==0)
				return(-1);
			// now read out lScriptSize bytes
//			myHelper.BufferData(hFile,szFileName,lScriptSize);
			myHelper.BufferDataFM(hFile,szFileName,lDummyOffset,lScriptSize);
		}
		else
		{
			// Object is a non-script object so we have:
			// 01 01 00 00 00 00 <size> <script>
			lDummyOffset=lObjectOffset=FindScript(hFile,szObjectName);
			if(lObjectOffset==-1)
				//no script found
				return(-1);
			SetFilePointer(hFile,(lObjectOffset+6),0,FILE_BEGIN);
			lDummyOffset+=10;
			lScriptSize=thisDatablock.GetLong(hFile);
//			myHelper.BufferData(hFile,szFileName,lScriptSize);
			myHelper.BufferDataFM(hFile,szFileName,lDummyOffset,lScriptSize);
		}
	}
	else
	if(nDatablockType==ADDITIONAL_MAIN)
	{
		// find additional script object
		lDummyOffset=lObjectOffset=FindObject(hFile,szObjectName,ADDITIONAL_MAIN);
		SetFilePointer(hFile,(lObjectOffset+7),0,FILE_BEGIN);
		lDummyOffset+=7;
		// Get lenght of script name
		lScriptSize=thisDatablock.GetLong(hFile);
		SetFilePointer(hFile,lScriptSize,0,FILE_CURRENT);
		lDummyOffset+=(8+lScriptSize);
		// Get size of script
		lScriptSize=thisDatablock.GetLong(hFile);
		// Buffer data
//		myHelper.BufferData(hFile,szFileName,lScriptSize);
		myHelper.BufferDataFM(hFile,szFileName,lDummyOffset,lScriptSize);
	}
	else
	{
		return(-1);
	}
	return(lScriptSize);
}

// Adds a script to an object situated in SPECIAL_MAIN
long CScene2::AddObjectScript(HANDLE hFile, char *  szObjectName, char * szFileName, short nDatablockType)
{
	ObjectInfo_t Scene2ObjectInfo_t;
	CDatablock thisDatablock;
	CHelper myHelper;

	long lObjectOffset=0,			// offset of object
		lObjDatablockSize=0,		// size of objects datablock
		lScriptOffset=0,			// offset of script of object
		lOldScriptSize=0,			// size of original script
		lNewScriptSize=0,			// size of new script
		lDatablockOffset=0,			// offsets of datablocks to change
		lDatablockSize=0,			// sizes of datablocks to change
		lDummyOffset=0;				// for BufferDataFM

	unsigned long lBytesWritten=0;

	char sSig[6]={0x01,0x01,0x00,0x00,0x00,0x00};

	// check specified datablock
	if((nDatablockType!=SPECIAL_MAIN) && (nDatablockType!=ADDITIONAL_MAIN))
		return(-1);

	if(nDatablockType==ADDITIONAL_MAIN)
	{
		lDummyOffset=lObjectOffset=FindObject(hFile,szObjectName,ADDITIONAL_MAIN);
		if(lObjectOffset==-1)
			return(-1);

		SetFilePointer(hFile,(lObjectOffset+7),0,FILE_BEGIN);
		lDummyOffset+=7;
		lObjDatablockSize=thisDatablock.GetLong(hFile);
		lDummyOffset+=(4+lObjDatablockSize);
		SetFilePointer(hFile,lObjDatablockSize,0,FILE_CURRENT);
		// get old script size
		lOldScriptSize=thisDatablock.GetLong(hFile);
		lDummyOffset+=(4+lOldScriptSize);
		// write new size
		SetFilePointer(hFile,-4,0,FILE_CURRENT);
		lNewScriptSize=myHelper.GetFileSize(szFileName);
		WriteFile(hFile,(char *)&lNewScriptSize,sizeof(lNewScriptSize),&lBytesWritten,NULL);
		// buffer upcoming data
		SetFilePointer(hFile,lOldScriptSize,0,FILE_CURRENT);
//		myHelper.BufferData(hFile,"buffer.tmp",-1);
		myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

		// now write new script
		SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize+15),0,FILE_BEGIN);
		myHelper.PasteBufferedDataFM(hFile,szFileName,false);

		// paste other data
		myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

		SetEndOfFile(hFile);

		// now change sizes
		// 1. objects size
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize+=(lNewScriptSize-lOldScriptSize);
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 2. ADDITIONAL_MAIN size
		lDatablockOffset=thisDatablock.FindDatablock(hFile,ADDITIONAL_MAIN);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize+=(lNewScriptSize-lOldScriptSize);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 3. SCENE2_MAIN
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize+=(lNewScriptSize-lOldScriptSize);
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
	}
	else
	{
		// look for object

		lDummyOffset=lObjectOffset=FindObject(hFile,szObjectName,SPECIAL_MAIN);
		if(lObjectOffset==-1)
			return(-1);

		// get lenght
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		lObjDatablockSize=thisDatablock.GetLong(hFile);

		// now check if the object already got a script section
		lScriptOffset=FindScript(hFile,szObjectName);
		if(lScriptOffset==-1)
		{
			// no
			// buffer data after object (ie new 21AE)
			SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize),0,FILE_BEGIN);
			lDummyOffset+=lObjDatablockSize;
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
			// Now write script signature
			WriteFile(hFile,&sSig,6,&lBytesWritten,NULL);
			// Get Script File Size
			lNewScriptSize=myHelper.GetFileSize(szFileName);
			// Write script size
			WriteFile(hFile,(char *)&lNewScriptSize,sizeof(lNewScriptSize),&lBytesWritten,NULL);
			// Write the script
			myHelper.PasteBufferedDataFM(hFile,szFileName,false);
			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// ---NOW: change datablock sizes
			// 2. 24AE datablock
//			SetFilePointer(hFile,(lObjectOffset+6),0,FILE_BEGIN);
//			lDatablockOffset=thisDatablock.FindSubDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
			lDatablockOffset=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
			lDatablockSize+=(lNewScriptSize+10);
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

			// 3. 21AE
			SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
			lDatablockSize+=(lNewScriptSize+10);
			SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

			// 4. 20AE datablock
			lDatablockOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
			lDatablockSize+=(lNewScriptSize+10);
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

			// 5. SL datablock
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
			lDatablockSize+=(lNewScriptSize+10);
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
		}
		else
		{
			// yes
			// at first we have to determine what object type it is
			ReadObject(hFile,lObjectOffset,&Scene2ObjectInfo_t,true);
			if(Scene2ObjectInfo_t.lType==SPECIAL_OBJECT_TYPE_SCRIPT)
			{
				// Object is a script so we have:
				// 01 00/01 00 00 00 01 00 00 00 00 <size> <script>

				// First get size of previous script
				SetFilePointer(hFile,(lScriptOffset+10),0,FILE_BEGIN);
				lOldScriptSize=thisDatablock.GetLong(hFile);
				// buffer data after object (ie new 21AE)
				SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize),0,FILE_BEGIN);
				lDummyOffset+=lObjDatablockSize;
//				myHelper.BufferData(hFile,"buffer.tmp",-1);
				myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
				// Go back to script
				SetFilePointer(hFile,(lScriptOffset+14),0,FILE_BEGIN);
				// Now write the script + save script size (lNewScriptSize)
				myHelper.PasteBufferedDataFM(hFile,szFileName,false);
				lNewScriptSize=myHelper.GetFileSize(szFileName);
				// Paste other data
				myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

				SetEndOfFile(hFile);

				// ---NOW: change datablock sizes
				// 1. Script Size
				SetFilePointer(hFile,(lScriptOffset+10),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lNewScriptSize,sizeof(lNewScriptSize),&lBytesWritten,NULL);
				// 2. 24AE datablock
//				SetFilePointer(hFile,(lObjectOffset+6),0,FILE_BEGIN);
//				lDatablockOffset=thisDatablock.FindSubDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
				lDatablockOffset=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 3. 21AE
				SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 4. 20AE datablock
				lDatablockOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 5. SL datablock
				SetFilePointer(hFile,2,0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				SetFilePointer(hFile,2,0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
			}
			else
			{
				// Object is NOT a script so we have:
				// 01 01 00 00 00 00 <size> <script>

				// First get size of previous script
				SetFilePointer(hFile,(lScriptOffset+6),0,FILE_BEGIN);
				lOldScriptSize=thisDatablock.GetLong(hFile);
				// buffer data after object (ie new 21AE)
				SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize),0,FILE_BEGIN);
				lDummyOffset+=lObjDatablockSize;
//				myHelper.BufferData(hFile,"buffer.tmp",-1);
				myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
				// Go back to script
				SetFilePointer(hFile,(lScriptOffset+10),0,FILE_BEGIN);
				// Now write the script + save script size (lNewScriptSize)
				myHelper.PasteBufferedDataFM(hFile,szFileName,false);
				lNewScriptSize=myHelper.GetFileSize(szFileName);
				// Paste other data
				myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

				SetEndOfFile(hFile);

				// ---NOW: change datablock sizes
				// 1. Script Size
				SetFilePointer(hFile,(lScriptOffset+6),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lNewScriptSize,sizeof(lNewScriptSize),&lBytesWritten,NULL);
				// 2. 24AE datablock
//				SetFilePointer(hFile,(lObjectOffset+6),0,FILE_BEGIN);
//				lDatablockOffset=thisDatablock.FindSubDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
				lDatablockOffset=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 3. 21AE
				SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 4. 20AE datablock
				lDatablockOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 5. SL datablock
				SetFilePointer(hFile,2,0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				SetFilePointer(hFile,2,0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
			}
		}
	}
	return 0;
}

// Removes a script from an object
long CScene2::RemoveObjectScript(HANDLE hFile, char * szObjectName)
{
	ObjectInfo_t Scene2ObjectInfo_t;
	CDatablock thisDatablock;
	CHelper myHelper;
	char null[4]={0,0,0,0};

	long lObjectOffset=0,			// offset of object
		lObjDatablockSize=0,		// size of objects datablock
		lScriptOffset=0,			// offset of script of object
		lScriptSize=0,				// size of script
		lDatablockOffset=0,			// offsets of datablocks to change
		lDatablockSize=0,			// sizes of datablocks to change
		lDummyOffset=0;

	unsigned long lBytesWritten=0;

	lDummyOffset=lObjectOffset=FindObject(hFile,szObjectName,SPECIAL_MAIN);
	SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
	lObjDatablockSize=thisDatablock.GetLong(hFile);
	ReadObject(hFile,lObjectOffset,&Scene2ObjectInfo_t,true);
	if(Scene2ObjectInfo_t.lType==SPECIAL_OBJECT_TYPE_SCRIPT)
	{
		// Object is a script:
		// 01 00/01 00 00 00 01 00 00 00 00 <size> <script>
		lScriptOffset=FindScript(hFile,szObjectName);
		if(lScriptOffset==-1)
			return(-1);
		// Get size of script
		SetFilePointer(hFile,(lScriptOffset+10),0,FILE_BEGIN);
		lScriptSize=thisDatablock.GetLong(hFile);
		// ZERO it
		SetFilePointer(hFile,(lScriptOffset+10),0,FILE_BEGIN);
		WriteFile(hFile,null,4,&lBytesWritten,NULL);

		// Got to end of object
		SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize),0,FILE_BEGIN);
		lDummyOffset+=lObjDatablockSize;
		// Buffer all data
//		myHelper.BufferData(hFile,"buffer.tmp",-1);
		myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
		// paste data
		SetFilePointer(hFile,(lScriptOffset+14),0,FILE_BEGIN);
		myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

		SetEndOfFile(hFile);

		// NOW: change sizes
		// 1. 24AE
//		lDatablockOffset=thisDatablock.FindSubDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
		lDatablockOffset=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize-=lScriptSize;
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 2. 21AE
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize-=lScriptSize;
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 3. 20AE datablock
		lDatablockOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize-=lScriptSize;
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 4. SL datablock
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize-=lScriptSize;
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

	}
	else
	{
		// Object is NOT a script:
		// 01 01 00 00 00 00 <size> <script>
		lScriptOffset=FindScript(hFile,szObjectName);
		if(lScriptOffset==-1)
			return(-1);
		// Get size of script
		SetFilePointer(hFile,(lScriptOffset+6),0,FILE_BEGIN);
		lScriptSize=thisDatablock.GetLong(hFile);

		// Got to end of object
		SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize),0,FILE_BEGIN);
		lDummyOffset+=lObjDatablockSize;
		// Buffer all data
//		myHelper.BufferData(hFile,"buffer.tmp",-1);
		myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
		// paste data
		SetFilePointer(hFile,lScriptOffset,0,FILE_BEGIN);
		myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

		SetEndOfFile(hFile);

		// NOW: change sizes
		// 1. 24AE
//		lDatablockOffset=thisDatablock.FindSubDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
		lDatablockOffset=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize-=(lScriptSize+10);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 2. 21AE
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize-=(lScriptSize+10);
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 3. 20AE datablock
		lDatablockOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize-=(lScriptSize+10);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 4. SL datablock
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize-=(lScriptSize+10);
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
	}
	return 0;
}

// Edits name of object szObjectName
long CScene2::EditObjectName(HANDLE hFile, char * szOldObjectName, char * szObjectName, short nDatablockType)
{
	CDatablock thisDatablock;
	CHelper myHelper;

	unsigned long lBytesWritten=0;
	long lObjectOffset=0,
		lNameDatablockOffset=0,
		lNameDatablockSize=0,
		lOldNameDatablockSize=0,
		lDatablockOffset=0,
		lDatablockSize=0,
		lDummyOffset=0;


	// Check if a proper datablock has been specified
	if((nDatablockType!=SPECIAL_MAIN) && (nDatablockType!=NORMAL_MAIN) && (nDatablockType!=ADDITIONAL_MAIN))
		return(-1);

	lDummyOffset=lObjectOffset=FindObject(hFile,szOldObjectName,nDatablockType);
	if(lObjectOffset==-1)
		return(-1);

	// additional object?
	if(nDatablockType==ADDITIONAL_MAIN)
	{
		// yes, we have to follow some other steps
		// get size of name
		SetFilePointer(hFile,(lObjectOffset+7),0,FILE_BEGIN);
		lDummyOffset+=11;
		lOldNameDatablockSize=thisDatablock.GetLong(hFile);
		// Write new size
		SetFilePointer(hFile,-4,0,FILE_CURRENT);
		lNameDatablockSize=strlen(szObjectName);
		WriteFile(hFile,(char *)&lNameDatablockSize,sizeof(lNameDatablockSize),&lBytesWritten,NULL);
		// Go to end of name
		SetFilePointer(hFile,lOldNameDatablockSize,0,FILE_CURRENT);
		lDummyOffset+=lOldNameDatablockSize;
		// Buffer the data
//		myHelper.BufferData(hFile,"buffer.tmp",-1);
		myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
		// go back to name and write it
		SetFilePointer(hFile,(lObjectOffset+11),0,FILE_BEGIN);
		WriteFile(hFile,szObjectName,strlen(szObjectName),&lBytesWritten,NULL);
		// paste other data
		myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

		SetEndOfFile(hFile);

		// now change sizes
		// 1. object size
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize+=(lNameDatablockSize-lOldNameDatablockSize);
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 2. ADDITIONAL_MAIN size
		lDatablockOffset=thisDatablock.FindDatablock(hFile,ADDITIONAL_MAIN);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize+=(lNameDatablockSize-lOldNameDatablockSize);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
	}
	else
	{
		if(nDatablockType==SPECIAL_MAIN)
//			lDummyOffset=lNameDatablockOffset=thisDatablock.FindSubDatablock(hFile,SPECIAL_OBJECT_NAME,(lObjectOffset+6));
			lDummyOffset=lNameDatablockOffset=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_NAME,(lObjectOffset+6));
		else
//			lDummyOffset=lNameDatablockOffset=thisDatablock.FindSubDatablock(hFile,NORMAL_OBJECT_OR_SECTOR_NAME,(lObjectOffset+6));
			lDummyOffset=lNameDatablockOffset=thisDatablock.FindObjectDatablock(hFile,NORMAL_OBJECT_OR_SECTOR_NAME,(lObjectOffset+6));

		SetFilePointer(hFile,(lNameDatablockOffset+2),0,FILE_BEGIN);

		lOldNameDatablockSize=thisDatablock.GetLong(hFile);

		// go to end of name datablock and buffer everything
		SetFilePointer(hFile,(lOldNameDatablockSize-6),0,FILE_CURRENT);
		lDummyOffset+=lOldNameDatablockSize;
//		myHelper.BufferData(hFile,"buffer3.tmp",-1);
		myHelper.BufferDataFM(hFile,"buffer3.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

		// go back to name datablock,edit size and replace my name
		SetFilePointer(hFile,(lNameDatablockOffset+2),0,FILE_BEGIN);
		lNameDatablockSize=strlen(szObjectName)+7;
		WriteFile(hFile,(char *)&lNameDatablockSize,sizeof(lNameDatablockSize),&lBytesWritten,NULL);
		WriteFile(hFile,szObjectName,strlen(szObjectName)+1,&lBytesWritten,NULL);

		// paste other data
		myHelper.PasteBufferedDataFM(hFile,"buffer3.tmp",true);

		SetEndOfFile(hFile);

		// change sizes
		// 1. XXXXX_OBJECT_MAIN

		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
		lDatablockSize+=((strlen(szObjectName)+1)-(lOldNameDatablockSize-6));
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 2. XXXXX_MAIN
		if(nDatablockType==SPECIAL_MAIN)
		{
			lDatablockOffset=thisDatablock.FindDatablock(hFile,nDatablockType);
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
			lDatablockSize+=((strlen(szObjectName)+1)-(lOldNameDatablockSize-6));
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
		}
		else
		{
			lDatablockOffset=thisDatablock.FindDatablock(hFile,nDatablockType);
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
			lDatablockSize+=((strlen(szObjectName)+1)-(lOldNameDatablockSize-6));
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
		}

	}
	// 3. SCENE2_MAIN

	SetFilePointer(hFile,2,0,FILE_BEGIN);
	lDatablockSize=thisDatablock.GetLong(hFile);
	if(nDatablockType==ADDITIONAL_MAIN)
		lDatablockSize+=(lNameDatablockSize-lOldNameDatablockSize);
	else
		lDatablockSize+=((strlen(szObjectName)+1)-(lOldNameDatablockSize-6));
	SetFilePointer(hFile,2,0,FILE_BEGIN);
	WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
	return 0;
}

// Initalizes ObjectInfo_t structure with default values, recommend after every declaration of this struct
void CScene2::PrepareObjectInfo(ObjectInfo_t * Scene2ObjectInfo_t)
{
	int i;
	
	// clear sName
	for(i=0;i<256;i++)
		Scene2ObjectInfo_t->sName[i]=0;

	// clear sModelName
	for(i=0;i<256;i++)
		Scene2ObjectInfo_t->sModelName[i]=0;

	// clear sSectorName
	for(i=0;i<256;i++)
		Scene2ObjectInfo_t->sSectorName[i]=0;

	// set bUseModel to false
	Scene2ObjectInfo_t->bUseModel=false;

	// clear pos
	for(i=0;i<3;i++)
		Scene2ObjectInfo_t->fPos[i]=0;

	// clear rot
	for(i=0;i<4;i++)
		Scene2ObjectInfo_t->fRot[i]=0;

	// clear size
	for(i=0;i<3;i++)
		Scene2ObjectInfo_t->fSize[i]=0;

	// clear additional translation
	for(i=0;i<3;i++)
		Scene2ObjectInfo_t->fAddTrans[i]=0;

	// ---- Character Specific

	// clear behaviour
	Scene2ObjectInfo_t->cBehaviour=0;

	// clear voice
	Scene2ObjectInfo_t->cVoice=0;

	// clear strenght
	Scene2ObjectInfo_t->fStrenght=0;

	// clear energy
	Scene2ObjectInfo_t->fEnergy=0;

	// clear left hand energy
	Scene2ObjectInfo_t->fEnergy_HANDL=0;

	// clear right hand energy
	Scene2ObjectInfo_t->fEnergy_HANDR=0;

	// clear left leg energy
	Scene2ObjectInfo_t->fEnergy_LEGL=0;

	// clear right leg energy
	Scene2ObjectInfo_t->fEnergy_LEGR=0;

	// clear reactions
	Scene2ObjectInfo_t->fReactions=0;

	// clear speed
	Scene2ObjectInfo_t->fSpeed=0;

	// clear aggressivity
	Scene2ObjectInfo_t->fAggr=0;

	// clear intelligence
	Scene2ObjectInfo_t->fInt=0;

	// clear shooting
	Scene2ObjectInfo_t->fShooting=0;

	// clear sight
	Scene2ObjectInfo_t->fSight=0;

	// clear hearing
	Scene2ObjectInfo_t->fHearing=0;

	// clear mass
	Scene2ObjectInfo_t->fMass=0;

	// clear driving
	Scene2ObjectInfo_t->fDriving=0;

	// clear behaviour 2
	Scene2ObjectInfo_t->fBehaviour=0;

	// ---- Light Specific

	// clear lighttype
	Scene2ObjectInfo_t->lLightType=0;

	// clear red
	Scene2ObjectInfo_t->fRed=0;

	// clear green
	Scene2ObjectInfo_t->fGreen=0;

	// clear blue
	Scene2ObjectInfo_t->fBlue=0;

	// clear flags
	Scene2ObjectInfo_t->lLightFlags=0;

	// clear power
	Scene2ObjectInfo_t->fPower=0;

	// clear unk1
	Scene2ObjectInfo_t->fLightUnk1=0;

	// clear unk2
	Scene2ObjectInfo_t->fLightUnk2=0;

	// clear near
	Scene2ObjectInfo_t->fLightNear=0;

	// clear far
	Scene2ObjectInfo_t->fLightFar=0;

	// clear light sector
	for(i=0;i<5000;i++)
		Scene2ObjectInfo_t->sLightSectors[i]=0;

	// ------ Type 06 specific
	for(i=0;i<6;i++)
		Scene2ObjectInfo_t->fVal[i]=0;

	// ------ Movable Object specific
	Scene2ObjectInfo_t->fMovVal1=0;
	Scene2ObjectInfo_t->fMovVal2=0;
	Scene2ObjectInfo_t->fMovVal3=0;
	Scene2ObjectInfo_t->fMovVal4=0;
	Scene2ObjectInfo_t->lMovVal5=0;
	Scene2ObjectInfo_t->fWeight=0;
	Scene2ObjectInfo_t->lSound=0;

	// Sector Specific
	Scene2ObjectInfo_t->fSectorVal1=0;
	Scene2ObjectInfo_t->fSectorVal2=0;
	Scene2ObjectInfo_t->fSectorVal3=0;
	Scene2ObjectInfo_t->fSectorVal4=0;
	Scene2ObjectInfo_t->lSectorVal5=0;
	Scene2ObjectInfo_t->fSectorVal6=0;
	Scene2ObjectInfo_t->lSectorVal7=0;
	Scene2ObjectInfo_t->fSectorVal8=0;
	Scene2ObjectInfo_t->lSectorVal9=0;
	Scene2ObjectInfo_t->fSectorVal10=0;
	Scene2ObjectInfo_t->fSectorVal11=0;
	Scene2ObjectInfo_t->lSectorVal12=0;
	Scene2ObjectInfo_t->lSectorVal13=0;
	Scene2ObjectInfo_t->lSectorVal14=0;
	Scene2ObjectInfo_t->lSectorVal15=0;
	Scene2ObjectInfo_t->fSectorVal16=0;
	Scene2ObjectInfo_t->lSectorVal17=0;
	Scene2ObjectInfo_t->fSectorVal18=0;
	Scene2ObjectInfo_t->fSectorVal19=0;
	Scene2ObjectInfo_t->lSectorVal20=0;

	for(i=0;i<3;i++)
		Scene2ObjectInfo_t->lSectorVal21[i]=0;

	for(i=0;i<3;i++)
		Scene2ObjectInfo_t->lSectorVal22[i]=0;

	// Traffic Object (only global vars)
	for(i=0;i<3;i++)
		Scene2ObjectInfo_t->fTrafficVal[i]=0;
	Scene2ObjectInfo_t->lDensity=0;
	Scene2ObjectInfo_t->lNumOfCars=0;

	// Ped Object (only global vars)
	for(i=0;i<5;i++)
		Scene2ObjectInfo_t->fPedVal[i]=0;
	Scene2ObjectInfo_t->lGlobalPedDensity=0;
	Scene2ObjectInfo_t->lNumOfPeds=0;

	// Camera Related
	Scene2ObjectInfo_t->bExtended=false;
	Scene2ObjectInfo_t->bSector=false;

	// Race Cam Related
	Scene2ObjectInfo_t->lRaceCamVal1=0;
	Scene2ObjectInfo_t->fRaceCamVal2=0;
	Scene2ObjectInfo_t->fRaceCamVal3=0;
	Scene2ObjectInfo_t->fRaceCamVal4=0;
	Scene2ObjectInfo_t->lRaceCamVal5=0;
	Scene2ObjectInfo_t->lRaceCamVal6=0;

	// Door Related
	Scene2ObjectInfo_t->bOpen1=false;
	Scene2ObjectInfo_t->bOpen2=false;
	Scene2ObjectInfo_t->fMoveAngle=0;
	Scene2ObjectInfo_t->bOpen=false;
	Scene2ObjectInfo_t->bLocked=false;
	Scene2ObjectInfo_t->fCloseSpeed=0;
	Scene2ObjectInfo_t->fOpenSpeed=0;

	for(i=0;i<16;i++)
	{
		Scene2ObjectInfo_t->sOpenSound[i]=0;
		Scene2ObjectInfo_t->sCloseSound[i]=0;
		Scene2ObjectInfo_t->sLockedSound[i]=0;
	}

	Scene2ObjectInfo_t->bDoorFlag=false;
}

// Retrieves object names from scene2.bin
int CScene2::GetNextObject(HANDLE hFile, BasicObjectInfo_t * pBasicObjectInfo_t, Filter_t * pObjectFilter_t)
{
	CDatablock thisDatablock;
//	CString czObjectName;
	ObjectInfo_t Scene2ObjectInfo_t;
	short nType=DATABLOCK_UNKNOWN;
	long lMainSize=0, lSubSize=0, /*lObjectOffset=6,*/ lDummyOffset=0/*, lTempOffset=0*/;
	unsigned long lBytesRead=0;
	char szObjectName[256];			// 256 chars max
	int i;
	char *dummy=0;					// for strstr

	PrepareObjectInfo(&Scene2ObjectInfo_t);


	// check if no filter was set up

	if(pObjectFilter_t==NULL)
	{
		//no
		// First set file pointer to 6 bytes after file begin
//		SetFilePointer(hFile,6,0,FILE_BEGIN);

//		while(1)
//		{
			// flush name buffer
			for(i=0;i<256;i++)
				szObjectName[i]=0;

			nType=thisDatablock.GetShort(hFile);
			if((nType==SCENE2_EOF) || (nType==SCENE2_NULL))
				return(-1);
			lMainSize=thisDatablock.GetLong(hFile);

			switch(nType)
			{
				case NORMAL_OBJECT_MAIN:
					// Get Name
					ReadObject(hFile,pBasicObjectInfo_t->lNextObjectOffset,&Scene2ObjectInfo_t,true);

					// Store name
					strcpy(pBasicObjectInfo_t->name,Scene2ObjectInfo_t.sName);
					pBasicObjectInfo_t->nDatablock=NORMAL_MAIN;
					pBasicObjectInfo_t->iObjectType=Scene2ObjectInfo_t.lType;

					// Put file pointer to next object
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
					return(1);

				case SPECIAL_OBJECT_MAIN:
					// Get Name
					ReadObject(hFile,pBasicObjectInfo_t->lNextObjectOffset,&Scene2ObjectInfo_t,true);

					// Store name
					strcpy(pBasicObjectInfo_t->name,Scene2ObjectInfo_t.sName);
					pBasicObjectInfo_t->nDatablock=SPECIAL_MAIN;
					pBasicObjectInfo_t->iObjectType=Scene2ObjectInfo_t.lType;

					// Put file pointer to next object
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
					return(1);

				case ADDITIONAL_OBJECT:
					SetFilePointer(hFile,1,0,FILE_CURRENT);
					lSubSize=thisDatablock.GetLong(hFile);
					ReadFile(hFile,szObjectName,lSubSize,&lBytesRead,NULL);

					// Store name
					strcpy(pBasicObjectInfo_t->name,szObjectName);
					pBasicObjectInfo_t->nDatablock=ADDITIONAL_MAIN;
					pBasicObjectInfo_t->iObjectType=ADDITIONAL_OBJECT_TYPE_SCRIPT;

					// Put to next object
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
					return(1);

				case UNKNOWN_DATABLOCK_NAME:
					SetFilePointer(hFile,10,0,FILE_CURRENT);
					lSubSize=thisDatablock.GetLong(hFile);
					ReadFile(hFile,szObjectName,lSubSize,&lBytesRead,NULL);

					// Store name
					strcpy(pBasicObjectInfo_t->name,szObjectName);
					pBasicObjectInfo_t->nDatablock=UNKNOWN_DATABLOCK_MAIN;
					pBasicObjectInfo_t->iObjectType=UNKNOWN_OBJECT_TYPE_UNKNOWN;
					pBasicObjectInfo_t->lNextObjectOffset=+lMainSize;

					// Put to next object
					pBasicObjectInfo_t->lNextObjectOffset=+lMainSize;
					SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
					return(1);

				case NORMAL_MAIN:
					lNormalMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);
				case SPECIAL_MAIN:
					lSpecialMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case ADDITIONAL_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case UNKNOWN_DATABLOCK_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				default:
					SetFilePointer(hFile,(lMainSize-6),0,FILE_CURRENT);
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					return(0);
				}
	}

	if((pObjectFilter_t->iFilterType&FILTER_OBJECTNAME)==8)
	{
		// name filter enabled
		// First set file pointer to 6 bytes after file begin
//		SetFilePointer(hFile,6,0,FILE_BEGIN);

//		while(1)
//		{
			// flush name buffer
			for(i=0;i<256;i++)
				szObjectName[i]=0;

			// clear lType
			Scene2ObjectInfo_t.lType=-1;

			nType=thisDatablock.GetShort(hFile);
			if((nType==SCENE2_EOF) || (nType==SCENE2_NULL))
				return(-1);

			lMainSize=thisDatablock.GetLong(hFile);

			switch(nType)
			{
				case NORMAL_OBJECT_MAIN:
					ReadObject(hFile,pBasicObjectInfo_t->lNextObjectOffset,&Scene2ObjectInfo_t,true);
					dummy=strstr(Scene2ObjectInfo_t.sName,pObjectFilter_t->szObjectName);
					if(dummy!=NULL)
					{
						strcpy(pBasicObjectInfo_t->name,Scene2ObjectInfo_t.sName);
						pBasicObjectInfo_t->nDatablock=NORMAL_MAIN;
						pBasicObjectInfo_t->iObjectType=Scene2ObjectInfo_t.lType;

						// Put file pointer to next object
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(1);
					}
					else
					{
						// Put file pointer to next object
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(0);
					}
					break;	// bull shit but who cares

				case SPECIAL_OBJECT_MAIN:
					ReadObject(hFile,pBasicObjectInfo_t->lNextObjectOffset,&Scene2ObjectInfo_t,true);
					dummy=strstr(Scene2ObjectInfo_t.sName,pObjectFilter_t->szObjectName);
					if(dummy!=NULL)
					{
						strcpy(pBasicObjectInfo_t->name,Scene2ObjectInfo_t.sName);
						pBasicObjectInfo_t->nDatablock=SPECIAL_MAIN;
						pBasicObjectInfo_t->iObjectType=Scene2ObjectInfo_t.lType;

						// Put file pointer to next object
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(1);
					}
					else
					{
						// Put file pointer to next object
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(0);
					}
					break;

				case NORMAL_MAIN:
					lNormalMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case SPECIAL_MAIN:
					lSpecialMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case ADDITIONAL_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case UNKNOWN_DATABLOCK_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				default:
					SetFilePointer(hFile,(lMainSize-6),0,FILE_CURRENT);
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					return(0);
				}
//		}
	}

	if((pObjectFilter_t->iFilterType&FILTER_SECTOR)==4)
	{
		// sector filter enabled
		// First set file pointer to 6 bytes after file begin
//		SetFilePointer(hFile,6,0,FILE_BEGIN);

//		while(1)
//		{
			// flush name buffer
			for(i=0;i<256;i++)
				szObjectName[i]=0;

			// clear lType
			Scene2ObjectInfo_t.lType=-1;

			nType=thisDatablock.GetShort(hFile);
			if((nType==SCENE2_EOF) || (nType==SCENE2_NULL))
				return(-1);

			lMainSize=thisDatablock.GetLong(hFile);

			switch(nType)
			{
				case NORMAL_OBJECT_MAIN:
					lDummyOffset=thisDatablock.FindObjectDatablock(hFile,NORMAL_SECTOR_MAIN,(pBasicObjectInfo_t->lNextObjectOffset+6));
					if(lDummyOffset!=SCENE2_EOF)
					{
						ReadObject(hFile,pBasicObjectInfo_t->lNextObjectOffset,&Scene2ObjectInfo_t,true);
						strcpy(pBasicObjectInfo_t->name,Scene2ObjectInfo_t.sName);
						pBasicObjectInfo_t->nDatablock=NORMAL_MAIN;
						pBasicObjectInfo_t->iObjectType=Scene2ObjectInfo_t.lType;

						// Put file pointer to next object
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(1);
					}
					else
					{
						// Put file pointer to next object
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(0);
					}
					break;

				case NORMAL_MAIN:
					lNormalMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case SPECIAL_MAIN:
					lSpecialMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case ADDITIONAL_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case UNKNOWN_DATABLOCK_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				default:
					SetFilePointer(hFile,(lMainSize-6),0,FILE_CURRENT);
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					return(0);
				}
//		}
	}

	if((pObjectFilter_t->iFilterType&FILTER_LIGHTMAP)==2)
	{
		// lightmap filter enabled
		// First set file pointer to 6 bytes after file begin
//		SetFilePointer(hFile,6,0,FILE_BEGIN);

//		while(1)
//		{
			// flush name buffer
			for(i=0;i<256;i++)
				szObjectName[i]=0;

			// clear lType
			Scene2ObjectInfo_t.lType=-1;

			nType=thisDatablock.GetShort(hFile);
			if((nType==SCENE2_EOF) || (nType==SCENE2_NULL))
				return(-1);

			lMainSize=thisDatablock.GetLong(hFile);

			switch(nType)
			{
				case NORMAL_OBJECT_MAIN:
					lDummyOffset=thisDatablock.FindObjectDatablock(hFile,NORMAL_LENSFLARE_OR_LIGHTMAP_OR_GLOW,(pBasicObjectInfo_t->lNextObjectOffset+6));
					if(lDummyOffset!=SCENE2_EOF)
					{
						ReadObject(hFile,pBasicObjectInfo_t->lNextObjectOffset,&Scene2ObjectInfo_t,true);
						strcpy(pBasicObjectInfo_t->name,Scene2ObjectInfo_t.sName);
						pBasicObjectInfo_t->nDatablock=NORMAL_MAIN;
						pBasicObjectInfo_t->iObjectType=Scene2ObjectInfo_t.lType;

						// Put file pointer to next object
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(1);
					}
					else
					{
						// Put file pointer to next object
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(0);
					}
					break;

				case NORMAL_MAIN:
					lNormalMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case SPECIAL_MAIN:
					lSpecialMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case ADDITIONAL_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case UNKNOWN_DATABLOCK_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				default:
					SetFilePointer(hFile,(lMainSize-6),0,FILE_CURRENT);
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					return(0);
				}
//		}
	}

	if((pObjectFilter_t->iFilterType&FILTER_OBJECTTYPE)==1)
	{
		// yes
		// First set file pointer to 6 bytes after file begin
//		SetFilePointer(hFile,6,0,FILE_BEGIN);

//		while(1)
//		{
			// flush name buffer
			for(i=0;i<256;i++)
				szObjectName[i]=0;

			// clear lType
			Scene2ObjectInfo_t.lType=-1;

			nType=thisDatablock.GetShort(hFile);
			if((nType==SCENE2_EOF) || (nType==SCENE2_NULL))
				return(-1);

			lMainSize=thisDatablock.GetLong(hFile);

			switch(nType)
			{
				case NORMAL_OBJECT_MAIN:
					// first check if the object is to be searched in NORMAL_MAIN
					if((pObjectFilter_t->nDatablock==SPECIAL_MAIN) || (pObjectFilter_t->nDatablock==ADDITIONAL_MAIN))
					{
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(0);
					}

					// now compare the object type to the requested one (filter)
					ReadObject(hFile,pBasicObjectInfo_t->lNextObjectOffset,&Scene2ObjectInfo_t,true);
					if(pObjectFilter_t->iObjectType==-1)
						Scene2ObjectInfo_t.lType=-1;
//					SetFilePointer(hFile,(pBasicObjectInfo_t->lNextObjectOffset+6),0,FILE_BEGIN);
					if((Scene2ObjectInfo_t.lType!=pObjectFilter_t->iObjectType)&&(Scene2ObjectInfo_t.lType!=-1))
					{
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(0);
					}

					strcpy(pBasicObjectInfo_t->name,Scene2ObjectInfo_t.sName);
					pBasicObjectInfo_t->nDatablock=NORMAL_MAIN;
					pBasicObjectInfo_t->iObjectType=Scene2ObjectInfo_t.lType;

					// put filepointer
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
					return(1);

				case SPECIAL_OBJECT_MAIN:
					// first check if the object is to be searched in SPECIAL_MAIN
					if((pObjectFilter_t->nDatablock==NORMAL_MAIN) || (pObjectFilter_t->nDatablock==ADDITIONAL_MAIN))
					{
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(0);
					}

					// now compare the object type to the requested one (filter)
					ReadObject(hFile,pBasicObjectInfo_t->lNextObjectOffset,&Scene2ObjectInfo_t,true);
					if(pObjectFilter_t->iObjectType==-1)
						Scene2ObjectInfo_t.lType=-1;
//					SetFilePointer(hFile,(pBasicObjectInfo_t->lNextObjectOffset+6),0,FILE_BEGIN);
					if((Scene2ObjectInfo_t.lType!=pObjectFilter_t->iObjectType)&&(Scene2ObjectInfo_t.lType!=-1))
					{
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(0);
					}

					strcpy(pBasicObjectInfo_t->name,Scene2ObjectInfo_t.sName);
					pBasicObjectInfo_t->nDatablock=SPECIAL_MAIN;
					pBasicObjectInfo_t->iObjectType=Scene2ObjectInfo_t.lType;

					// put filepointer
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
					return(1);

				case ADDITIONAL_OBJECT:
					// first check if the object is to be searched in ADDITIONAL_MAIN
					if((pObjectFilter_t->nDatablock==NORMAL_MAIN) || (pObjectFilter_t->nDatablock==SPECIAL_MAIN))
					{
						pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
						SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
						return(0);
					}

					SetFilePointer(hFile,1,0,FILE_CURRENT);
					lSubSize=thisDatablock.GetLong(hFile);
					ReadFile(hFile,szObjectName,lSubSize,&lBytesRead,NULL);
					
					// Store name
					strcpy(pBasicObjectInfo_t->name,szObjectName);
					pBasicObjectInfo_t->nDatablock=ADDITIONAL_MAIN;
					pBasicObjectInfo_t->iObjectType=ADDITIONAL_OBJECT_TYPE_SCRIPT;

					// Put filepointer
					pBasicObjectInfo_t->lNextObjectOffset=+lMainSize;
					SetFilePointer(hFile,pBasicObjectInfo_t->lNextObjectOffset,0,FILE_BEGIN);
					return(1);

				case NORMAL_MAIN:
					lNormalMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case SPECIAL_MAIN:
					lSpecialMainOffset=pBasicObjectInfo_t->lNextObjectOffset;
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case ADDITIONAL_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				case UNKNOWN_DATABLOCK_MAIN:
					pBasicObjectInfo_t->lNextObjectOffset+=6;
					return(0);

				default:
					SetFilePointer(hFile,(lMainSize-6),0,FILE_CURRENT);
					pBasicObjectInfo_t->lNextObjectOffset+=lMainSize;
					return(0);
				}
		}
//	*pResult = 0;

	return(-1);
}

// Reads out the copyright text in scene2.bin
bool CScene2::GetCopyrightText(HANDLE hFile, char * Buffer)
{
	CDatablock thisDatablock;
	short nDatablockType=0;
	long lSize;
	unsigned long lBytesRead=0;

	SetFilePointer(hFile,6,0,FILE_BEGIN);
	nDatablockType=thisDatablock.GetShort(hFile);
	if(nDatablockType!=DATABLOCK_COMMENT)
		return false;

	lSize=thisDatablock.GetLong(hFile);
	lSize-=10;
	SetFilePointer(hFile,4,0,FILE_CURRENT);
	ReadFile(hFile,Buffer,lSize,&lBytesRead,NULL);

	return(true);
}

// Sets the copyright
bool CScene2::SetCopyrightText(HANDLE hFile, char * Copyright)
{
	CDatablock thisDatablock;
	CHelper myHelper;

	short nDatablockType=0;
	long lOffset=6,lSize=0, lOldSize;
	unsigned long lBytesWritten=0;
	char null[2]={0,0};

	SetFilePointer(hFile,6,0,FILE_BEGIN);
	nDatablockType=thisDatablock.GetShort(hFile);
	if(nDatablockType!=DATABLOCK_COMMENT)
		return false;

	// buffer data
	lOldSize=thisDatablock.GetLong(hFile);
	lOffset+=lOldSize;
	myHelper.BufferDataFM(hFile,"buffer.tmp",lOffset,::GetFileSize(hFile,NULL)-lOffset);

	// return
	SetFilePointer(hFile,2,0,FILE_BEGIN);
	lSize=thisDatablock.GetLong(hFile);
	lSize+=((strlen(Copyright)+12)-lOldSize);
	SetFilePointer(hFile,2,0,FILE_BEGIN);
	WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);

	SetFilePointer(hFile,2,0,FILE_CURRENT);
	lSize=strlen(Copyright)+12;
	WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);

	SetFilePointer(hFile,4,0,FILE_CURRENT);

	// write copyright text
	WriteFile(hFile,Copyright,strlen(Copyright),&lBytesWritten,NULL);

	// put 00
	WriteFile(hFile,null,2,&lBytesWritten,NULL);

	// append data
	myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

	return true;
}

// Reads the script of a given object and puts it into a buffer
long CScene2::ReadObjectScript(HANDLE hFile, char * Buffer, char * szObjectName, short nDatablockType)
{
	ObjectInfo_t Scene2ObjectInfo_t;
	CDatablock thisDatablock;
	CHelper myHelper;
	long lObjectOffset=0,		// offset of our object
		lScriptSize=0,			// the size of the script to extract
		lDummyOffset=0;			// for BufferDataFM
	unsigned long lBytesRead=0;

	if(nDatablockType==SPECIAL_MAIN)
	{
		lObjectOffset=FindObject(hFile,szObjectName,SPECIAL_MAIN);
		ReadObject(hFile,lObjectOffset,&Scene2ObjectInfo_t,true);
		if(Scene2ObjectInfo_t.lType==SPECIAL_OBJECT_TYPE_SCRIPT)
		{
			// Object is a script object so we have:
			// 01 01 00 00 00 01 00 00 00 00 <size> <script>
			lDummyOffset=lObjectOffset=FindScript(hFile,szObjectName);
			if(lObjectOffset==-1)
				// no script was found
				return(-1);
			SetFilePointer(hFile,(lObjectOffset+10),0,FILE_BEGIN);	// to get to size of script
			// now read out lScriptSize bytes
			lScriptSize=thisDatablock.GetLong(hFile);
			lDummyOffset+=14;
			SetFilePointer(hFile,(lObjectOffset+14),0,FILE_BEGIN);
			// first check if it is really a script
			if(lScriptSize==0)
				return(-1);

//			myHelper.BufferData(hFile,szFileName,lScriptSize);
//			myHelper.BufferDataFM(hFile,szFileName,lDummyOffset,lScriptSize);
			ReadFile(hFile,Buffer,lScriptSize,&lBytesRead,NULL);
		}
		else
		{
			// Object is a non-script object so we have:
			// 01 01 00 00 00 00 <size> <script>
			lDummyOffset=lObjectOffset=FindScript(hFile,szObjectName);
			if(lObjectOffset==-1)
				//no script found
				return(-1);
			SetFilePointer(hFile,(lObjectOffset+6),0,FILE_BEGIN);
			lScriptSize=thisDatablock.GetLong(hFile);
			lDummyOffset+=10;
			SetFilePointer(hFile,(lObjectOffset+10),0,FILE_BEGIN);
//			myHelper.BufferData(hFile,szFileName,lScriptSize);
//			myHelper.BufferDataFM(hFile,szFileName,lDummyOffset,lScriptSize);
			ReadFile(hFile,Buffer,lScriptSize,&lBytesRead,NULL);
		}
	}
	else
	if(nDatablockType==ADDITIONAL_MAIN)
	{
		// find additional script object
		lDummyOffset=lObjectOffset=FindObject(hFile,szObjectName,ADDITIONAL_MAIN);
		SetFilePointer(hFile,(lObjectOffset+7),0,FILE_BEGIN);
		lDummyOffset+=7;
		// Get lenght of script name
		lScriptSize=thisDatablock.GetLong(hFile);
		SetFilePointer(hFile,lScriptSize,0,FILE_CURRENT);
		lDummyOffset+=(8+lScriptSize);
		// Get size of script
		lScriptSize=thisDatablock.GetLong(hFile);
		// Buffer data
//		myHelper.BufferData(hFile,szFileName,lScriptSize);
//		myHelper.BufferDataFM(hFile,szFileName,lDummyOffset,lScriptSize);
		ReadFile(hFile,Buffer,lScriptSize,&lBytesRead,NULL);
	}
	else
	{
		return(-1);
	}
	return(lScriptSize);
}

// Adds a script to an object - reads script from buffer
int CScene2::AddObjectScriptBuffer(HANDLE hFile, char * szObjectName, char * Buffer, int ScriptSize, short nDatablockType)
{
	ObjectInfo_t Scene2ObjectInfo_t;
	CDatablock thisDatablock;
	CHelper myHelper;

	long lObjectOffset=0,			// offset of object
		lObjDatablockSize=0,		// size of objects datablock
		lScriptOffset=0,			// offset of script of object
		lOldScriptSize=0,			// size of original script
//		lNewScriptSize=0,			// size of new script - commented in order to detect/prevent usage
		lDatablockOffset=0,			// offsets of datablocks to change
		lDatablockSize=0,			// sizes of datablocks to change
		lDummyOffset=0;				// for BufferDataFM

	unsigned long lBytesWritten=0;

	char sSig[6]={0x01,0x01,0x00,0x00,0x00,0x00};

	// check specified datablock
	if((nDatablockType!=SPECIAL_MAIN) && (nDatablockType!=ADDITIONAL_MAIN))
		return(-1);

	if(nDatablockType==ADDITIONAL_MAIN)
	{
		lDummyOffset=lObjectOffset=FindObject(hFile,szObjectName,ADDITIONAL_MAIN);
		if(lObjectOffset==-1)
			return(-1);

		SetFilePointer(hFile,(lObjectOffset+7),0,FILE_BEGIN);
		lDummyOffset+=7;
		lObjDatablockSize=thisDatablock.GetLong(hFile);
		lDummyOffset+=(4+lObjDatablockSize);
		SetFilePointer(hFile,lObjDatablockSize,0,FILE_CURRENT);
		// get old script size
		lOldScriptSize=thisDatablock.GetLong(hFile);
		lDummyOffset+=(4+lOldScriptSize);
		// write new size
		SetFilePointer(hFile,-4,0,FILE_CURRENT);
//		lNewScriptSize=myHelper.GetFileSize(szFileName);
//		WriteFile(hFile,(char *)&lOldScriptSize,sizeof(lOldScriptSize),&lBytesWritten,NULL);
		WriteFile(hFile,(char *)&ScriptSize,sizeof(ScriptSize),&lBytesWritten,NULL);
		// buffer upcoming data
		SetFilePointer(hFile,lOldScriptSize,0,FILE_CURRENT);
//		myHelper.BufferData(hFile,"buffer.tmp",-1);
		myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

		// now write new script
		SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize+15),0,FILE_BEGIN);
//		myHelper.PasteBufferedDataFM(hFile,szFileName,false);
		WriteFile(hFile,Buffer,ScriptSize,&lBytesWritten,NULL);

		// paste other data
		myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

		SetEndOfFile(hFile);

		// now change sizes
		// 1. objects size
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
//		lDatablockSize+=(lNewScriptSize-lOldScriptSize);
		lDatablockSize+=(ScriptSize-lOldScriptSize);
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 2. ADDITIONAL_MAIN size
		lDatablockOffset=thisDatablock.FindDatablock(hFile,ADDITIONAL_MAIN);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
//		lDatablockSize+=(lNewScriptSize-lOldScriptSize);
		lDatablockSize+=(ScriptSize-lOldScriptSize);
		SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

		// 3. SCENE2_MAIN
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		lDatablockSize=thisDatablock.GetLong(hFile);
//		lDatablockSize+=(lNewScriptSize-lOldScriptSize);
		lDatablockSize+=(ScriptSize-lOldScriptSize);
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
	}
	else
	{
		// look for object

		lDummyOffset=lObjectOffset=FindObject(hFile,szObjectName,SPECIAL_MAIN);
		if(lObjectOffset==-1)
			return(-1);

		// get lenght
		SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
		lObjDatablockSize=thisDatablock.GetLong(hFile);

		// now check if the object already got a script section
		lScriptOffset=FindScript(hFile,szObjectName);
		if(lScriptOffset==-1)
		{
			// no
			// buffer data after object (ie new 21AE)
			SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize),0,FILE_BEGIN);
			lDummyOffset+=lObjDatablockSize;
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
			// Now write script signature
			WriteFile(hFile,&sSig,6,&lBytesWritten,NULL);
			// Get Script File Size
//			lNewScriptSize=myHelper.GetFileSize(szFileName);
			// Write script size
//			WriteFile(hFile,(char *)&lNewScriptSize,sizeof(lNewScriptSize),&lBytesWritten,NULL);
			WriteFile(hFile,(char *)&ScriptSize,sizeof(ScriptSize),&lBytesWritten,NULL);
			// Write the script
//			myHelper.PasteBufferedDataFM(hFile,szFileName,false);
			WriteFile(hFile,Buffer,ScriptSize,&lBytesWritten,NULL);
			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// ---NOW: change datablock sizes
			// 2. 24AE datablock
//			SetFilePointer(hFile,(lObjectOffset+6),0,FILE_BEGIN);
//			lDatablockOffset=thisDatablock.FindSubDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
			lDatablockOffset=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
//			lDatablockSize+=(lNewScriptSize+10);
			lDatablockSize+=(ScriptSize+10);
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

			// 3. 21AE
			SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
//			lDatablockSize+=(lNewScriptSize+10);
			lDatablockSize+=(ScriptSize+10);
			SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

			// 4. 20AE datablock
			lDatablockOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
//			lDatablockSize+=(lNewScriptSize+10);
			lDatablockSize+=(ScriptSize+10);
			SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

			// 5. SL datablock
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lDatablockSize=thisDatablock.GetLong(hFile);
//			lDatablockSize+=(lNewScriptSize+10);
			lDatablockSize+=(ScriptSize+10);
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
		}
		else
		{
			// yes
			// at first we have to determine what object type it is
			ReadObject(hFile,lObjectOffset,&Scene2ObjectInfo_t,true);
			if(Scene2ObjectInfo_t.lType==SPECIAL_OBJECT_TYPE_SCRIPT)
			{
				// Object is a script so we have:
				// 01 00/01 00 00 00 01 00 00 00 00 <size> <script>

				// First get size of previous script
				SetFilePointer(hFile,(lScriptOffset+10),0,FILE_BEGIN);
				lOldScriptSize=thisDatablock.GetLong(hFile);
				// buffer data after object (ie new 21AE)
				SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize),0,FILE_BEGIN);
				lDummyOffset+=lObjDatablockSize;
//				myHelper.BufferData(hFile,"buffer.tmp",-1);
				myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
				// Go back to script
				SetFilePointer(hFile,(lScriptOffset+14),0,FILE_BEGIN);
				// Now write the script + save script size (lNewScriptSize)
//				myHelper.PasteBufferedDataFM(hFile,szFileName,false);
				WriteFile(hFile,Buffer,ScriptSize,&lBytesWritten,NULL);
//				lNewScriptSize=myHelper.GetFileSize(szFileName);
				// Paste other data
				myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

				SetEndOfFile(hFile);

				// ---NOW: change datablock sizes
				// 1. Script Size
				SetFilePointer(hFile,(lScriptOffset+10),0,FILE_BEGIN);
//				WriteFile(hFile,(char *)&lNewScriptSize,sizeof(lNewScriptSize),&lBytesWritten,NULL);
				WriteFile(hFile,(char *)&ScriptSize,sizeof(ScriptSize),&lBytesWritten,NULL);
				// 2. 24AE datablock
//				SetFilePointer(hFile,(lObjectOffset+6),0,FILE_BEGIN);
//				lDatablockOffset=thisDatablock.FindSubDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
				lDatablockOffset=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
//				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				lDatablockSize+=(ScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 3. 21AE
				SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
//				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				lDatablockSize+=(ScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 4. 20AE datablock
				lDatablockOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
//				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				lDatablockSize+=(ScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 5. SL datablock
				SetFilePointer(hFile,2,0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
//				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				lDatablockSize+=(ScriptSize-lOldScriptSize);
				SetFilePointer(hFile,2,0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
			}
			else
			{
				// Object is NOT a script so we have:
				// 01 01 00 00 00 00 <size> <script>

				// First get size of previous script
				SetFilePointer(hFile,(lScriptOffset+6),0,FILE_BEGIN);
				lOldScriptSize=thisDatablock.GetLong(hFile);
				// buffer data after object (ie new 21AE)
				SetFilePointer(hFile,(lObjectOffset+lObjDatablockSize),0,FILE_BEGIN);
				lDummyOffset+=lObjDatablockSize;
//				myHelper.BufferData(hFile,"buffer.tmp",-1);
				myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);
				// Go back to script
				SetFilePointer(hFile,(lScriptOffset+10),0,FILE_BEGIN);
				// Now write the script + save script size (lNewScriptSize)
//				myHelper.PasteBufferedDataFM(hFile,szFileName,false);
				WriteFile(hFile,Buffer,ScriptSize,&lBytesWritten,NULL);
//				lNewScriptSize=myHelper.GetFileSize(szFileName);
				// Paste other data
				myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

				SetEndOfFile(hFile);

				// ---NOW: change datablock sizes
				// 1. Script Size
				SetFilePointer(hFile,(lScriptOffset+6),0,FILE_BEGIN);
//				WriteFile(hFile,(char *)&lNewScriptSize,sizeof(lNewScriptSize),&lBytesWritten,NULL);
				WriteFile(hFile,(char *)&ScriptSize,sizeof(ScriptSize),&lBytesWritten,NULL);
				// 2. 24AE datablock
//				SetFilePointer(hFile,(lObjectOffset+6),0,FILE_BEGIN);
//				lDatablockOffset=thisDatablock.FindSubDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
				lDatablockOffset=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_DATA,(lObjectOffset+6));
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
//				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				lDatablockSize+=(ScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 3. 21AE
				SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
//				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				lDatablockSize+=(ScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lObjectOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 4. 20AE datablock
				lDatablockOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
//				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				lDatablockSize+=(ScriptSize-lOldScriptSize);
				SetFilePointer(hFile,(lDatablockOffset+2),0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);

				// 5. SL datablock
				SetFilePointer(hFile,2,0,FILE_BEGIN);
				lDatablockSize=thisDatablock.GetLong(hFile);
//				lDatablockSize+=(lNewScriptSize-lOldScriptSize);
				lDatablockSize+=(ScriptSize-lOldScriptSize);
				SetFilePointer(hFile,2,0,FILE_BEGIN);
				WriteFile(hFile,(char *)&lDatablockSize,sizeof(lDatablockSize),&lBytesWritten,NULL);
			}
		}
	}
	return 0;
}

// Used to find the script section of a special object
long CScene2::FindScript(HANDLE hFile, char * szObjectName)
{
	CHelper myHelper;
	long lObjectOffset=0,
		lObjDatablockSize=0;
	char cSig;
//	unsigned long lBytesRead=0;
	CDatablock thisDatablock;
	ObjectInfo_t objinfo;
	int loc=0;
	
	// Find the object itself
	lObjectOffset=FindObject(hFile,szObjectName,SPECIAL_MAIN);
	if(lObjectOffset==-1)
		return(-1);

	// Get Object Type
	ReadObject(hFile,lObjectOffset,&objinfo,true);

	// we won't allow script extraction if object is neither a char nor a script
	if(objinfo.lType!=SPECIAL_OBJECT_TYPE_SCRIPT && objinfo.lType!=SPECIAL_OBJECT_TYPE_CHARACTER
		&& objinfo.lType!=SPECIAL_OBJECT_TYPE_PLAYER)
		return(-1);

	// Find 24 AE
	loc=thisDatablock.FindObjectDatablock(hFile,SPECIAL_OBJECT_DATA,lObjectOffset+6);
	if(loc==SCENE2_EOF)
		return(-1);

	// now we add a constant value (in dependency of the objecttype) to get the addy of the script
	loc+=(objinfo.lType==SPECIAL_OBJECT_TYPE_CHARACTER || objinfo.lType==SPECIAL_OBJECT_TYPE_PLAYER) ? 0x4f : 6;

	// ok now we know where to find the object so let's start with looking for the
	// script signature
	SetFilePointer(hFile,loc,0,FILE_BEGIN);
	cSig=thisDatablock.GetByte(hFile);

	// the following code is not necessary, but it's added for security reasons
	// it is supposed to check whether a comment-chunk follows after the 24AE block
	// because the if(cSig...) would detect it as a script signature and MafiaLib would
	// become stupid again :/
	// We actually use the aspect that each chunk must have a lenght that follows after the
	// chunk ID. So we check whether the BYTE 2 bytes after the script signature is 0. If
	// it is we should have found a proper signature, otherwise it seems to be a comment-chunk.

	SetFilePointer(hFile,1,0,FILE_CURRENT);
	cSig=thisDatablock.GetByte(hFile);

	if(cSig)
		return(-1);

	if(cSig==1 || !cSig)	// the beginning of a signature is either 00 or 01...
		return(loc);	// will return addy even if script-obj doesn't got a script
	return(-1);
}
// Looks for a particular car in a traffic object
/*long CScene2::FindCar(char * szCarName)
{
	return 0;
}
*/
// Returns the build of MafiaLib Engine
int CScene2::GetBuild(void)
{
	return(MAFIALIB_BUILD);
}

// Returns version of Scene2.bin module of MafiaLib
float CScene2::GetVersion(void)
{
	return(MAFIALIB_VERSION);
}

// Returns minor version of Scene2.bin module of MafiaLib
/*int CScene2::GetSubVersion(void)
{
	return(MAFIALIB_SUB_VERSION);
}*/

// Adds or Replaces Objects (extended version: no szScriptFileName anymore, managed internally)
int CScene2::AddObjectEx(HANDLE hFile, ObjectInfo_t * Scene2ObjectInfo_t, char * szObjectToReplace)
{
//	char dummy[256];
	int i, cur=0;
	long lOffset=0,
		lDummyOffset=0,			// dummy offset for BufferDataFM
//		lScriptOffset=0,
		lScriptSize=-1;			// to avoid problems
	unsigned long lBytesWritten=0/*, lBytesRead=0*/;
	// long lSizes[8];
	long lNormalMainSize=0,
		lSpecialMainSize=0,
		lScene2Size=0,
		lDatablockSize=0,
		lObjectSize=0, 		// for replace: how big was object before replace
		lLightMain=0,		// size of lightmain datablock
		lType06Main=0,		// size of type 06 main datablock
		lNumSave=0,			// for storing temporary numbers
		lDummy=0;			// the dummy

	const unsigned char null[3]={0,0,0};	// if we have to fill sth w 3 null
	const unsigned char bignull[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};	// for Traffic Item slack space
	const unsigned char bigcd[17]={0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD,0xCD};	// ped item slack space
	const unsigned char one[3]={1,1,1};				// if we need ones
	const unsigned char datatype_character=0x06;		// datatype for character
	const short datatype_movable=0x0102;	// datatype for movable objects
	const unsigned char datatype_traffic=0x05;		// datatype for traffic objects
	const unsigned char datatype_object=0x01;		// datatype for objects like peds
	const unsigned char datatype_door=0x08;			// datatype for door objects
//	char dummy;				// the dummy >:)

	HANDLE hFileOwn;

	// Debug Vars
//	long Size1=0, Size2=0, Size3=0;

	// check for object type to add
	switch(Scene2ObjectInfo_t->lType)
	{
		case S_NORMAL_OBJECT_TYPE_MODEL:

			// Remove f of object type indicator
			Scene2ObjectInfo_t->lType&=0x0F;

			// check if the object is supposed to be replaced
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,NORMAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find NORMAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);

				// Find END of NORMAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				//	lOffset2+=lOffset+lDatablockSize;				//save address of OBJECT_MAIN (end of NORMAL_MAIN)
				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}
				// save all upcoming data
//				myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size
			// For every datablock we have: Datasize+6

			// General
			lDatablockSize=6;
			// 1. Type
			lDatablockSize+=0x0A;

			// 2. Name
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 3. Position
			lDatablockSize+=0x12;

			// 4. Rotation
			lDatablockSize+=0x16;

			// 5. Size
			lDatablockSize+=0x12;

			// 6. Additional Translation
			lDatablockSize+=0x12;

			// 7. Sector Name
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sSectorName)+13; // 12 - sector datablock + 12 - name datablock + 1 - the 0 at the end

			// 8. Model (if we need one)
			if(Scene2ObjectInfo_t->bUseModel)
				lDatablockSize+=strlen(Scene2ObjectInfo_t->sModelName)+7;

			// Create a new main datablock for our object
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_MAIN,lDatablockSize);

			// Create sub datablocks

			// 1. Type
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_TYPE,sizeof(Scene2ObjectInfo_t->lType)+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 2. Name
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Position
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_POS,sizeof(Scene2ObjectInfo_t->fPos[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fPos[i],sizeof(Scene2ObjectInfo_t->fPos[0]),&lBytesWritten,NULL);

			// 4. Rotation
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_ROT,sizeof(Scene2ObjectInfo_t->fRot[0])*4+6);
			for(i=0;i<4;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fRot[i],sizeof(Scene2ObjectInfo_t->fRot[0]),&lBytesWritten,NULL);

			// 5. Size
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_DIMENSION,sizeof(Scene2ObjectInfo_t->fSize[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSize[i],sizeof(Scene2ObjectInfo_t->fSize[0]),&lBytesWritten,NULL);

			// 6. Additional Translation
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_ADDTRANSLATION,sizeof(Scene2ObjectInfo_t->fAddTrans[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fAddTrans[i],sizeof(Scene2ObjectInfo_t->fAddTrans[0]),&lBytesWritten,NULL);

			// 7. Sector Name
			// Create Main Datablock
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_SECTOR,strlen(Scene2ObjectInfo_t->sSectorName)+13);

			// now create sub-datablock with name of sector
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,strlen(Scene2ObjectInfo_t->sSectorName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sSectorName,strlen(Scene2ObjectInfo_t->sSectorName)+1,&lBytesWritten,NULL);

			// 8. Model
			if(Scene2ObjectInfo_t->bUseModel)
			{
				thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_MODEL,strlen(Scene2ObjectInfo_t->sModelName)+7);
				WriteFile(hFileOwn,Scene2ObjectInfo_t->sModelName,strlen(Scene2ObjectInfo_t->sModelName)+1,&lBytesWritten,NULL);
			}

			// Close Handle to Object file
			CloseHandle(hFileOwn);

			// Add my object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Add other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// -------------------------------------------------
			// Finally, change NORMAL_MAIN size
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

			lNormalMainSize=thisDatablock.GetLong(hFile);

			if(szObjectToReplace!=NULL)
				lNormalMainSize+=(lDatablockSize-lObjectSize);
			else
				lNormalMainSize+=lDatablockSize;

			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lNormalMainSize,4,&lBytesWritten,NULL);

			// Change size of scene2.bin file in header
			lScene2Size=GetFileSize(hFile);

			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
	
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,4,&lBytesWritten,NULL);
			break;

		case S_NORMAL_OBJECT_TYPE_06:

			// Remove f of object type indicator
			Scene2ObjectInfo_t->lType&=0x0F;

			// check if the object is supposed to be replaced
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,NORMAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find NORMAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);

				// Find END of NORMAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				//	lOffset2+=lOffset+lDatablockSize;				//save address of OBJECT_MAIN (end of NORMAL_MAIN)
				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}
				// save all upcoming data
//				myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size
			// For every datablock we have: Datasize+6

			// General
			lDatablockSize=6;
			// 1. Type
			lDatablockSize+=0x0A;

			// 2. Name
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 3. Position
			lDatablockSize+=0x12;

			// 4. Rotation
			lDatablockSize+=0x16;

			// 5. Size
			lDatablockSize+=0x12;

			// 6. Additional Translation
			lDatablockSize+=0x12;

			// 7. Sector Name
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sSectorName)+13; // 12 - sector datablock + 12 - name datablock + 1 - the 0 at the end

			// 8. Type 06 Main Datablock
			lDatablockSize+=lType06Main=0x1E;

			// Create a new main datablock for our object
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_MAIN,lDatablockSize);

			// Create sub datablocks

			// 1. Type
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_TYPE,sizeof(Scene2ObjectInfo_t->lType)+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 2. Name
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Position
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_POS,sizeof(Scene2ObjectInfo_t->fPos[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fPos[i],sizeof(Scene2ObjectInfo_t->fPos[0]),&lBytesWritten,NULL);

			// 4. Rotation
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_ROT,sizeof(Scene2ObjectInfo_t->fRot[0])*4+6);
			for(i=0;i<4;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fRot[i],sizeof(Scene2ObjectInfo_t->fRot[0]),&lBytesWritten,NULL);

			// 5. Size
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_DIMENSION,sizeof(Scene2ObjectInfo_t->fSize[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSize[i],sizeof(Scene2ObjectInfo_t->fSize[0]),&lBytesWritten,NULL);

			// 6. Additional Translation
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_ADDTRANSLATION,sizeof(Scene2ObjectInfo_t->fAddTrans[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fAddTrans[i],sizeof(Scene2ObjectInfo_t->fAddTrans[0]),&lBytesWritten,NULL);

			// 7. Sector Name
			// Create Main Datablock
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_SECTOR,strlen(Scene2ObjectInfo_t->sSectorName)+13);

			// now create sub-datablock with name of sector
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,strlen(Scene2ObjectInfo_t->sSectorName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sSectorName,strlen(Scene2ObjectInfo_t->sSectorName)+1,&lBytesWritten,NULL);

			// 8. Type 06 Main
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_TYPE_06_DATA,lType06Main);
			for(i=0;i<6;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fVal[i],sizeof(Scene2ObjectInfo_t->fVal[0]),&lBytesWritten,NULL);


			// Close Handle to Object file
			CloseHandle(hFileOwn);

			// Add my object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Add other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// -------------------------------------------------
			// Finally, change NORMAL_MAIN size
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

			lNormalMainSize=thisDatablock.GetLong(hFile);

			if(szObjectToReplace!=NULL)
				lNormalMainSize+=(lDatablockSize-lObjectSize);
			else
				lNormalMainSize+=lDatablockSize;

			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lNormalMainSize,4,&lBytesWritten,NULL);

			// Change size of scene2.bin file in header
			lScene2Size=GetFileSize(hFile);

			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
	
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,4,&lBytesWritten,NULL);
			break;

		case S_NORMAL_OBJECT_TYPE_CAMERA:

			// Remove f of object type indicator
			Scene2ObjectInfo_t->lType&=0x0F;

			// check if the object is supposed to be replaced
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,NORMAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find NORMAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);

				// Find END of NORMAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				//	lOffset2+=lOffset+lDatablockSize;				//save address of OBJECT_MAIN (end of NORMAL_MAIN)
				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}
				// save all upcoming data
//				myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size
			// For every datablock we have: Datasize+6

			// General
			lDatablockSize=6;
			// 1. Type
			lDatablockSize+=0x0A;

			// 2. Name
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 3. Position
			lDatablockSize+=0x12;

			// 4. Rotation
			lDatablockSize+=0x16;

			// 5. Size
			lDatablockSize+=0x12;

			// 6. Additional Translation
			lDatablockSize+=0x12;

			// 7. Sector Name
			// we ONLY add sector if bSector==true
			if(Scene2ObjectInfo_t->bSector)
				lDatablockSize+=strlen(Scene2ObjectInfo_t->sSectorName)+13; // 12 - sector datablock + 12 - name datablock + 1 - the 0 at the end

			// 8. Type 06 Main Datablock
			// we ONLY add type 06 data if bExtended==true
			if(Scene2ObjectInfo_t->bExtended)
				lDatablockSize+=lType06Main=0x1E;

			// Create a new main datablock for our object
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_MAIN,lDatablockSize);

			// Create sub datablocks

			// 1. Type
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_TYPE,sizeof(Scene2ObjectInfo_t->lType)+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 2. Name
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Position
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_POS,sizeof(Scene2ObjectInfo_t->fPos[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fPos[i],sizeof(Scene2ObjectInfo_t->fPos[0]),&lBytesWritten,NULL);

			// 4. Rotation
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_ROT,sizeof(Scene2ObjectInfo_t->fRot[0])*4+6);
			for(i=0;i<4;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fRot[i],sizeof(Scene2ObjectInfo_t->fRot[0]),&lBytesWritten,NULL);

			// 5. Size
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_DIMENSION,sizeof(Scene2ObjectInfo_t->fSize[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSize[i],sizeof(Scene2ObjectInfo_t->fSize[0]),&lBytesWritten,NULL);

			// 6. Additional Translation
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_ADDTRANSLATION,sizeof(Scene2ObjectInfo_t->fAddTrans[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fAddTrans[i],sizeof(Scene2ObjectInfo_t->fAddTrans[0]),&lBytesWritten,NULL);

			// 7. Sector Name
			// we ONLY add sector if bSector==true
			if(Scene2ObjectInfo_t->bSector)
			{
				// Create Main Datablock
				thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_SECTOR,strlen(Scene2ObjectInfo_t->sSectorName)+13);

				// now create sub-datablock with name of sector
				thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,strlen(Scene2ObjectInfo_t->sSectorName)+7);
				WriteFile(hFileOwn,Scene2ObjectInfo_t->sSectorName,strlen(Scene2ObjectInfo_t->sSectorName)+1,&lBytesWritten,NULL);
			}

			// 8. Type 06 Main
			// we ONLY add type 06 data if bExtended==true
			if(Scene2ObjectInfo_t->bExtended)
			{
				thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_TYPE_06_DATA,lType06Main);
				for(i=0;i<6;i++)
					WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fVal[i],sizeof(Scene2ObjectInfo_t->fVal[0]),&lBytesWritten,NULL);
			}

			// Close Handle to Object file
			CloseHandle(hFileOwn);

			// Add my object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Add other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// -------------------------------------------------
			// Finally, change NORMAL_MAIN size
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

			lNormalMainSize=thisDatablock.GetLong(hFile);

			if(szObjectToReplace!=NULL)
				lNormalMainSize+=(lDatablockSize-lObjectSize);
			else
				lNormalMainSize+=lDatablockSize;

			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lNormalMainSize,4,&lBytesWritten,NULL);

			// Change size of scene2.bin file in header
			lScene2Size=GetFileSize(hFile);

			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
	
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,4,&lBytesWritten,NULL);
			break;

		case S_NORMAL_OBJECT_TYPE_LIGHT:

			// Remove f of object type indicator
			Scene2ObjectInfo_t->lType&=0x0F;

			// check if the object is supposed to be replaced
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,NORMAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find NORMAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);

				// Find END of NORMAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				//	lOffset2+=lOffset+lDatablockSize;				//save address of OBJECT_MAIN (end of NORMAL_MAIN)
				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}
				// save all upcoming data
//				myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size
			// For every datablock we have: Datasize+6

			// General
			lDatablockSize=6;
			// 1. Type
			lDatablockSize+=0x0A;

			// 2. Name
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 3. Position
			lDatablockSize+=0x12;

			// 4. Rotation
			lDatablockSize+=0x16;

			// 5. Size
			lDatablockSize+=0x12;

			// 6. Additional Translation
			lDatablockSize+=0x12;

			// 7. Sector Name
			if(strlen(Scene2ObjectInfo_t->sSectorName)!=0)
				lDatablockSize+=strlen(Scene2ObjectInfo_t->sSectorName)+13; // 12 - sector datablock + 12 - name datablock + 1 - the 0 at the end

			// 8. Light Main
			lDatablockSize+=lLightMain=6;

			// 9. Light Type
			lDatablockSize+=0x0A;
			lLightMain+=0x0A;

			// 10. Light Color
			lDatablockSize+=0x12;
			lLightMain+=0x12;

			// 11. Light Power
			lDatablockSize+=0x0A;
			lLightMain+=0x0A;

			// 12. Light Unk 1
			lDatablockSize+=0x0E;
			lLightMain+=0x0E;

			// 13. Light Range
			lDatablockSize+=0x0E;
			lLightMain+=0x0E;

			// 14. Light Flags
			lDatablockSize+=0x0A;
			lLightMain+=0x0A;

			// 15. Light Sectors
			do{
				lDatablockSize+=strlen(&Scene2ObjectInfo_t->sLightSectors[cur])+7;
				lLightMain+=strlen(&Scene2ObjectInfo_t->sLightSectors[cur])+7;
				cur+=strlen(&Scene2ObjectInfo_t->sLightSectors[cur])+1;
			}while(Scene2ObjectInfo_t->sLightSectors[cur]!=0);

			// Create a new main datablock for our object
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_MAIN,lDatablockSize);

			// Create sub datablocks

			// 1. Type
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_TYPE,sizeof(Scene2ObjectInfo_t->lType)+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 2. Name
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Position
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_POS,sizeof(Scene2ObjectInfo_t->fPos[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fPos[i],sizeof(Scene2ObjectInfo_t->fPos[0]),&lBytesWritten,NULL);

			// 4. Rotation
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_ROT,sizeof(Scene2ObjectInfo_t->fRot[0])*4+6);
			for(i=0;i<4;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fRot[i],sizeof(Scene2ObjectInfo_t->fRot[0]),&lBytesWritten,NULL);

			// 5. Size
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_DIMENSION,sizeof(Scene2ObjectInfo_t->fSize[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSize[i],sizeof(Scene2ObjectInfo_t->fSize[0]),&lBytesWritten,NULL);

			// 6. Additional Translation
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_ADDTRANSLATION,sizeof(Scene2ObjectInfo_t->fAddTrans[0])*3+6);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fAddTrans[i],sizeof(Scene2ObjectInfo_t->fAddTrans[0]),&lBytesWritten,NULL);

			// 7. Sector Name
			// Create Main Datablock
			// but check if light got a sector
			if(strlen(Scene2ObjectInfo_t->sSectorName)!=0)
			{
				thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_SECTOR,strlen(Scene2ObjectInfo_t->sSectorName)+13);

				// now create sub-datablock with name of sector
				thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,strlen(Scene2ObjectInfo_t->sSectorName)+7);
				WriteFile(hFileOwn,Scene2ObjectInfo_t->sSectorName,strlen(Scene2ObjectInfo_t->sSectorName)+1,&lBytesWritten,NULL);
			}

			// 8. Light Main
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_LIGHTNING_MAIN,lLightMain);

			// 9. Light Type
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_LIGHTNING_TYPE,sizeof(Scene2ObjectInfo_t->lLightType)+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lLightType,sizeof(Scene2ObjectInfo_t->lLightType),&lBytesWritten,NULL);

			// 10. Light Color
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_LIGHTNING_COLOR,sizeof(Scene2ObjectInfo_t->fRed)*3+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fRed,sizeof(Scene2ObjectInfo_t->fRed),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fGreen,sizeof(Scene2ObjectInfo_t->fGreen),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fBlue,sizeof(Scene2ObjectInfo_t->fBlue),&lBytesWritten,NULL);

			// 11. Light Power
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_LIGHTNING_POWER,sizeof(Scene2ObjectInfo_t->fPower)+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fPower,sizeof(Scene2ObjectInfo_t->fPower),&lBytesWritten,NULL);

			// 12. Light Unk_1
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_LIGHTNING_UNK_1,sizeof(Scene2ObjectInfo_t->fLightUnk1)*2+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fLightUnk1,sizeof(Scene2ObjectInfo_t->fLightUnk1),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fLightUnk2,sizeof(Scene2ObjectInfo_t->fLightUnk2),&lBytesWritten,NULL);

			// 13. Light Range
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_LIGHTNING_RANGE,sizeof(Scene2ObjectInfo_t->fLightNear)*2+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fLightNear,sizeof(Scene2ObjectInfo_t->fLightNear),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fLightFar,sizeof(Scene2ObjectInfo_t->fLightFar),&lBytesWritten,NULL);

			// 14. Light Flags
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_LIGHTNING_FLAGS,sizeof(Scene2ObjectInfo_t->lLightFlags)+6);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lLightFlags,sizeof(Scene2ObjectInfo_t->lLightFlags),&lBytesWritten,NULL);

			// 15. Light Sectors
			cur=0;
			do{
				thisDatablock.CreateDatablock(hFileOwn,NORMAL_LIGHTNING_SECTOR,strlen(&Scene2ObjectInfo_t->sLightSectors[cur])+7);
				WriteFile(hFileOwn,&Scene2ObjectInfo_t->sLightSectors[cur],strlen(&Scene2ObjectInfo_t->sLightSectors[cur])+1,&lBytesWritten,NULL);
				cur+=(int)strlen(&Scene2ObjectInfo_t->sLightSectors[cur])+1;
			}while(Scene2ObjectInfo_t->sLightSectors[cur]!=0);

			// Close Handle to Object file
			CloseHandle(hFileOwn);

			// Add my object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Add other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// -------------------------------------------------
			// Finally, change NORMAL_MAIN size
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

			lNormalMainSize=thisDatablock.GetLong(hFile);

			if(szObjectToReplace!=NULL)
				lNormalMainSize+=(lDatablockSize-lObjectSize);
			else
				lNormalMainSize+=lDatablockSize;

			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lNormalMainSize,4,&lBytesWritten,NULL);

			// Change size of scene2.bin file in header
			lScene2Size=GetFileSize(hFile);

			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
	
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,4,&lBytesWritten,NULL);
			break;

		case NORMAL_OBJECT_TYPE_SECTOR:
			// check if the object is supposed to be replaced
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,NORMAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find NORMAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);

				// Find END of NORMAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				//	lOffset2+=lOffset+lDatablockSize;				//save address of OBJECT_MAIN (end of NORMAL_MAIN)
				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}
				// save all upcoming data
//				myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// setup size
			lDatablockSize=0xFE;		// size of normal sector object without name
			// add size of name
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+1;

			// setup datablocks

			// main
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_MAIN,lDatablockSize);

			// Name
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// sector main
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_MAIN,0xF2);

			// NORMAL_SECTOR_UNK_1
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_1,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal1,sizeof(Scene2ObjectInfo_t->fSectorVal1),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_2
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_2,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal2,sizeof(Scene2ObjectInfo_t->fSectorVal2),&lBytesWritten,NULL);

			// DATABLOCK_COMMENT
			thisDatablock.CreateDatablock(hFileOwn,DATABLOCK_COMMENT,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal3,sizeof(Scene2ObjectInfo_t->fSectorVal3),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_3
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_3,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal4,sizeof(Scene2ObjectInfo_t->fSectorVal4),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_4
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_4,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal5,sizeof(Scene2ObjectInfo_t->lSectorVal5),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_5
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_5,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal6,sizeof(Scene2ObjectInfo_t->fSectorVal6),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_6
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_6,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal7,sizeof(Scene2ObjectInfo_t->lSectorVal7),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_7
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_7,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal8,sizeof(Scene2ObjectInfo_t->fSectorVal8),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_8
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_8,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal9,sizeof(Scene2ObjectInfo_t->lSectorVal9),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_9
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_9,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal10,sizeof(Scene2ObjectInfo_t->fSectorVal10),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_10
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_10,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal11,sizeof(Scene2ObjectInfo_t->fSectorVal11),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_11
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_11,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal12,sizeof(Scene2ObjectInfo_t->lSectorVal12),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_12
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_12,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal13,sizeof(Scene2ObjectInfo_t->lSectorVal13),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_13
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_13,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal14,sizeof(Scene2ObjectInfo_t->lSectorVal14),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_14
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_14,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal15,sizeof(Scene2ObjectInfo_t->lSectorVal15),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_15
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_15,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal16,sizeof(Scene2ObjectInfo_t->fSectorVal16),&lBytesWritten,NULL);

			// NORMAL_OBJECT_OR_SECTOR_NAME
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_OBJECT_OR_SECTOR_NAME,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal17,sizeof(Scene2ObjectInfo_t->lSectorVal17),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_17
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_17,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal18,sizeof(Scene2ObjectInfo_t->fSectorVal18),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_18
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_18,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSectorVal19,sizeof(Scene2ObjectInfo_t->fSectorVal19),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_19
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_19,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal20,sizeof(Scene2ObjectInfo_t->lSectorVal20),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_20
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_20,0x12);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal21[i],sizeof(Scene2ObjectInfo_t->lSectorVal21[i]),&lBytesWritten,NULL);

			// NORMAL_SECTOR_UNK_21
			thisDatablock.CreateDatablock(hFileOwn,NORMAL_SECTOR_UNK_21,0x12);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSectorVal22[i],sizeof(Scene2ObjectInfo_t->lSectorVal22[i]),&lBytesWritten,NULL);

			// Close Handle to Object file
			CloseHandle(hFileOwn);

			// Add my object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Add other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// -------------------------------------------------
			// Finally, change NORMAL_MAIN size
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

			lNormalMainSize=thisDatablock.GetLong(hFile);

			if(szObjectToReplace!=NULL)
				lNormalMainSize+=(lDatablockSize-lObjectSize);
			else
				lNormalMainSize+=lDatablockSize;

			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lNormalMainSize,4,&lBytesWritten,NULL);

			// Change size of scene2.bin file in header
			lScene2Size=GetFileSize(hFile);

			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
	
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,4,&lBytesWritten,NULL);

			break;

		case SPECIAL_OBJECT_TYPE_CAR:
			// car is simple - we easily add the cardata[CARDATASIZE] data
			// Find SPECIAL_MAIN
			lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

			// Find END of SPECIAL_MAIN
			lDatablockSize=thisDatablock.GetLong(hFile);

			SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
			lDummyOffset+=lDatablockSize;

			// buffer data
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// write cardata
			WriteFile(hFile,cardata,CARDATASIZE,&lBytesWritten,NULL);

			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			// Change sizes

			// 1. SPECIAL_MAIN
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			lSpecialMainSize+=CARDATASIZE;
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);

			// 2. SCENE2_MAIN
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lScene2Size=thisDatablock.GetLong(hFile);
			lScene2Size+=CARDATASIZE;
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,sizeof(lScene2Size),&lBytesWritten,NULL);
			break;

		case SPECIAL_OBJECT_TYPE_SCRIPT:
			// script is simple - we easily add the scriptdata[SCRIPTDATASIZE] data
			// Find SPECIAL_MAIN
			lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

			// Find END of SPECIAL_MAIN
			lDatablockSize=thisDatablock.GetLong(hFile);

			SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
			lDummyOffset+=lDatablockSize;

			// buffer data
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// write scriptdata
			WriteFile(hFile,scriptdata,SCRIPTDATASIZE,&lBytesWritten,NULL);

			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			// Change sizes

			// 1. SPECIAL_MAIN
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			lSpecialMainSize+=SCRIPTDATASIZE;
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);

			// 2. SCENE2_MAIN
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lScene2Size=thisDatablock.GetLong(hFile);
			lScene2Size+=SCRIPTDATASIZE;
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,sizeof(lScene2Size),&lBytesWritten,NULL);
			break;

		case ADDITIONAL_OBJECT_TYPE_SCRIPT:
			// script is simple - we easily add the initscriptdata[INITSCRIPTDATASIZE] data
			// Find ADDITIONAL_MAIN
			lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,ADDITIONAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

			// Find END of ADDITIONAL_MAIN
			lDatablockSize=thisDatablock.GetLong(hFile);

			SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of ADDITIONAL_MAIN
			lDummyOffset+=lDatablockSize;

			// buffer data
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// write scriptdata
			WriteFile(hFile,initscriptdata,INITSCRIPTDATASIZE,&lBytesWritten,NULL);

			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			// Change sizes

			// 1. ADDITIONAL_MAIN
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			lSpecialMainSize+=INITSCRIPTDATASIZE;
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);

			// 2. SCENE2_MAIN
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lScene2Size=thisDatablock.GetLong(hFile);
			lScene2Size+=INITSCRIPTDATASIZE;
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,sizeof(lScene2Size),&lBytesWritten,NULL);
			break;

		case SPECIAL_OBJECT_TYPE_SHOOTABLE:
			// replace existing object?
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,SPECIAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				// buffer script
				lScriptSize=ExtractObjectScript(hFile,szObjectToReplace,SPECIAL_MAIN,"buffer_99.tmp");

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find SPECIAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find END of SPECIAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}

			// save all upcoming data
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size

			lDatablockSize=6;

			// 1. Name Datablock
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 2. Type Datablock
			lDatablockSize+=0x0A;

			// 3. Data Datablock
			lDatablockSize+=0x25;

			//--------NOW WRITE DATA

			// 1. Main Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_MAIN,lDatablockSize);

			// 2. Name Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Type Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_TYPE,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 4. Data Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_DATA,0x25);

//			WriteFile(hFileOwn,(char *)&,sizeof(),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&datatype_movable,sizeof(datatype_movable),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fMovVal1,sizeof(Scene2ObjectInfo_t->fMovVal1),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fMovVal2,sizeof(Scene2ObjectInfo_t->fMovVal2),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fWeight,sizeof(Scene2ObjectInfo_t->fWeight),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fMovVal3,sizeof(Scene2ObjectInfo_t->fMovVal3),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fMovVal4,sizeof(Scene2ObjectInfo_t->fMovVal4),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lSound,sizeof(Scene2ObjectInfo_t->lSound),&lBytesWritten,NULL);
			WriteFile(hFileOwn,&one,1,&lBytesWritten,NULL);	//strange thing but the 1 needs to be put :|
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lMovVal5,sizeof(Scene2ObjectInfo_t->lMovVal5),&lBytesWritten,NULL);

			CloseHandle(hFileOwn);

			// Paste object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// Change sizes

			// 1. SPECIAL_MAIN
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lSpecialMainSize+=(lDatablockSize-lObjectSize);
			else
				lSpecialMainSize+=lDatablockSize;
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);

			// 2. SCENE2_MAIN
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lScene2Size=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,sizeof(lScene2Size),&lBytesWritten,NULL);

			// Append script if one was specified
			if(lScriptSize!=-1)
			{
				AddObjectScript(hFile,Scene2ObjectInfo_t->sName,"buffer_99.tmp",SPECIAL_MAIN);
				DeleteFile("buffer_99.tmp");
			}

			break;

		case SPECIAL_OBJECT_TYPE_RACECAM:
			// replace existing object?
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,SPECIAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				// buffer script
				lScriptSize=ExtractObjectScript(hFile,szObjectToReplace,SPECIAL_MAIN,"buffer_99.tmp");

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find SPECIAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find END of SPECIAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}

			// save all upcoming data
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size

			lDatablockSize=6;

			// 1. Name Datablock
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 2. Type Datablock
			lDatablockSize+=0x0A;

			// 3. Data Datablock
			lDatablockSize+=0x1e;

			//--------NOW WRITE DATA

			// 1. Main Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_MAIN,lDatablockSize);

			// 2. Name Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Type Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_TYPE,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 4. Data Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_DATA,0x1e);

//			WriteFile(hFileOwn,(char *)&,sizeof(),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lRaceCamVal1,sizeof(Scene2ObjectInfo_t->lRaceCamVal1),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fRaceCamVal2,sizeof(Scene2ObjectInfo_t->fRaceCamVal2),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fRaceCamVal3,sizeof(Scene2ObjectInfo_t->fRaceCamVal3),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fRaceCamVal4,sizeof(Scene2ObjectInfo_t->fRaceCamVal4),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lRaceCamVal5,sizeof(Scene2ObjectInfo_t->lRaceCamVal5),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lRaceCamVal6,sizeof(Scene2ObjectInfo_t->lRaceCamVal6),&lBytesWritten,NULL);

			CloseHandle(hFileOwn);

			// Paste object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// Change sizes

			// 1. SPECIAL_MAIN
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lSpecialMainSize+=(lDatablockSize-lObjectSize);
			else
				lSpecialMainSize+=lDatablockSize;
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);

			// 2. SCENE2_MAIN
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lScene2Size=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,sizeof(lScene2Size),&lBytesWritten,NULL);

			// Append script if one was specified
			if(lScriptSize!=-1)
			{
				AddObjectScript(hFile,Scene2ObjectInfo_t->sName,"buffer_99.tmp",SPECIAL_MAIN);
				DeleteFile("buffer_99.tmp");
			}

			break;

		case SPECIAL_OBJECT_TYPE_TRAFFIC:
			// replace existing object?
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,SPECIAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				// buffer script
				lScriptSize=ExtractObjectScript(hFile,szObjectToReplace,SPECIAL_MAIN,"buffer_99.tmp");

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find SPECIAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find END of SPECIAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}

			// save all upcoming data
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size

			lDatablockSize=6;

			// 1. Name Datablock
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 2. Type Datablock
			lDatablockSize+=0x0A;

			// 3. Data Datablock
			lDatablockSize+=30+(Scene2ObjectInfo_t->lNumOfCars*32);

			//--------NOW WRITE DATA

			// 1. Main Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_MAIN,lDatablockSize);

			// 2. Name Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Type Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_TYPE,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 4. Data Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_DATA,30+(Scene2ObjectInfo_t->lNumOfCars*32));

//			WriteFile(hFileOwn,(char *)&,sizeof(),&lBytesWritten,NULL);
			WriteFile(hFileOwn,&datatype_traffic,sizeof(datatype_traffic),&lBytesWritten,NULL);
			WriteFile(hFileOwn,null,sizeof(null),&lBytesWritten,NULL);
			for(i=0;i<3;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fTrafficVal[i],sizeof(Scene2ObjectInfo_t->fTrafficVal[i]),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lDensity,sizeof(Scene2ObjectInfo_t->lDensity),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lNumOfCars,sizeof(Scene2ObjectInfo_t->lNumOfCars),&lBytesWritten,NULL);

			// now write car entries
			for(i=0;i<Scene2ObjectInfo_t->lNumOfCars;i++)
			{
				if(!Scene2ObjectInfo_t->bDel[i])
				{
					WriteFile(hFileOwn,Scene2ObjectInfo_t->sCarname[i],strlen(Scene2ObjectInfo_t->sCarname[i])+1,&lBytesWritten,NULL);
					WriteFile(hFileOwn,bignull,20-(strlen(Scene2ObjectInfo_t->sCarname[i])+1),&lBytesWritten,NULL);
					WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fDensity[i],sizeof(Scene2ObjectInfo_t->fDensity[i]),&lBytesWritten,NULL);
					WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lColor[i],sizeof(Scene2ObjectInfo_t->lColor[i]),&lBytesWritten,NULL);
					WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->bPolice[i],sizeof(Scene2ObjectInfo_t->bPolice[i]),&lBytesWritten,NULL);
					WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->bFlag2[i],sizeof(Scene2ObjectInfo_t->bFlag2[i]),&lBytesWritten,NULL);
					WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->bGangster[i],sizeof(Scene2ObjectInfo_t->bGangster[i]),&lBytesWritten,NULL);
					WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->bFlag4[i],sizeof(Scene2ObjectInfo_t->bFlag4[i]),&lBytesWritten,NULL);
				}
				else
					Scene2ObjectInfo_t->lNumOfCars++;
			}

			CloseHandle(hFileOwn);

			// Paste object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// Change sizes

			// 1. SPECIAL_MAIN
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lSpecialMainSize+=(lDatablockSize-lObjectSize);
			else
				lSpecialMainSize+=lDatablockSize;
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);

			// 2. SCENE2_MAIN
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lScene2Size=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,sizeof(lScene2Size),&lBytesWritten,NULL);

			// Append script if one was specified
			if(lScriptSize!=-1)
			{
				AddObjectScript(hFile,Scene2ObjectInfo_t->sName,"buffer_99.tmp",SPECIAL_MAIN);
				DeleteFile("buffer_99.tmp");
			}

			break;

		case SPECIAL_OBJECT_TYPE_PEDS:
			// replace existing object?
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,SPECIAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				// buffer script
				lScriptSize=ExtractObjectScript(hFile,szObjectToReplace,SPECIAL_MAIN,"buffer_99.tmp");

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find SPECIAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find END of SPECIAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}

			// save all upcoming data
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size

			lDatablockSize=6;

			// 1. Name Datablock
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 2. Type Datablock
			lDatablockSize+=0x0A;

			// 3. Data Datablock
			lDatablockSize+=39+(Scene2ObjectInfo_t->lNumOfPeds*21);

			//--------NOW WRITE DATA

			// 1. Main Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_MAIN,lDatablockSize);

			// 2. Name Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Type Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_TYPE,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 4. Data Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_DATA,39+(Scene2ObjectInfo_t->lNumOfPeds*21));

//			WriteFile(hFileOwn,(char *)&,sizeof(),&lBytesWritten,NULL);
			WriteFile(hFileOwn,&datatype_object,sizeof(datatype_object),&lBytesWritten,NULL);
			WriteFile(hFileOwn,null,sizeof(null),&lBytesWritten,NULL);
			// put 01 (dunno why, but we do it)
			WriteFile(hFileOwn,one,1,&lBytesWritten,NULL);
			for(i=0;i<5;i++)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fPedVal[i],sizeof(Scene2ObjectInfo_t->fPedVal[i]),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lGlobalPedDensity,sizeof(Scene2ObjectInfo_t->lGlobalPedDensity),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lNumOfPeds,sizeof(Scene2ObjectInfo_t->lNumOfPeds),&lBytesWritten,NULL);

			// save lNumOfPeds
			lNumSave=Scene2ObjectInfo_t->lNumOfPeds;
			// now write peds entries
			for(i=0;i<Scene2ObjectInfo_t->lNumOfPeds;i++)
			{
				if(!Scene2ObjectInfo_t->bDel[i])
				{
					lDummy=strlen(Scene2ObjectInfo_t->sPedname[i]);
					// check if namelen=1
					if(lDummy==1)
					{
						lDummy+=1+strlen(&Scene2ObjectInfo_t->sPedname[i][2])+1;  //-> the stuped >:( 0\0l12.i3d entry so we use another method
						WriteFile(hFileOwn,Scene2ObjectInfo_t->sPedname[i],lDummy,&lBytesWritten,NULL);
						WriteFile(hFileOwn,bigcd,17-lDummy,&lBytesWritten,NULL);
					}
					else
					{
						WriteFile(hFileOwn,Scene2ObjectInfo_t->sPedname[i],strlen(Scene2ObjectInfo_t->sPedname[i])+1,&lBytesWritten,NULL);
						WriteFile(hFileOwn,bigcd,17-(strlen(Scene2ObjectInfo_t->sPedname[i])+1),&lBytesWritten,NULL);
					}
				}
				else
					Scene2ObjectInfo_t->lNumOfPeds++;
			}

			Scene2ObjectInfo_t->lNumOfPeds=lNumSave;
			// now write peds density
			for(i=0;i<Scene2ObjectInfo_t->lNumOfPeds;i++)
			{
				if(!Scene2ObjectInfo_t->bDel[i])
					WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lLocalPedDensity[i],sizeof(Scene2ObjectInfo_t->lLocalPedDensity[i]),&lBytesWritten,NULL);
				else
					Scene2ObjectInfo_t->lNumOfPeds++;
			}

			CloseHandle(hFileOwn);

			// Paste object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// Change sizes

			// 1. SPECIAL_MAIN
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lSpecialMainSize+=(lDatablockSize-lObjectSize);
			else
				lSpecialMainSize+=lDatablockSize;
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);

			// 2. SCENE2_MAIN
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lScene2Size=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,sizeof(lScene2Size),&lBytesWritten,NULL);

			// Append script if one was specified
			if(lScriptSize!=-1)
			{
				AddObjectScript(hFile,Scene2ObjectInfo_t->sName,"buffer_99.tmp",SPECIAL_MAIN);
				DeleteFile("buffer_99.tmp");
			}

			break;

		case SPECIAL_OBJECT_TYPE_DOOR:
			// replace existing object?
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,SPECIAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				// buffer script
				lScriptSize=ExtractObjectScript(hFile,szObjectToReplace,SPECIAL_MAIN,"buffer_99.tmp");

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find SPECIAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find END of SPECIAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}

			// save all upcoming data
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size

			lDatablockSize=6;

			// 1. Name Datablock
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 2. Type Datablock
			lDatablockSize+=0x0A;

			// 3. Data Datablock
			lDatablockSize+=0x4c;

			//--------NOW WRITE DATA

			// 1. Main Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_MAIN,lDatablockSize);

			// 2. Name Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Type Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_TYPE,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 4. Data Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_DATA,0x4c);

//			WriteFile(hFileOwn,(char *)&,sizeof(),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&datatype_door,sizeof(datatype_door),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&bignull,4,&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->bOpen1,sizeof(Scene2ObjectInfo_t->bOpen1),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->bOpen2,sizeof(Scene2ObjectInfo_t->bOpen2),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fMoveAngle,sizeof(Scene2ObjectInfo_t->fMoveAngle),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->bOpen,sizeof(Scene2ObjectInfo_t->bOpen),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->bLocked,sizeof(Scene2ObjectInfo_t->bLocked),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fOpenSpeed,sizeof(Scene2ObjectInfo_t->fOpenSpeed),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fCloseSpeed,sizeof(Scene2ObjectInfo_t->fCloseSpeed),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->sOpenSound,strlen(Scene2ObjectInfo_t->sOpenSound)+1,&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&bigcd,16-(strlen(Scene2ObjectInfo_t->sOpenSound)+1),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->sCloseSound,strlen(Scene2ObjectInfo_t->sCloseSound)+1,&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&bigcd,16-(strlen(Scene2ObjectInfo_t->sCloseSound)+1),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->sLockedSound,strlen(Scene2ObjectInfo_t->sLockedSound)+1,&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&bigcd,16-(strlen(Scene2ObjectInfo_t->sLockedSound)+1),&lBytesWritten,NULL);
			// we append the 1 here cuz all door objects have such a zero appended
//			WriteFile(hFileOwn,(char *)&one,1,&lBytesWritten,NULL);
			// we append the 0 here cuz all door objects have such a zero appended
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->bDoorFlag,sizeof(Scene2ObjectInfo_t->bDoorFlag),&lBytesWritten,NULL);

			CloseHandle(hFileOwn);

			// Paste object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// Change sizes

			// 1. SPECIAL_MAIN
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lSpecialMainSize+=(lDatablockSize-lObjectSize);
			else
				lSpecialMainSize+=lDatablockSize;
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);

			// 2. SCENE2_MAIN
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lScene2Size=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,sizeof(lScene2Size),&lBytesWritten,NULL);

			// Append script if one was specified
			if(lScriptSize!=-1)
			{
				AddObjectScript(hFile,Scene2ObjectInfo_t->sName,"buffer_99.tmp",SPECIAL_MAIN);
				DeleteFile("buffer_99.tmp");
			}
			break;

		case SPECIAL_OBJECT_TYPE_CHARACTER:
		case SPECIAL_OBJECT_TYPE_PLAYER:
			// replace existing object?
			if(szObjectToReplace!=NULL)
			{
				// Find Object to replace
				lDummyOffset=lOffset=FindObject(hFile,szObjectToReplace,SPECIAL_MAIN);
				if(lOffset==-1)
					return(-1);	// stop function if object not found

				// buffer script
				lScriptSize=ExtractObjectScript(hFile,szObjectToReplace,SPECIAL_MAIN,"buffer_99.tmp");

				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find End of Object
				lObjectSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lObjectSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lObjectSize;
			}
			else
			{
				// Find SPECIAL_MAIN
				lDummyOffset=lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);

				// Find END of SPECIAL_MAIN
				lDatablockSize=thisDatablock.GetLong(hFile);

				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT); //set file pointer to end of NORMAL_MAIN
				lDummyOffset+=lDatablockSize;
			}

			// save all upcoming data
//			myHelper.BufferData(hFile,"buffer.tmp",-1);
			myHelper.BufferDataFM(hFile,"buffer.tmp",lDummyOffset,::GetFileSize(hFile,NULL)-lDummyOffset);

			// do we replace?
			if(szObjectToReplace!=NULL)
				// yes so we have to set back the file pointer cuz our object must be overwritten
				SetFilePointer(hFile,-(lObjectSize),0,FILE_CURRENT);

			// create the object file
			hFileOwn=CreateFile("object.tmp",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

			// Calculate main datablock size

			lDatablockSize=6;

			// 1. Name Datablock
			lDatablockSize+=strlen(Scene2ObjectInfo_t->sName)+7;

			// 2. Type Datablock
			lDatablockSize+=0x0A;

			// 3. Data Datablock
			lDatablockSize+=0x4f;

			//--------NOW WRITE DATA

			// 1. Main Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_MAIN,lDatablockSize);

			// 2. Name Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_NAME,strlen(Scene2ObjectInfo_t->sName)+7);
			WriteFile(hFileOwn,Scene2ObjectInfo_t->sName,strlen(Scene2ObjectInfo_t->sName)+1,&lBytesWritten,NULL);

			// 3. Type Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_TYPE,0x0A);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->lType,sizeof(Scene2ObjectInfo_t->lType),&lBytesWritten,NULL);

			// 4. Data Datablock
			thisDatablock.CreateDatablock(hFileOwn,SPECIAL_OBJECT_DATA,0x4f);

//			WriteFile(hFileOwn,(char *)&,sizeof(),&lBytesWritten,NULL);
			WriteFile(hFileOwn,&datatype_character,1,&lBytesWritten,NULL);
			WriteFile(hFileOwn,&Scene2ObjectInfo_t->cBehaviour,sizeof(Scene2ObjectInfo_t->cBehaviour),&lBytesWritten,NULL);
			WriteFile(hFileOwn,&null,3,&lBytesWritten,NULL);
			WriteFile(hFileOwn,&Scene2ObjectInfo_t->cVoice,sizeof(Scene2ObjectInfo_t->cVoice),&lBytesWritten,NULL);
			WriteFile(hFileOwn,&null,3,&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fStrenght,sizeof(Scene2ObjectInfo_t->fStrenght),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fEnergy,sizeof(Scene2ObjectInfo_t->fEnergy),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fEnergy_HANDL,sizeof(Scene2ObjectInfo_t->fEnergy_HANDL),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fEnergy_HANDR,sizeof(Scene2ObjectInfo_t->fEnergy_HANDR),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fEnergy_LEGL,sizeof(Scene2ObjectInfo_t->fEnergy_LEGL),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fEnergy_LEGR,sizeof(Scene2ObjectInfo_t->fEnergy_LEGR),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fReactions,sizeof(Scene2ObjectInfo_t->fReactions),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSpeed,sizeof(Scene2ObjectInfo_t->fSpeed),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fAggr,sizeof(Scene2ObjectInfo_t->fAggr),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fInt,sizeof(Scene2ObjectInfo_t->fInt),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fShooting,sizeof(Scene2ObjectInfo_t->fShooting),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fSight,sizeof(Scene2ObjectInfo_t->fSight),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fHearing,sizeof(Scene2ObjectInfo_t->fHearing),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fDriving,sizeof(Scene2ObjectInfo_t->fDriving),&lBytesWritten,NULL);
			WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fMass,sizeof(Scene2ObjectInfo_t->fMass),&lBytesWritten,NULL);
//			if(Scene2ObjectInfo_t->lType==SPECIAL_OBJECT_TYPE_CHARACTER)
				WriteFile(hFileOwn,(char *)&Scene2ObjectInfo_t->fBehaviour,sizeof(Scene2ObjectInfo_t->fBehaviour),&lBytesWritten,NULL);

			CloseHandle(hFileOwn);

			// Paste object
			myHelper.PasteBufferedDataFM(hFile,"object.tmp",true);

			// Paste other data
			myHelper.PasteBufferedDataFM(hFile,"buffer.tmp",true);

			SetEndOfFile(hFile);

			// Change sizes

			// 1. SPECIAL_MAIN
			if(szObjectToReplace!=NULL)
				lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			lSpecialMainSize=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lSpecialMainSize+=(lDatablockSize-lObjectSize);
			else
				lSpecialMainSize+=lDatablockSize;
			SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lSpecialMainSize,sizeof(lSpecialMainSize),&lBytesWritten,NULL);

			// 2. SCENE2_MAIN
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			lScene2Size=thisDatablock.GetLong(hFile);
			if(szObjectToReplace!=NULL)
				lScene2Size+=(lDatablockSize-lObjectSize);
			else
				lScene2Size+=lDatablockSize;
			SetFilePointer(hFile,2,0,FILE_BEGIN);
			WriteFile(hFile,(char *)&lScene2Size,sizeof(lScene2Size),&lBytesWritten,NULL);

			// Append script if one was specified
			if(lScriptSize!=-1)
			{
				AddObjectScript(hFile,Scene2ObjectInfo_t->sName,"buffer_99.tmp",SPECIAL_MAIN);
				DeleteFile("buffer_99.tmp");
			}
			break;
	}
	return(1);
}
// Protects a scene2.bin file from being opened/edited by other editors and unauthorized DC||ED editors.
int CScene2::Protect(HANDLE hFile,DWORD dwFlags,const unsigned long lProtBlockSize)
{
	CDatablock thisDatablock;
	CHelper myHelper;

	HANDLE hProtFile1;

	long lNormalMainOff=0,		// offset of normal main
		lSpecialMainOff=0,		// offset of special main
		lAdditionalMainOff=0,	// offset of additional main
		lCurOff=0,				// current offset (e.g. for BufferDataFM)
		lSize=0,				// size for different things
		lNormMainEntries=0,		// number of protection blocks in NORMAL_MAIN
		lSpecMainEntries=0,		// number of protection blocks in SPECIAL_MAIN
		lAddMainEntries=0,		// number of protection blocks in ADDITIONAL_MAIN
		lNeutralEntries=0;		// number of protection blocks which have NOT been added to a main datablock

//	BasicObjectInfo_t Scene2BasicObjectInfo_t;	// getnextobject

	unsigned long lDummy=0,											// the dummy for different stuff we need to remember ;)
		lBytesWritten=0;

	int i, RndNum;

	short nType;				// type of datablock just read

	// init random number generator
	srand(time(NULL));

	// check if lProtBlockSize-6 is a multiple of 4
	if(((lProtBlockSize-6)%4)!=0)
		// no, it isn't
		return(-1);

	// now we begin with our protection

	// duplicate type datablock
	if((dwFlags&PROTECT_ADD)==PROTECT_ADD)
	{
		lSpecialMainOff=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
		SetFilePointer(hFile,lSpecialMainOffset+6,0,FILE_BEGIN);
		lCurOff=lSpecialMainOffset+6;
		nType=thisDatablock.GetShort(hFile);
		while(nType==SPECIAL_OBJECT_MAIN)
		{
			lSize=thisDatablock.GetLong(hFile);

			// update the object size
			lSize+=10;
			SetFilePointer(hFile,-4,0,FILE_CURRENT);
			WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);

			// write block
			lCurOff+=6;
			myHelper.BufferDataFM(hFile,"protbuf3.tmp",lCurOff,::GetFileSize(hFile,NULL)-lCurOff);
			thisDatablock.CreateProtectionDatablock(hFile,SPECIAL_OBJECT_TYPE,10);
			myHelper.PasteBufferedDataFM(hFile,"protbuf3.tmp",true);
			lCurOff+=(lSize-6);
			SetFilePointer(hFile,lCurOff,0,FILE_BEGIN);
			nType=thisDatablock.GetShort(hFile);
			lSpecMainEntries++;
		}

		// setup sizes
		// 1. SPECIAL_MAIN
		SetFilePointer(hFile,lSpecialMainOff+2,0,FILE_BEGIN);
		lSize=thisDatablock.GetLong(hFile);
		lSize+=(lSpecMainEntries*10);
		SetFilePointer(hFile,lSpecialMainOff+2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);

		// 2. SCENE2_MAIN
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		lSize=thisDatablock.GetLong(hFile);
		lSize+=(lSpecMainEntries*10);
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);
	}

	if((dwFlags&PROTECT_GARBAGE)==PROTECT_GARBAGE)
	{
		lSpecMainEntries=0;
		// 1. we add protection blocks before every scene2.bin property datablock
		SetFilePointer(hFile,6,0,FILE_BEGIN);
		lCurOff=6;
		nType=thisDatablock.GetShort(hFile);

		while((nType!=NORMAL_MAIN)&&(nType!=SPECIAL_MAIN)&&(nType!=ADDITIONAL_MAIN)&&(nType!=SCENE2_EOF)&&(nType!=NULL))
		{
			lSize=thisDatablock.GetLong(hFile);
			SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
			lCurOff+=lSize;
			// buffer data & insert protection block
			myHelper.BufferDataFM(hFile,"protbuffer.tmp",lCurOff,::GetFileSize(hFile,NULL)-lCurOff);
			// random number in order to decide what block we add
			RndNum=random(0,(PROTBLOCKNUM-1));
			thisDatablock.CreateProtectionDatablock(hFile,nProtBlocks[RndNum],lProtBlockSize);
			lCurOff+=lProtBlockSize;
			// readd buffered data
			myHelper.PasteBufferedDataFM(hFile,"protbuffer.tmp",true);
			//set file pointer back
			SetFilePointer(hFile,lCurOff,0,FILE_BEGIN);
			lNeutralEntries++;
			nType=thisDatablock.GetShort(hFile);
		}

		// we should have reached NORMAL_MAIN now
		SetFilePointer(hFile,4,0,FILE_CURRENT);
		lCurOff+=6;

		// we insert a protection block directly after this block
		myHelper.BufferDataFM(hFile,"protbuffer.tmp",lCurOff,::GetFileSize(hFile,NULL)-lCurOff);
		RndNum=random(0,(PROTBLOCKNUM-1));
		thisDatablock.CreateProtectionDatablock(hFile,nProtBlocks[RndNum],lProtBlockSize);
		lCurOff+=lProtBlockSize;
		myHelper.PasteBufferedDataFM(hFile,"protbuffer.tmp",true);
		SetFilePointer(hFile,lCurOff,0,FILE_BEGIN);
		lNormMainEntries++;

		nType=thisDatablock.GetShort(hFile);
		while((nType!=NORMAL_MAIN)&&(nType!=SPECIAL_MAIN)&&(nType!=ADDITIONAL_MAIN)&&(nType!=SCENE2_EOF)&&(nType!=NULL))
		{
			lSize=thisDatablock.GetLong(hFile);
			SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
			lCurOff+=lSize;
			// buffer data & insert protection block
			myHelper.BufferDataFM(hFile,"protbuffer.tmp",lCurOff,::GetFileSize(hFile,NULL)-lCurOff);
			// random number in order to decide what block we add
			RndNum=random(0,(PROTBLOCKNUM-1));
			thisDatablock.CreateProtectionDatablock(hFile,nProtBlocks[RndNum],lProtBlockSize);
			lCurOff+=lProtBlockSize;
			// readd buffered data
			myHelper.PasteBufferedDataFM(hFile,"protbuffer.tmp",true);
			//set file pointer back
			SetFilePointer(hFile,lCurOff,0,FILE_BEGIN);
			lNormMainEntries++;
			nType=thisDatablock.GetShort(hFile);
		}

		// we should have reached SPECIAL_MAIN now
		SetFilePointer(hFile,4,0,FILE_CURRENT);
		lCurOff+=6;

		// we insert a protection block directly after this block
		myHelper.BufferDataFM(hFile,"protbuffer.tmp",lCurOff,::GetFileSize(hFile,NULL)-lCurOff);
		RndNum=random(0,(PROTBLOCKNUM-1));
		thisDatablock.CreateProtectionDatablock(hFile,nProtBlocks[RndNum],lProtBlockSize);
		lCurOff+=lProtBlockSize;
		myHelper.PasteBufferedDataFM(hFile,"protbuffer.tmp",true);
		SetFilePointer(hFile,lCurOff,0,FILE_BEGIN);
		lSpecMainEntries++;

		nType=thisDatablock.GetShort(hFile);
		while((nType!=NORMAL_MAIN)&&(nType!=SPECIAL_MAIN)&&(nType!=ADDITIONAL_MAIN)&&(nType!=SCENE2_EOF)&&(nType!=NULL))
		{
			lSize=thisDatablock.GetLong(hFile);
			SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
			lCurOff+=lSize;
			// buffer data & insert protection block
			myHelper.BufferDataFM(hFile,"protbuffer.tmp",lCurOff,::GetFileSize(hFile,NULL)-lCurOff);
			// random number in order to decide what block we add
			RndNum=random(0,(PROTBLOCKNUM-1));
			thisDatablock.CreateProtectionDatablock(hFile,nProtBlocks[RndNum],lProtBlockSize);
			lCurOff+=lProtBlockSize;
			// readd buffered data
			myHelper.PasteBufferedDataFM(hFile,"protbuffer.tmp",true);
			//set file pointer back
			SetFilePointer(hFile,lCurOff,0,FILE_BEGIN);
			lSpecMainEntries++;
			nType=thisDatablock.GetShort(hFile);
		}

		// we should have reached ADDITIONAL_MAIN now
		SetFilePointer(hFile,4,0,FILE_CURRENT);
		lCurOff+=6;

		// we insert a protection block directly after this block
		myHelper.BufferDataFM(hFile,"protbuffer.tmp",lCurOff,::GetFileSize(hFile,NULL)-lCurOff);
		RndNum=random(0,(PROTBLOCKNUM-1));
		thisDatablock.CreateProtectionDatablock(hFile,nProtBlocks[RndNum],lProtBlockSize);
		lCurOff+=lProtBlockSize;
		myHelper.PasteBufferedDataFM(hFile,"protbuffer.tmp",true);
		SetFilePointer(hFile,lCurOff,0,FILE_BEGIN);
		lAddMainEntries++;
	
		nType=thisDatablock.GetShort(hFile);
		while((nType!=NORMAL_MAIN)&&(nType!=SPECIAL_MAIN)&&(nType!=ADDITIONAL_MAIN)&&(nType!=SCENE2_EOF)&&(nType!=NULL))
		{
			lSize=thisDatablock.GetLong(hFile);
			SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
			lCurOff+=lSize;
			// buffer data & insert protection block
			myHelper.BufferDataFM(hFile,"protbuffer.tmp",lCurOff,::GetFileSize(hFile,NULL)-lCurOff);
			// random number in order to decide what block we add
			RndNum=random(0,(PROTBLOCKNUM-1));
			thisDatablock.CreateProtectionDatablock(hFile,nProtBlocks[RndNum],lProtBlockSize);
			lCurOff+=lProtBlockSize;
			// readd buffered data
			myHelper.PasteBufferedDataFM(hFile,"protbuffer.tmp",true);
			//set file pointer back
			SetFilePointer(hFile,lCurOff,0,FILE_BEGIN);
			lAddMainEntries++;
			nType=thisDatablock.GetShort(hFile);
		}

		// now we have to change the size of each main datablock
		// 1. NORMAL_MAIN
		lNormalMainOff=thisDatablock.FindDatablock(hFile,NORMAL_MAIN);
		SetFilePointer(hFile,(lNormalMainOff+2),0,FILE_BEGIN);
		lSize=thisDatablock.GetLong(hFile);
		lSize+=(lNormMainEntries*lProtBlockSize);
		SetFilePointer(hFile,(lNormalMainOff+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);

		// 2. SPECIAL_MAIN
		lSpecialMainOff=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
		SetFilePointer(hFile,(lSpecialMainOff+2),0,FILE_BEGIN);
		lSize=thisDatablock.GetLong(hFile);
		lSize+=(lSpecMainEntries*lProtBlockSize);
		SetFilePointer(hFile,(lSpecialMainOff+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);

		// 3. ADDITIONAL_MAIN
		lAdditionalMainOff=thisDatablock.FindDatablock(hFile,ADDITIONAL_MAIN);
		SetFilePointer(hFile,(lAdditionalMainOff+2),0,FILE_BEGIN);
		lSize=thisDatablock.GetLong(hFile);
		lSize+=(lAddMainEntries*lProtBlockSize);
		SetFilePointer(hFile,(lAdditionalMainOff+2),0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);

		// 4. SCENE2_MAIN
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		lSize=thisDatablock.GetLong(hFile);
		lSize+=((lNeutralEntries*lProtBlockSize)+(lNormMainEntries*lProtBlockSize)+(lSpecMainEntries*lProtBlockSize)+(lAddMainEntries*lProtBlockSize));
		SetFilePointer(hFile,2,0,FILE_BEGIN);
		WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);
	}
	return 0;
}

// Checks if a file is protected.
bool CScene2::IsProtected(HANDLE hFile)
{
	unsigned long lSize=0, lOffset=0;
	short nType=0;
	int i;
	bool beenthere=false;
	CDatablock thisDatablock;

	// 1 step: check for garbage datablocks
	SetFilePointer(hFile,6,0,FILE_BEGIN);
	while(1)
	{
		nType=thisDatablock.GetShort(hFile);
		// did we reach the end?
		if(nType==SCENE2_EOF)
			break;
		// check protection datablock table
		for(i=0;i<PROTBLOCKNUM;i++)
			if(nType==nProtBlocks[i])
				return true;

		lSize=thisDatablock.GetLong(hFile);
		if(lSize==SCENE2_EOF)
			break;

		switch(nType)
		{
			case NORMAL_MAIN:
			case SPECIAL_MAIN:
			case ADDITIONAL_MAIN:
			case UNKNOWN_DATABLOCK_MAIN:
				break;

			default:
				SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);

		}
	}

	// 2 step: look for duplicated type-datablocks in SPECIAL_MAIN
	lOffset=thisDatablock.FindDatablock(hFile,SPECIAL_MAIN);
	SetFilePointer(hFile,(lOffset+6),0,FILE_BEGIN);

	while(1)
	{
		nType=thisDatablock.GetShort(hFile);
		// did we reach the end?
		if(nType==SCENE2_EOF)
			return false;

		lSize=thisDatablock.GetLong(hFile);
		if(lSize==SCENE2_EOF || lSize==0)	// lSize==0 is important, otherwise an infinite loop might result
			return false;

		switch(nType)
		{
			case SPECIAL_OBJECT_MAIN:
				break;
			case SPECIAL_OBJECT_NAME:
				SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
			case SPECIAL_OBJECT_TYPE:
				// has this already been triggered?
				if(beenthere)
					return true;
				beenthere=true;
				SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
				break;
			case SPECIAL_OBJECT_DATA:
				beenthere=false;
				SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
				break;

			default:
				SetFilePointer(hFile,(lSize-6),0,FILE_CURRENT);
				break;
		}
	}
	return false;
}

// Deletes actors, cars & scripts from a scene2.bin file so you can start editing immediately
int CScene2::NewTemplate(HANDLE hFile)
{
	BasicObjectInfo_t GeneralObjectInfo_t;
	Filter_t BasicFilter_t;
	CDatablock thisDatablock;
	unsigned long lResult, lSize, lOffset;

	SetFilePointer(hFile,6,0,FILE_BEGIN);
	GeneralObjectInfo_t.lNextObjectOffset=6;

	// delete all actors
	BasicFilter_t.iFilterType=FILTER_OBJECTTYPE;
	BasicFilter_t.iObjectType=SPECIAL_OBJECT_TYPE_CHARACTER;
	BasicFilter_t.nDatablock=SPECIAL_MAIN;

	lResult=GetNextObject(hFile,&GeneralObjectInfo_t,&BasicFilter_t);
	while(lResult!=-1)
	{
		if(lResult==1)
		{
			lSize=0;
			// get object sizes to calculate delta offset
			lOffset=FindObject(hFile,GeneralObjectInfo_t.name,SPECIAL_MAIN);
			// for safety ;)
			if(lOffset!=-1)
			{
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
				lSize=thisDatablock.GetLong(hFile);
			}

			lOffset=FindObject(hFile,GeneralObjectInfo_t.name,NORMAL_MAIN);
			if(lOffset!=-1)
			{
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
				lSize+=thisDatablock.GetLong(hFile);
			}

			GeneralObjectInfo_t.lNextObjectOffset-=lSize;

			DeleteObject(hFile,GeneralObjectInfo_t.name,SPECIAL_MAIN);
			DeleteObject(hFile,GeneralObjectInfo_t.name,NORMAL_MAIN);

			// fix filepointer
			SetFilePointer(hFile,GeneralObjectInfo_t.lNextObjectOffset,NULL,FILE_BEGIN);
		}
		lResult=GetNextObject(hFile,&GeneralObjectInfo_t,&BasicFilter_t);
	}

	SetFilePointer(hFile,6,0,FILE_BEGIN);
	GeneralObjectInfo_t.lNextObjectOffset=6;

	// delete all cars
	BasicFilter_t.iFilterType=FILTER_OBJECTTYPE;
	BasicFilter_t.iObjectType=SPECIAL_OBJECT_TYPE_CAR;
	BasicFilter_t.nDatablock=SPECIAL_MAIN;

	lResult=GetNextObject(hFile,&GeneralObjectInfo_t,&BasicFilter_t);
	while(lResult!=-1)
	{
		if(lResult==1)
		{
			lSize=0;
			// get object sizes to calculate delta offset
			lOffset=FindObject(hFile,GeneralObjectInfo_t.name,SPECIAL_MAIN);
			// for safety ;)
			if(lOffset!=-1)
			{
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
				lSize=thisDatablock.GetLong(hFile);
			}

			lOffset=FindObject(hFile,GeneralObjectInfo_t.name,NORMAL_MAIN);
			if(lOffset!=-1)
			{
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
				lSize+=thisDatablock.GetLong(hFile);
			}

			GeneralObjectInfo_t.lNextObjectOffset-=lSize;

			DeleteObject(hFile,GeneralObjectInfo_t.name,SPECIAL_MAIN);
			DeleteObject(hFile,GeneralObjectInfo_t.name,NORMAL_MAIN);

			// fix filepointer
			SetFilePointer(hFile,GeneralObjectInfo_t.lNextObjectOffset,NULL,FILE_BEGIN);
		}
		lResult=GetNextObject(hFile,&GeneralObjectInfo_t,&BasicFilter_t);
	}

	SetFilePointer(hFile,6,0,FILE_BEGIN);
	GeneralObjectInfo_t.lNextObjectOffset=6;

	// delete all scripts
	BasicFilter_t.iFilterType=FILTER_OBJECTTYPE;
	BasicFilter_t.iObjectType=SPECIAL_OBJECT_TYPE_SCRIPT;
	BasicFilter_t.nDatablock=SPECIAL_MAIN;

	lResult=GetNextObject(hFile,&GeneralObjectInfo_t,&BasicFilter_t);
	while(lResult!=-1)
	{
		if(lResult==1)
		{
			lSize=0;
			// get object sizes to calculate delta offset
			lOffset=FindObject(hFile,GeneralObjectInfo_t.name,SPECIAL_MAIN);
			// for safety ;)
			if(lOffset!=-1)
			{
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
				lSize=thisDatablock.GetLong(hFile);
			}

			lOffset=FindObject(hFile,GeneralObjectInfo_t.name,NORMAL_MAIN);
			if(lOffset!=-1)
			{
				SetFilePointer(hFile,(lOffset+2),0,FILE_BEGIN);
				lSize+=thisDatablock.GetLong(hFile);
			}

			GeneralObjectInfo_t.lNextObjectOffset-=lSize;

			DeleteObject(hFile,GeneralObjectInfo_t.name,SPECIAL_MAIN);
			DeleteObject(hFile,GeneralObjectInfo_t.name,NORMAL_MAIN);

			// fix filepointer
			SetFilePointer(hFile,GeneralObjectInfo_t.lNextObjectOffset,NULL,FILE_BEGIN);
		}
		lResult=GetNextObject(hFile,&GeneralObjectInfo_t,&BasicFilter_t);
	}

	return 0;
}
