#pragma once
#include "../../Utils.h"
#include "../libraries/math.h"

class CBaseEntity
{
public:
	float& GetFriction()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_flFriction");
		return *(float*)(this + offset);
	}

	int& GetTeam()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_iTeamNum");
		return *(int*)(this + offset);
	}

	Vector& GetVelocity()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_vecVelocity[0]");
		return *(Vector*)(this + offset);
	}

	int& GetHealth()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_iHealth");
		return *(int*)(this + offset);
	}

	int Index()
	{
		return *(int*)((DWORD)this + 64);
	}

	int& GetFlags()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_fFlags");
		return *(int*)(this + offset);
	}

	QAngle& GetAimPunch()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_aimPunchAngle");
		return *(QAngle*)(this + offset);
	}

	QAngle& GetViewPunch()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_viewPunchAngle");
		return *(QAngle*)(this + offset);
	}

	int& GetTickBase()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_nTickBase");
		return *(int*)(this + offset);
	}

	Vector GetEyePosition()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_vecViewOffset[0]");
		Vector vecViewOffset = *(Vector*)(this + offset);
		return GetAbsOrigin() + vecViewOffset;
	}

	QAngle& GetEyeAngles() const
	{
		static int offset = netVars->GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		return *(Vector*)(this + offset);
	}

	void* ActiveWeapon()
	{
		static int offset = netVars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		return *(void**)(this + offset);
	}

	/*
	const char* GetClassname()
	{
		static int offset = netVars->GetOffset("DT_BaseEntity", "m_iClassname");
		return *(const char**)(this + offset);
	}

	const char* GetModelName()
	{
		static int offset = netVars->GetOffset("DT_BaseEntity", "m_ModelName");
		return *(const char**)(this + offset);
	}

	const char* GetName()
	{
		static int offset = netVars->GetOffset("DT_BaseEntity", "m_iName");
		return *(const char**)(this + offset);
	}
	*/

	template<typename T>
	T& GetNetProp(std::string prop, std::string table = "DT_BaseEntity") const
	{
		int offset = netVars->GetOffset(table.c_str(), prop.c_str());
		return *(T*)(this + offset);
	}

	Vector& GetAbsOrigin()
	{
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return ((OriginalFn)VMT.GetFunction(this, indexes::GetAbsOrigin))(this);
	}

	Vector& GetAbsAngles()
	{
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return ((OriginalFn)VMT.GetFunction(this, indexes::GetAbsAngles))(this);
	}

	void* GetClientClass()
	{
		void* Networkable = (void*)(this + 0x8);
		typedef void* (__thiscall* OriginalFn)(void*);
		return ((OriginalFn)VMT.GetFunction(Networkable, indexes::GetClientClass))(Networkable);
	}
	
	bool IsDormant()
	{
		// return *reinterpret_cast<bool*>((DWORD)this + 0xE9);
		PVOID pNetworkable = static_cast<PVOID>(this + 0x8);
		typedef bool(__thiscall * OriginalFn)(PVOID);
		return ((OriginalFn)VMT.GetFunction(pNetworkable, indexes::IsDormant))(pNetworkable);
	}

	bool IsAlive()
	{
		static int offset = netVars->GetOffset("DT_BasePlayer", "m_lifeState");
		BYTE lifestate = *(BYTE*)((DWORD)this + offset);
		return (lifestate == 0);
	}

	int GetIndex()
	{
		void* networkable = (void*)(this + 0x8);
		typedef int(__thiscall* OriginalFn)(PVOID);
		return ((OriginalFn)VMT.GetFunction(this, indexes::GetIndex))(this);
	}

	bool SetupBones(VMatrix *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		void *pRenderable = (void*)(this + 0x4);
		typedef bool(__thiscall* OriginalFn)(PVOID, VMatrix*, int, int, float);
		return ((OriginalFn)VMT.GetFunction(pRenderable, indexes::SetupBones))(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	model_t* GetModel()
	{
		void *pRenderable = (void*)(this + 0x4);
		typedef model_t*(__thiscall* OriginalFn)(PVOID);
		return VMT.getvfunc<OriginalFn>(pRenderable, indexes::GetModel)(pRenderable);
	}

	int GetWeaponID()
	{
		typedef int(__thiscall *OriginalFn)(PVOID);
		return VMT.getvfunc<OriginalFn>(this, indexes::GetWeaponId)(this);
	}

	Vector GetHitboxPosition(int Hitbox)
	{
		VMatrix matrix[128];
		if (!this->SetupBones(matrix, 128, 0x00000100, GetTickCount64())) return Vector(0, 0, 0);
		const model_t* mod = this->GetModel();
		if (!mod) return Vector(0, 0, 0);
		studiohdr_t* hdr = Interfaces.ModelInfo->GetStudioModel(mod);
		if (!hdr) return Vector(0, 0, 0);
		mstudiohitboxset_t* set = hdr->pHitboxSet(0);
		if (!set) return Vector(0, 0, 0);
		mstudiobbox_t* hitbox = set->pHitbox(Hitbox);
		if (!hitbox) return Vector(0, 0, 0);
		Vector MIN, MAX, MIDDLE;
		VectorTransform(hitbox->bbmin, matrix[hitbox->bone], MIN);
		VectorTransform(hitbox->bbmax, matrix[hitbox->bone], MAX);
		MIDDLE = (MIN + MAX) * 0.5f;
		return MIDDLE;
	}

	Vector GetBonePosition(int bone)
	{
		VMatrix boneMatrix[128];
		if (this->SetupBones(boneMatrix, 128, 0x00000100, GetTickCount64()))
		{
			return Vector(boneMatrix[bone][0][3], boneMatrix[bone][1][3], boneMatrix[bone][2][3]);
		}

		return Vector(0, 0, 0);
	}
};