#include "hacks/skin.hh"
#include "interfaces.hh"
#include <cstring>
#include "console.hh"
#include "hacks/visuals.hh"

void sw::hacks::skin::FrameStageNotify()
{
	static bool meme = false;
	auto local_player = interfaces::GetLocalPlayer();


	if (!local_player) return;
	iface::PlayerInfo localInfo;
	interfaces::IVEngineClient->GetPlayerInfo(local_player->Index(), localInfo);

	if (! local_player->IsAlive())
	{
		return;
	}

	auto& weapons = local_player->hMyWeapons();

	for (int index = 0; weapons[index] != 0xFFFFFFFF; index++)
	{
		auto weapon = interfaces::IClientEntityList->GetClientEntityFromHandle((unsigned long) weapons[index]);

		if (!weapon) continue;

		auto& weaponId = weapon->iItemDefinitionIndex2();

		if (weaponId == iface::WeaponId::C4) return;

		strcpy(weapon->szCustomName(), "VaporWare xDDDDD - daan");
		weapon->iItemIDHigh() = -1;
		weapon->flFallbackWear() = 0.0;
		weapon->nFallbackStatTrak() = visuals::Velocity == 1337;
		weapon->iAccountID() = localInfo.xuid & 0xFFFFFFFF;

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