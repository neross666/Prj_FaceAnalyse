#include <locale.h>
#include "CharSet.h"

using namespace std;

std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";  

	setlocale(LC_ALL, "chs");

	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	std::string result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

std::wstring s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs");

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, "C");

	return result;
}

//string ws2s(wstring& inputws){ return WChar2Ansi(inputws.c_str()); }
std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) 
		return std::string("");

	char* pszDst = new char[nLen];

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

//std::wstring s2ws(const string& s){ return Ansi2WChar(s.c_str(),s.size());} 
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if (nSize <= 0) 
		return NULL;

	WCHAR *pwszDst = new WCHAR[nSize + 1];

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;

	if (pwszDst[0] == 0xFEFF) // skip Oxfeff
	for (int i = 0; i < nSize; i++)
		pwszDst[i] = pwszDst[i + 1];

	wstring wcharString(pwszDst);
	delete pwszDst;

	return wcharString;
}




wchar_t * MBs2WCs(const char* pszSrc)
{
	wchar_t* pwcs = NULL;
	int size = 0;
#if defined(_linux_)
	setlocale(LC_ALL, "zh_CN.GB2312");
	size = mbstowcs(NULL, pszSrc, 0);
	pwcs = new wchar_t[size + 1];
	size = mbstowcs(pwcs, pszSrc, size + 1);
	pwcs[size] = 0;
#else
	size = MultiByteToWideChar(20936, 0, pszSrc, -1, 0, 0);
	if (size <= 0)
		return NULL;
	pwcs = new wchar_t[size];
	MultiByteToWideChar(20936, 0, pszSrc, -1, pwcs, size);
#endif 
return pwcs;
}


char* WCs2MBs(const wchar_t * wcharStr)
{
	char* str = NULL;
	int size = 0;
#if defined(_linux_)
	setlocale(LC_ALL, "zh_CN.UTF8");
	size = wcstombs(NULL, wcharStr, 0);
	str = new char[size + 1];
	wcstombs(str, wcharStr, size);
	str[size] = '\0';
#else
	size = WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, NULL, NULL, NULL, NULL);
	str = new char[size];
	WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, str, size, NULL, NULL);
#endif 
return str;
 }