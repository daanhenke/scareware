#pragma once

#include <cstdint>

namespace sw::iface
{
    enum class WeaponType;

	struct WeaponInfo
	{
		char pad0[20];
		int maxClip;
		char pad1[112];
		const char* name;
		char pad2[60];
        WeaponType type;
        char pad3[4];
        int price;
        char pad4[8];
        float cycletime;
        char pad5[12];
        bool fullAuto;
        char pad6[3];
        int damage;
        float armorRatio;
        int bullets;
        float penetration;
        char pad7[8];
        float range;
        float rangeModifier;
        char pad8[16];
        bool silencer;
        char pad9[15];
        float maxSpeed;
        float maxSpeedAlt;
        char pad10[100];
        float recoilMagnitude;
        float recoilMagnitudeAlt;
        char pad11[16];
        float recoveryTimeStand;
	};
}