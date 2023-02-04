#pragma once
#include<DirectXMath.h>
#include<wrl.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include"Framework.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class MyGame : public Framework
{
public:
	void TitleInitialize();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
	bool t = false, y = false;
};
