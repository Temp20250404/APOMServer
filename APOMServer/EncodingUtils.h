
#pragma once
#include <string>

// UTF-8 �� UTF-16 (std::wstring)
std::wstring Utf8ToWString(const std::string& utf8Str);

// UTF-16 (std::wstring) �� UTF-8
std::string WStringToUtf8(const std::wstring& wideStr);
