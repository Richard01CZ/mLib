// Helper.cpp
//
// CHelper class definition file
//
// CHelper class is a part of the MafiaLib Engine
//
// Copyright (c) 2003, 2004 ASM
//
// Notice: Functions have been made static due to request of scream.

#include <windows.h>

#pragma once

// definitions
#define TYPE_BYTE	1
#define TYPE_WORD	2

#define MASK 0x0F

#define MAFIALIB_PI 3.14159265358979323846

#pragma once

class CHelper
{
public:
	CHelper(void);
	~CHelper(void);
	static unsigned long Char2Long(BYTE *ptr,int size);
	static unsigned long Char2Long(BYTE *ptr);
	static unsigned long Char2LongE(BYTE * ptr, int size);
	static unsigned long Char2LongE(BYTE * ptr);

	// Reverses bit order in a long value -- useful for converting big to little endian and vice versa
	static long ReverseBits(unsigned long value, int type, int times);

	static void CHelper::Long2String(char *out, long number, long size);
	// Used for reading data and buffering it in a temporary file
	static long BufferData(HANDLE hFile, char * szFilename, long lSize);
	// Reads out buffered data and puts it into the currently opened file
	static long PasteBufferedData(HANDLE hFile, char * szFilename, bool bDel);
	// Used for determining the lenght of a string
	static long StrLen(char * str);
	// Compares lSize chars of two strings
	static int Compare(char * szString1, char * szString2, long lSize);
	// Returns the size of file szFileName
	static long GetFileSize(char * szFileName);
	// Buffers data from the current pos of hFile, uses File Mapping
	static long BufferDataFM(HANDLE hFile, char * szFileName, long lOffset, long lSize);
	// Pastes all data of szFileName to current pos of hFile, uses File Mapping
	static long PasteBufferedDataFM(HANDLE hFile, char * szFileName, bool bDel);
	// Looks for a sequence of bytes in a buffer (binary)
	static int binstrstr(char * buffer, char * tofind, int tofindsize, int size);
	// Computes the Z-angle from the first rotation value of Mafia objects
	static float ZRot2Angle(float fZRot);
	// Computes the X or Y angle from the second or fourth rotation value of Mafia objects
	static float XYRot2Angle(float fXYRot);
	// Computes the rotation value for the first rotation parameter
	static float Angle2ZRotCos(float fAngle);
	// Computes the rotation value for the third rotation parameter
	static float Angle2ZRotSin(float fAngle);
	// Computes the rotation value for the second or fourth rotation parameter
	static float Angle2XYRot(float fAngle);
	// Used for reading until a special character is reached
	static int ReadTillChar(HANDLE hFile, char * Buffer, char cEndChar);
	// Returns the lenght of a sequence of bytes by specifying the end character
	static int BinSeqLen(char * ptr, char EndChar);
	// Used for dumping contents of a file into another file
	static int Dump(HANDLE hFile, char * szFileName, unsigned long lOffset, unsigned long lSize);
	// Converts a given move angle for doors to the corresponding float value
	static float DoorAngleToVal(float fRealAngle);
	// Converts a given door value into the move angle
	static float DoorValToAngle(float fMoveAngle);
	// Searches for a given binary string in a buffer, if you want by using a search pattern
	static unsigned long SearchPattern(char * ptr, const char * SearchBuffer, char cPlaceholder, int nPtrLenght, int nSearchLenght);
};
