// Datablock.cpp
//
// CDatablock Implementation file
//
// CDatablock class is a part of the MafiaLib Engine
//
// Copyright (c) 2003, 2004 ASM

#include "datablock.h"
#include <time.h>

CDatablock::CDatablock(void)
{
}

CDatablock::~CDatablock(void)
{
}


// Returns the size of a datablock which begins at lOffset
long CDatablock::GetLong(HANDLE hFile)
{
	long lLong=0;
	unsigned long lBytesRead=0;
	ReadFile(hFile,(char *)&lLong,sizeof(lLong),&lBytesRead,NULL);
	if(lBytesRead==0)
		return(SCENE2_EOF);
	return(lLong);
}

// Returns the type of the datablock
short CDatablock::GetShort(HANDLE hFile)
{
	short nShort=0;
	unsigned long lBytesRead=0;
	ReadFile(hFile,(char *)&nShort,sizeof(nShort),&lBytesRead,NULL);
	if(lBytesRead==0)
		return(SCENE2_EOF);
	return(nShort);
}

// Creates a new datablock and puts it directly into a file, overwrites any existing data
void CDatablock::CreateDatablock(HANDLE hFile, short nType, long lSize)
{
	unsigned long lBytesWritten=0;

	// Write Type
	WriteFile(hFile,(char *)&nType,sizeof(nType),&lBytesWritten,NULL);

	// Write Size
	WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);

	return;
}

// Looks for a particular datablock in Scene2.bin file and, if found, returns the offset of the biginning of the datablock
unsigned long CDatablock::FindDatablock(HANDLE hFile,short nType)
{
	unsigned long lOffset=6, lDatablockSize;

	SetFilePointer(hFile,6,0,FILE_BEGIN);		//File pointer to 6th byte after file start
	while(1)
	{
		if(GetShort(hFile)==nType)
			break;

		SetFilePointer(hFile,-2,0,FILE_CURRENT);

		if(GetShort(hFile)==SCENE2_EOF)
			return(SCENE2_EOF);

		lDatablockSize=GetLong(hFile);
		SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT);
		lOffset+=lDatablockSize;
	}
	return(lOffset);
}

// Used for retrieving floating point values
float CDatablock::GetFloat(HANDLE hFile)
{
	float fFloat=0;
	unsigned long lBytesRead=0;
	ReadFile(hFile,(char *)&fFloat,sizeof(fFloat),&lBytesRead,NULL);
	if(lBytesRead==0)
		return(SCENE2_EOF);
	return(fFloat);
}

/*// Creates a new datablock and puts it directly into a file, moves any existing data/existing data is not destroyed
void CDatablock::CreateDatablock(HANDLE hFile, short nType, long lSize)
{
	unsigned long lBytesWritten=0, lBytesRead=0;
	char cDefault[256];
	char dummy[256];
	int i=0;

	// fill cDefault
	for(i=0;i<(lSize-7);i++)
		cDefault[i]=0x30;

	cDefault[i+1]=0x00;

	// Go back to beginning of datablock
	SetFilePointer(hFile,-(lBytesMoved),0,FILE_CURRENT);

	// Write Type
	WriteFile(hFile,(char *)&nType,sizeof(nType),&lBytesWritten,NULL);

	// Write Size
	WriteFile(hFile,(char *)&lSize,sizeof(lSize),&lBytesWritten,NULL);

	// Fill rest with 0x01 (to prevent WriteFile from doing a Null-Write-Operation
	WriteFile(hFile,cDefault,strlen(cDefault),&lBytesWritten,NULL);

	// Put our old data
	SetFilePointer(hFileOwn,0,0,FILE_BEGIN);

	for(i=0;i<lBytesMoved;i+=4)
	{
		ReadFile(hFileOwn,dummy,1,&lBytesRead,NULL);
		WriteFile(hFile,dummy,1,&lBytesWritten,NULL);
	}

	// move file pointer back to start of datablock
	SetFilePointer(hFile,-(lSize+lBytesMoved),0,FILE_CURRENT);
}
*/
// Finds subdatablocks like 22AE, 23AE, etc.....
long CDatablock::FindSubDatablock(HANDLE hFile, short nType, long lBeginOffset)
{
	unsigned long lOffset=lBeginOffset, lDatablockSize;
	short nCurSel=0;

	SetFilePointer(hFile,lBeginOffset,0,FILE_BEGIN);

	while(1)
	{
		nCurSel=GetShort(hFile);
		if(nCurSel==nType)
			return(lOffset);

		if(nType==SCENE2_EOF)
			return(SCENE2_EOF);

		lDatablockSize=GetLong(hFile);
		SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT);
		lOffset+=lDatablockSize;
	}
	return(SCENE2_EOF);
}

// Reads a byte
char CDatablock::GetByte(HANDLE hFile)
{
	char cByte=0;
	unsigned long lBytesRead=0;
	ReadFile(hFile,&cByte,sizeof(cByte),&lBytesRead,NULL);
	if(lBytesRead==0)
		return(SCENE2_EOF);
	return(cByte);
}

// Looks for datablocks belonging to objects
long CDatablock::FindObjectDatablock(HANDLE hFile, short nType, long lBeginOffset)
{
	unsigned long lOffset=lBeginOffset, lDatablockSize;
	short nCurType;

	SetFilePointer(hFile,lBeginOffset,0,FILE_BEGIN);

	while(1)
	{
		nCurType=GetShort(hFile);
		if(nCurType==nType)
			return(lOffset);

		switch(nCurType)
		{
			case SCENE2_EOF:
				return(nType);

			case NORMAL_OBJECT_MAIN:
			case SPECIAL_OBJECT_MAIN:
			case ADDITIONAL_OBJECT:
			case NORMAL_MAIN:
			case SPECIAL_MAIN:
			case ADDITIONAL_MAIN:
			case UNKNOWN_DATABLOCK_MAIN:
				return(SCENE2_EOF);

			default:
				lDatablockSize=GetLong(hFile);
				SetFilePointer(hFile,(lDatablockSize-6),0,FILE_CURRENT);
				lOffset+=lDatablockSize;
				break;
		}
	}
	return(SCENE2_EOF);
}
// Creates a datablock in file specified by hFile and fills it with random data
int CDatablock::CreateProtectionDatablock(HANDLE hFile, short nProtDatablockType, unsigned long lSize)
{
	int iRndData=0,
		iDummy=0,
		i=0;

	unsigned long lBytesWritten=0;

	// init random number generator
	srand(time(NULL));

	CreateDatablock(hFile,nProtDatablockType,lSize);

	// random data
	iDummy=(lSize-6)/4;
	for(i=0;i<iDummy;i++)
	{
		iRndData=rand();
		WriteFile(hFile,(char *)&iRndData,sizeof(iRndData),&lBytesWritten,NULL);
	}

	return 0;
}
