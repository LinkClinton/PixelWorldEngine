#pragma once

#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <queue>
#include <cmath>
#include <map>
#include <set>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace PixelWorldEngine {

	typedef unsigned char byte;

}
#ifdef _WIN32

#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "freetype.lib")

#endif // _WIN32
