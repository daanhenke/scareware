#include "hacks/skin.hh"
#include "interfaces.hh"
#include <cstring>
#include "console.hh"

void sw::hacks::skin::FrameStageNotify()
{
	static bool meme = false;
	auto local_player = interfaces::GetLocalPlayer();

	if (!local_player) return;

	if (! local_player->IsAlive())
	{
		return;
	}

	auto& weapons = local_player->hMyWeapons();

	for (int index = 0; weapons[index] != 0xFFFFFFFF; index++)
	{
		interfaces::ICvar->ConsoleDPrintf("weapon handle: %x\n", weapons[index]);

		auto weapon = interfaces::IClientEntityList->GetClientEntityFromHandle((unsigned long) weapons[index]);

		if (!weapon) continue;
		interfaces::ICvar->ConsoleDPrintf("weapon entity: %x\n", weapon);

		auto& weaponId = weapon->iItemDefinitionIndex2();

		strcpy(weapon->szCustomName(), "VaporWare xDDDDD - daan");
		weapon->iItemIDHigh() = -1;
		weapon->flFallbackWear() = 0.0;
		weapon->nFallbackStatTrak() = 1337;

		if (weaponId == iface::WeaponId::Awp)
		{
			weapon->nFallbackPaintKit() = 344;
		}

		if (weaponId == iface::WeaponId::M4A1)
		{
			weapon->nFallbackPaintKit() = 309;
		}
	}

	meme = true;
}