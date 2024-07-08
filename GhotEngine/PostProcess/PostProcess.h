#pragma once

#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <wrl.h>
#include "Matrix4x4.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include <DescriptorManager/SrvManager/SrvManager.h>
#include "DescriptorManager/DescriptorManager.h"
#include "GraphicsPipeline.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")

class PostProcess
{
public:



	PostProcess();
	~PostProcess();

	void Init();
	void Update();
	void Draw();

private:

	Property property_{};

};

