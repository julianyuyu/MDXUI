// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>

//#include "XMWrapper.h"
//using namespace DirectX;
//using namespace WrappedXM;

inline bool MByteToWChar(LPCSTR str, LPWSTR wstr, UINT len)
{
	// Get the required size of the buffer that receives the Unicode string.
	UINT dstlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	if (len < dstlen)
	{
		return false;
	}

	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, dstlen);
	return true;
}

#define	PI_180	0.0174532925f	/* pi/180 */

#define SKIP_CHAR(str, ch)	do {while(*(str) == (ch)) {(str)++;}} while(0)
#define SKIP_SPACE(str)		SKIP_CHAR(str, ' ')
#define SKIP_INDICATOR(str)	SKIP_CHAR(str, ':')
#define SKIPTO_DATA(str)	do {SKIP_SPACE(str);SKIP_INDICATOR(str);SKIP_SPACE(str);}while(0)

#define STRMATCH_N(str1, str2, n) (!_strnicmp((str1), (str2), (n)))
#define STRMATCH(str1, str2) (!_stricmp((str1), (str2)))

#define SAFEFREE(ptr)		do {if(ptr) { free(ptr); (ptr) = NULL; }} while(0)
#define SAFEDELETE(ptr)		do {if(ptr) { delete (ptr); (ptr) = NULL; }} while(0)
#define SAFEDELETEARRAY(ptr) do {if(ptr) { delete [] (ptr); (ptr) = NULL; }} while(0)
#define SAFERELEASE(ptr)	do {if(ptr) {(ptr)->Release(); (ptr) = NULL; }} while(0)
#define DESTROYONFAILED(hr) do { if (FAILED(hr)) {Destroy(); return false;} } while(0)
#define RETURNONFAILED(hr)	do { if (FAILED(hr)) { return false;} } while(0)
#define RETURNONNULL(ptr)	do { if (!(ptr)) { return false;} } while(0)

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

template <typename T> void SafeReleaseList(std::vector<T> & list)
{
	std::vector<T>::iterator it = list.begin();
	for (; it != list.end(); ++it)
	{
		SAFERELEASE(*it);
		//SafeRelease(it);
	}
	list.clear();
}