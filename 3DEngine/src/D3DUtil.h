#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <comdef.h>
#include <string>

#ifndef GFX_EXC
#define GFX_EXC(x)		\
{						\
	HRESULT hr__ = (x);	\
	std::wstring wfn = AnsiToWString(__FILE__);	\
	if(FAILED(hr__)){throw D3DException(hr__, L#x, wfn, __LINE__);}	\
}
#endif

class D3DException
{
public:
	D3DException() = default;
	D3DException(
		HRESULT hr,
		const std::wstring& functionName,
		const std::wstring& fileName,
		int lineNumber)
		:
		errorCode(hr),
		functionName(functionName),
		fileName(fileName),
		lineNumber(lineNumber)
	{}

	std::wstring ToWstring() const
	{
		const _com_error err(errorCode);
		const std::wstring msg = err.ErrorMessage();

		return functionName + L"Failed in " + fileName + L" -- Line " + std::to_wstring(lineNumber) + L" -- Error: " + msg;
	}

public:
	HRESULT errorCode = S_OK;
	std::wstring functionName;
	std::wstring fileName;
	int lineNumber;
};

inline std::wstring AnsiToWString(const std::string& str)
{
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}