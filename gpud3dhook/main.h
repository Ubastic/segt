#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9types.h>
#include "d3dumddi.h"
#include <cstdio>
#include <functional>
#include <dwmapi.h>
#include "overlay.h"

// #include "../detours/detours.h"
// #include <easyhook.h>

#ifdef EASYHOOK_API
#if defined(_AMD64_) || defined(_IA64_) || defined(_ARM64_)
	#pragma comment(lib, "EasyHook64")
#else
	#pragma comment(lib, "EasyHook32")
#endif
#endif

#ifdef DETOURS_VERSION
#pragma comment(lib, "detours")
#else
#include "detourxs.h"
#endif

#ifdef D3D_SDK_VERSION
#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")
#endif

#ifdef DWMAPI
#pragma comment(lib, "dwmapi")
#endif

#define NAMESPACE_START(_x)		namespace _x {
#define NAMESPACE_END			}

NAMESPACE_START(dh)

typedef HRESULT (APIENTRY FnOpenAdapter)(D3DDDIARG_OPENADAPTER*);
typedef struct D3DHook
{
	PFND3DDDI_DRAWINDEXEDPRIMITIVE oDrawIndexedPrimitive = nullptr;
	PFND3DDDI_SETRENDERSTATE oSetRenderState = nullptr;
	PFND3DDDI_CREATEQUERY oCreateQuery = nullptr;
	PFND3DDDI_SETSTREAMSOURCE oSetStreamSource = nullptr;
	PFND3DDDI_PRESENT oPresent = nullptr;
	PFND3DDDI_LOCK oLock = nullptr;
	PFND3DDDI_CREATERESOURCE oCreateResource = nullptr;
	PFND3DDDI_UNLOCK oUnlock = nullptr;
	PFND3DDDI_BLT oBlt = nullptr;

	DWORD iStride;
} D3DPFNHooker;

extern Overlay* gOverlay;
extern IDirect3D9* gD3D9Internal;
extern IDirect3DDevice9* gDeviceInternal;
extern FnOpenAdapter* oOpenAdapter;
extern PFND3DDDI_CREATEDEVICE oCreateDevice;
extern D3DPFNHooker gDetourFunc, gDetourFunc1;
extern PFND3DDDI_DRAWINDEXEDPRIMITIVE oDrawIndexedPrimitive;
extern PFND3DDDI_SETRENDERSTATE oSetRenderState;
extern PFND3DDDI_CREATEQUERY oCreateQuery;
extern PFND3DDDI_SETSTREAMSOURCE oSetStreamSource;
extern PFND3DDDI_PRESENT oPresent;
extern PFND3DDDI_LOCK oLock;
extern PFND3DDDI_CREATERESOURCE oCreateResource;
extern PFND3DDDI_UNLOCK oUnlock;
extern PFND3DDDI_BLT oBlt;

/*
* ��ʼ�� D3D ����
*
* @return None
*/
void InitGPUD3DHook();

/*
* ���Գ�ʼ�� Direct3D9
*
* @return			����ɹ����� true ���� false
*/
bool InitDesktopDirectX(HWND window = nullptr);

/*
* ����һ���ٵĴ��ڣ��û� hook ԭ����
*
* @return			����ɹ����� true ���� false
*/
bool InitFakeDirectXDevice();

/*
* ���ٴ����ͷŵ�
*
* @return			����ɹ����� true ���� false
*/
bool ReleaseFakeDirectXDevice();

/*
* ��ʼһ�� IDirect3DDevice9 �ҹ�
* ����ɹ��������� func ��Ϊ Hook ����
*
* @param func		�ҹ��õĺ��������� IDirect3D9* Ϊ pD3D�� IDirect3DDevice9* Ϊ pD3DDevice��
*					DWORD* Ϊ�麯����ָ�룬ֱ��ʹ��������� Hook ��;
*
* @return			None
*/
void StartDeviceHook(std::function<void(IDirect3D9*&, IDirect3DDevice9*&, DWORD*&)> func);

// �������� D3D Device ָ�룬���Ƿǳ���
IDirect3DDevice9* FindDirexcXDevice();

/*
* ��ʼһ�� IDirect3DDevice9 �ҹ�
* ����ɹ��������� func ��Ϊ Hook ����
*
* @param window		�����ǵĴ���
*
* @return			None
*/
void CreateOverlay(HWND window, HINSTANCE instance);

NAMESPACE_END

/*
* �ҹ�����
*
* @param src		ԭ����
* @param dst		��Ҫ�޸ĵ��ĸ�����
* @param len		������С
*
* @return			ԭ�����ĵ�ַ
*/
template<typename Fn>
Fn* DetourFunction(Fn* src, Fn* dst, int len);

/*
* ���¹ҹ�����
*
* @param src		ԭ����
* @param dst		��Ҫ�޸ĵ��ĸ�����
* @param len		������С
*
* @return			ԭ�����ĵ�ַ
*/
template<typename Fn>
Fn* RetourFunction(Fn* src, Fn* dst, int len);
