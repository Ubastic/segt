#pragma once
#pragma warning(disable : 4800)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)

#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <map>

using namespace std;
extern std::ofstream errlog;

#include "definitions.h"
#include "indexes.h"
#include "libraries\xorstr.h"

#include "libraries\vmt.h"
#include "../gpud3dhook/main.h"
#include "../imgui/drawer/DrawManager.h"
#include "../imgui/imgui.h"
#include "../imgui/DX9/imgui_impl_dx9.h"

#include "structs\keyvalues.h"
#include "structs\vector.h"
#include "structs\vmatrix.h"
#include "structs\usercmd.h"
#include "structs\playerinfo.h"
#include "structs\engine.h"
#include "structs\globals.h"
#include "structs\input.h"
#include "structs\panel.h"
#include "structs\surface.h"
#include "structs\client.h"
#include "structs\crc32.h"
#include "structs\quaternion.h"
#include "structs\modelinfo.h"
#include "structs\checksum_md5.h"
#include "libraries\interfaces.h"
#include "Utils.h"
#include "structs\weapon.h"
#include "structs\baseentity.h"
#include "structs\cliententlist.h"
#include "structs\trace.h"
#include "structs\debugoverlay.h"
#include "structs\console.h"
#include "structs\event.h"
#include "structs\render.h"
#include "libraries\math.h"
