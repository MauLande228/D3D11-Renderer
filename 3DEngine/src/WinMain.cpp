#include "EngineApp.h"
#include "D3DUtil.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Engine::EngineApp engine{};
	
	try
	{
		engine.Run();
	}
	catch (D3DException& e)
	{
		MessageBox(nullptr, e.ToWstring().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}