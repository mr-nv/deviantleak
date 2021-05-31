#pragma once
#include "common.h"
#include "sdk.h"
#include "sdk\ehandle.h"
#include "math\math.h"
#include "interfaces.h"
#include "sdk/animstate.h"
class C_BaseCombatWeapon;
enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

class C_BaseEntity;
class AnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
};

class CHudTexture
{
public:
	char szShortName[64];    //0x0000
	char szTextureFile[64];  //0x0040
	bool bRenderUsingFont;   //0x0080
	bool bPrecached;         //0x0081
	int8_t cCharacterInFont; //0x0082
	uint8_t pad_0083[1];     //0x0083
	uint32_t hFont;          //0x0084
	int32_t iTextureId;      //0x0088
	float afTexCoords[4];    //0x008C
	uint8_t pad_009C[16];    //0x009C
};

#pragma pack(push, 1)
class CCSWeaponInfo
{
public:
	void*   vftbl;
	char*   consoleName;
	char    pad_0008[12];
	int	    iMaxClip1;
	int	    iMaxClip2;
	int	    iDefaultClip1;
	int	    iDefaultClip2;
	char    pad_0024[8];
	char*   szWorldModel;
	char*   szViewModel;
	char*   szDroppedModel;
	char    pad_0038[4];
	char*   N0000023E;
	char    pad_0040[56];
	char*   szEmptySound;
	char    pad_007C[4];
	char*   szBulletType;
	char    pad_0084[4];
	char*   szHudName;
	char*   szWeaponName;
	char    pad_0090[56];
	int     WeaponType;
	int	    iWeaponPrice;
	int	    iKillAward;
	char*   szAnimationPrefix;
	float   flCycleTime;
	float   flCycleTimeAlt;
	float   flTimeToIdle;
	float   flIdleInterval;
	bool    bFullAuto;
	char    pad_0x00E5[3];
	int	    iDamage;
	float   flArmorRatio;
	int	    iBullets;
	float   flPenetration;
	float   flFlinchVelocityModifierLarge;
	float   flFlinchVelocityModifierSmall;
	float   flRange;
	float   flRangeModifier;
	float   flThrowVelocity;
	char    pad_0x010C[12];
	bool    bHasSilencer;
	char    pad_0x0119[3];
	char*   pSilencerModel;
	int	    iCrosshairMinDistance;
	int	    iCrosshairDeltaDistance;
	float   flMaxPlayerSpeed;
	float   flMaxPlayerSpeedAlt;
	float   flSpread;
	float   flSpreadAlt;
	float   flInaccuracyCrouch;
	float   flInaccuracyCrouchAlt;
	float   flInaccuracyStand;
	float   flInaccuracyStandAlt;
	float   flInaccuracyJumpInitial;
	float   flInaccuracyJump;
	float   flInaccuracyJumpAlt;
	float   flInaccuracyLand;
	float   flInaccuracyLandAlt;
	float   flInaccuracyLadder;
	float   flInaccuracyLadderAlt;
	float   flInaccuracyFire;
	float   flInaccuracyFireAlt;
	float   flInaccuracyMove;
	float   flInaccuracyMoveAlt;
	float   flInaccuracyReload;
	int	    iRecoilSeed;
	float   flRecoilAngle;
	float   flRecoilAngleAlt;
	float   flRecoilAngleVariance;
	float   flRecoilAngleVarianceAlt;
	float   flRecoilMagnitude;
	float   flRecoilMagnitudeAlt;
	float   flRecoilMagnitudeVariance;
	float   flRecoilMagnitudeVarianceAlt;
	float   flRecoveryTimeCrouch;
	float   flRecoveryTimeStand;
	float   flRecoveryTimeCrouchFinal;
	float   flRecoveryTimeStandFinal;
	int	    iRecoveryTransitionStartBullet;
	int	    iRecoveryTransitionEndBullet;
	bool    bUnzoomAfterShot;
	bool    bHideViewModelZoomed;
	char    pad_0x01B5[2];
	char    iZoomLevels[3];
	int	    iZoomFOV[2];
	float   fZoomTime[3];
	char*   szWeaponClass;
	float   flAddonScale;
	char    pad_0x01DC[4];
	char*   szEjectBrassEffect;
	char*   szTracerEffect;
	int	    iTracerFrequency;
	int	    iTracerFrequencyAlt;
	char*   szMuzzleFlashEffect_1stPerson;
	char    pad_0x01F4[4];
	char*   szMuzzleFlashEffect_3rdPerson;
	char    pad_0x01FC[4];
	char*   szMuzzleSmokeEffect;
	float   flHeatPerShot;
	char*   szZoomInSound;
	char*   szZoomOutSound;
	float   flInaccuracyPitchShift;
	float   flInaccuracySoundThreshold;
	float   flBotAudibleRange;
	char    pad_0x0218[8];
	char*   pWrongTeamMsg;
	bool    bHasBurstMode;
	char    pad_0x0225[3];
	bool    bIsRevolver;
	bool    bCannotShootUnderwater;
};
#pragma pack(pop)
//
class C_EconItemView
{
public:
};

class C_BaseEntity : public IClientEntity
{
public:

	netvar_fn(int, flags, "DT_CSPlayer", "m_fFlags");
	netvar_fn(Vector, velocity, "DT_BasePlayer", "m_vecVelocity[0]");
	netvar_fn(int, movetype, "DT_CSPlayer", "m_nMoveType");
	netvar_fn(Vector, origin, "DT_BasePlayer", "m_vecOrigin");
	offset_fn(bool, dormant, 0xE9);
	netvar_fn(int, health, "DT_CSPlayer", "m_iHealth");
	netvar_fn(int, life_state, "DT_CSPlayer", "m_lifeState");
	netvar_fn(int, team, "DT_CSPlayer", "m_iTeamNum");
	netvar_fn(Vector, view_offset, "DT_BasePlayer", "m_vecViewOffset[0]");
	netvar_fn(int, armor, "DT_CSPlayer", "m_ArmorValue");
	netvar_fn(CHandle<C_BasePlayer>, owner_handle, "DT_BaseEntity", "m_hOwnerEntity")
	netvar_fn(CHandle<C_BaseCombatWeapon>, activeweapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	netvar_fn(int, tick_base, "DT_CSPlayer", "m_nTickBase");
    netvar_fn(float, next_attack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
    netvar_fn(bool, has_helmet, "DT_CSPlayer", "m_bHasHelmet");
	netvar_fn(float, simulation_time, "DT_CSPlayer", "m_flSimulationTime");
	netvar_fn(Vector, eye_angles, "DT_CSPlayer", "m_angEyeAngles");
    netvar_fn(float, lower_body_yaw, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	netvar_fn(float, getcycle, "DT_CSPlayer", "m_flCycle");
	netvar_fn(int, getsequence, "DT_CSPlayer", "m_nSequence");
	netvar_fn(float, maxspeed, "DT_BasePlayer", "m_flMaxspeed");
	bool alive() {return life_state() == 0;}
	QAngle & getabsangels() { return vmt::get().getvfunc<QAngle&(__thiscall*)(void*)>(this, 11)(this);}
	Vector eyepos(void)	{return origin() + *(Vector*)((DWORD)this + 0x104);}
	void updateclientsideanimation() { typedef void(__thiscall* original)(void*); return vmt::get().getvfunc<original>(this, 218)(this); }
	player_info_t playerinfo() { player_info_t info; ctx::engine->GetPlayerInfo(EntIndex(), &info); return info;	}
	netvar_fn(QAngle, aim_punch, "DT_BasePlayer", "m_aimPunchAngle");
	netvar_fn(bool, immune, "DT_CSPlayer", "m_bGunGameImmunity");

};

class C_PlantedC4
{
public:

};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	inline int* getitemdefinitionindex() {
		return (int*)((DWORD)this + 0x2D70 + 0x40 + 0x1D8);
	}
	
};


class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
	CCSWeaponInfo* getcsweapondata()
	{
		static auto fnGetWpnData = reinterpret_cast<CCSWeaponInfo*(__thiscall*)(void*)>(memory::find_pattern(GetModuleHandleW(L"client.dll"), "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B"));
		return fnGetWpnData(this);
	}
	float getinaccuracy()
	{
		return vmt::get().call_vfunc<float(__thiscall*)(void*)>(this, 468)(this);
	}
	netvar_fn(int, ammoclip, "DT_BaseCombatWeapon", "m_iClip1");
	bool isknife()
	{
		int* Index = getitemdefinitionindex();
		if (*Index == 42 || *Index == 59 || *Index >= 500)
			return true;
		return false;
	}
   const char* getgunname()
	{
		int WeaponId = *this->getitemdefinitionindex();
		switch (WeaponId)
		{
		case WEAPON_AK47:
			return "AK47";
			break;
		case WEAPON_AUG:
			return "AUG";
			break;
		case WEAPON_REVOLVER:
			return "R8-REVOLVER";
			break;
		case WEAPON_AWP:
			return "AWP";
			break;
		case WEAPON_BIZON:
			return "BIZON";
			break;
		case WEAPON_C4:
			return "C4";
			break;
		case WEAPON_CZ75A:
			return "CZ75";
			break;
		case WEAPON_DEAGLE:
			return "DESERT EAGLE";
			break;
		case WEAPON_DECOY:
			return "DECOY";
			break;
		case WEAPON_ELITE:
			return "DUALBERETTAS";
			break;
		case WEAPON_FAMAS:
			return "FAMAS";
			break;
		case WEAPON_FIVESEVEN:
			return "FIVESEVEN";
			break;
		case WEAPON_FLASHBANG:
			return "FLASH";
			break;
		case WEAPON_G3SG1:
			return "G3SG1";
			break;
		case WEAPON_GALILAR:
			return "GALIL";
			break;
		case WEAPON_GLOCK:
			return "GLOCK";
			break;
		case WEAPON_INCGRENADE:
			return "INCENDENARY";
			break;
		case WEAPON_MOLOTOV:
			return "MOLOTOV";
			break;
		case WEAPON_SSG08:
			return "SSG08";
			break;
		case WEAPON_HEGRENADE:
			return "HEGRENADE";
			break;
		case WEAPON_M249:
			return "M249";
			break;
		case WEAPON_M4A1:
			return "M4A1";
			break;
		case WEAPON_MAC10:
			return "MAC10";
			break;
		case WEAPON_MAG7:
			return "MAG7";
			break;
		case WEAPON_MP7:
			return "MP7";
			break;
		case WEAPON_MP9:
			return "MP9";
			break;
		case WEAPON_NOVA:
			return "NOVA";
			break;
		case WEAPON_NEGEV:
			return "NEGEV";
			break;
		case WEAPON_P250:
			return "P250";
			break;
		case WEAPON_P90:
			return "P90";
			break;
		case WEAPON_SAWEDOFF:
			return "SAWEDOFF";
			break;
		case WEAPON_SCAR20:
			return "SCAR-20";
			break;
		case WEAPON_SMOKEGRENADE:
			return "SMOKE";
			break;
		case WEAPON_SG556:
			return "SG553";
			break;
		case WEAPON_TEC9:
			return "TEC9";
			break;
		case WEAPON_HKP2000:
			return "P2000";
			break;
		case WEAPON_USP_SILENCER:
			return "USP-S";
			break;
		case WEAPON_UMP45:
			return "UMP45";
			break;
		case WEAPON_XM1014:
			return "XM1014";
			break;
		case WEAPON_TASER:
			return "TASER";
			break;
		case WEAPON_M4A1_SILENCER:
			return "M4A1-S";
		}
		if (isknife())
			return "KNIFE";
		return "";
	}
	netvar_fn(float, firereadytime, "DT_WeaponCSBase", "m_flPostponeFireReadyTime");
};

class C_BasePlayer : public C_BaseEntity
{
public:

};

class C_BaseViewModel : public C_BaseEntity
{
public:
	
};