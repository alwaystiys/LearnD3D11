#include<learnd3d11/systemclass.h>
#include<learnd3d11/renderer.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	Renderer* myRenderer;
	bool result;

	System = new SystemClass;
	myRenderer = new Renderer;

	result = System->Initialize(myRenderer);
	if (result) {
		System->Run();
	}

	System->ShutDown();
	delete System;
	System = 0;

	return 0;
}