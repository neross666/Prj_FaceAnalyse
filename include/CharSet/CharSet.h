#ifndef CHARSET_INCLUDE
#define CHARSET_INCLUDE

#include <string>  
#include <windows.h>

std::string ws2s(const std::wstring& ws);

std::wstring s2ws(const std::string& s);	

std::string WChar2Ansi(LPCWSTR pwszSrc);

std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);



#endif