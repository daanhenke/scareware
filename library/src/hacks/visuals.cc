#include "hacks/visuals.hh"
#include "interfaces.hh"
#include "iface/Vector2D.hh"
#include "util.hh"
#include <algorithm>
#include "interfaces.hh"
#include "console.hh"
#include "iface/Color.hh"
#include "draw.hh"
#include <vector>

#undef min
#undef max

int sw::hacks::visuals::Velocity = 0;

struct EntityBB
{
	sw::iface::Vector2D min;
	sw::iface::Vector2D max;
	bool isValid;

	EntityBB(sw::iface::IClientEntity* entity)
	{
		isValid = true;

		auto origin = entity->vecOrigin();

		auto collide = entity->GetCollideable();
		auto obbMax = sw::iface::Vector(0, 0, collide->OBBMaxs().z) + origin;

		sw::iface::Vector pos2d, top2d;
		if (!sw::util::WorldToScreen(origin, pos2d) || !sw::util::WorldToScreen(obbMax, top2d))
		{
			isValid = false;
			return;
		}

		float height = pos2d.y - top2d.y;
		float width = height / 2.f;

		min.x = top2d.x - width / 2;
		min.y = top2d.y;

		max.x = top2d.x + width / 2;
		max.y = top2d.y + height;
	}
};

#include "iface/WeaponId.hh"
using namespace sw;
std::map<iface::WeaponId, char> weapon_icons =
{
	{ iface::WeaponId::Awp, 'Z' },
	{ iface::WeaponId::Ak47, 'W' }
};

void sw::hacks::visuals::RenderPlayer(sw::iface::IClientEntity* entity)
{
	// Show on minimap
	auto localPlayer = interfaces::GetLocalPlayer();
	if (localPlayer->iTeamNum() != entity->iTeamNum())
	{
		entity->bSpotted() = true;
	}

	// Render text etc
	auto bb = EntityBB(entity);
	iface::PlayerInfo playerInfo;

	if (!bb.isValid) return;

	interfaces::IVEngineClient->GetPlayerInfo(entity->Index(), playerInfo);

	std::string playerNameStr(playerInfo.name);
	std::wstring playerNameWideStr(playerNameStr.begin(), playerNameStr.end());

	playerNameWideStr += L"\t(" + std::to_wstring(entity->Health()) + L" HP";

	auto armor = entity->ArmorValue();
	if (armor != 0)
	{
		playerNameWideStr += L", " + std::to_wstring(armor) + L" AR";
	}

	if (entity->bHasHelmet())
	{
		playerNameWideStr += L", H";
	}

	if (entity->bHasDefuser())
	{
		playerNameWideStr += L", DF";
	}

	playerNameWideStr += L")";

	draw::DrawText(draw::FontDefault, playerNameWideStr, bb.min.x, bb.min.y - 20);

	auto weapon = entity->GetActiveWeapon();
	if (!weapon) return;

	auto weaponData = weapon->GetWeaponData();
	if (!weaponData || !weaponData->name) return;
	std::string weaponNameStr(weaponData->name);
	std::wstring weaponNameWideStr(weaponNameStr.begin(), weaponNameStr.end());
	std::wstring weaponChunk = weaponNameWideStr.substr(weaponNameWideStr.rfind(L'_') + 1);

	auto clip = weapon->iClip1();
	if (clip != -1)
	{
		weaponChunk += L" (" + std::to_wstring(clip) + L" / " + std::to_wstring(weaponData->maxClip) + L")";
	}

	draw::DrawText(draw::FontDefault, weaponChunk, bb.min.x, bb.min.y - 10);

	std::wstring specialStr = L"";
	bool renderSpecial = true;

	if (entity->bIsDefusing())
	{
		specialStr += L" * defusing *";
	}
	else if (entity->bIsScoped())
	{
		specialStr += L"* scoped *";
	}
	else if (weapon != nullptr && weapon->IsReloading())
	{
		specialStr += L"* reloading *";
	}
	else
	{
		renderSpecial = false;
	}

	if (renderSpecial)
	{
		draw::DrawText(draw::FontDefault, specialStr, bb.min.x, bb.min.y - 30);
	}
	

	//interfaces::ISurface->DrawSetColor(255, 0, 0, 255);
	//interfaces::ISurface->DrawFilledRect(bb.min.x, bb.min.y, bb.max.x, bb.max.y);
}

void sw::hacks::visuals::RenderWeapon(sw::iface::IClientEntity* entity)
{
	auto bb = EntityBB(entity);

	if (!bb.isValid) return;

	auto weaponData = entity->GetWeaponData();
	std::string weaponNameStr(weaponData->name);
	std::wstring weaponNameWideStr(weaponNameStr.begin(), weaponNameStr.end());
	std::wstring weaponChunk = weaponNameWideStr.substr(weaponNameWideStr.rfind(L'_') + 1);

	draw::DrawText(draw::FontDefault, weaponChunk, bb.min.x, bb.min.y);

	auto clip = entity->iClip1();
	if (clip == -1) return;

	std::wstring ammoStr = std::to_wstring(clip) + L" / " + std::to_wstring(weaponData->maxClip);
	draw::DrawText(draw::FontDefault, ammoStr, bb.min.x, bb.min.y + 10);
}

void sw::hacks::visuals::RenderVelocity()
{
	auto localPlayer = interfaces::GetLocalPlayer();
	if (!localPlayer) return;

	auto vecVelocity = localPlayer->vecVelocity();
	vecVelocity.z = 0;
	int velocity = round(vecVelocity.Length());
	static int lastVelocity = 0;
	static int lastFlags = localPlayer->fFlags();
	Velocity = velocity;
	if (!(localPlayer->fFlags() & 1) && lastFlags & 1) lastVelocity = velocity;
	lastFlags = localPlayer->fFlags();

	std::wstring velocityString = std::to_wstring(velocity);
	std::wstring lastVelocityString = L"";
	if (lastVelocity > 0 && !(localPlayer->fFlags() & 1)) lastVelocityString = L"(" + std::to_wstring(lastVelocity) + L")";

	int textW, textH;
	int textW2, textH2;
	sw::interfaces::ISurface->GetTextSize(draw::FontDefault, velocityString.c_str(), &textW, &textH);
	sw::interfaces::ISurface->GetTextSize(draw::FontDefault, lastVelocityString.c_str(), &textW2, &textH2);

	int width, height;
	sw::interfaces::IVEngineClient->GetScreenSize(width, height);

	iface::Color Result = util::MixColors(iface::Color(255, 180, 0), iface::Color(0, 255, 0), std::min(1.f, velocity / 250.f));
	iface::Color lastResult = util::MixColors(iface::Color(255, 180, 0), iface::Color(0, 255, 0), std::min(1.f, lastVelocity /250.f));

	draw::DrawText(draw::FontDefault, velocityString, width / 2 - textW / 2, height / 5 * 4, Result);
	draw::DrawText(draw::FontDefault, lastVelocityString, width / 2 - textW2 / 2, height / 5 * 4 + textH + 5, lastResult);

	auto weapon = localPlayer->GetActiveWeapon();
	if (!weapon) return;

	weapon->nFallbackStatTrak() = velocity;
	weapon->PostDataUpdate(0);
}

void sw::hacks::visuals::Render()
{
	auto localPlayer = interfaces::GetLocalPlayer();
	if (!localPlayer) return;

	for (int i = 1; i < interfaces::IClientEntityList->GetHighestEntityIndex(); i++)
	{
		auto entity = interfaces::IClientEntityList->GetClientEntity(i);

		if (!entity) continue;
		//if (localPlayer == entity) continue;
		if (entity->IsDormant() || !entity->IsAlive()) continue;

		if (entity->IsPlayer())
		{
			RenderPlayer(entity);
		}
		else if (entity->IsWeapon() && entity->hOwnerEntity() == -1)
		{
			RenderWeapon(entity);
		}

		/*BYTE* fastRenderPath = reinterpret_cast<BYTE*>(entity->flFrozen()) + 4;
		fastRenderPath = 0;*/
	}

	RenderVelocity();
	RenderSpectatorList();
}

void sw::hacks::visuals::RenderSpectatorList()
{
	auto localPlayer = interfaces::GetLocalPlayer();
	if (!localPlayer) return;

	int scr_width, scr_height;
	interfaces::IVEngineClient->GetScreenSize(scr_width, scr_height);

	std::vector<std::wstring> spectatingEntityNames;
	spectatingEntityNames.push_back(L"Spectating:");
	for (int entityId = 1; entityId < interfaces::IClientEntityList->GetHighestEntityIndex(); entityId++)
	{
		auto entity = interfaces::IClientEntityList->GetClientEntity(entityId);
		if (!entity) continue;
		if (!entity->IsPlayer()) continue;
		if (entity->GetObserverMode() == iface::ObsMode::OBS_MODE_NONE) continue;
		if (entity->GetObserverTarget() != localPlayer) continue;

		iface::PlayerInfo info;
		interfaces::IVEngineClient->GetPlayerInfo(entity->Index(), info);
		auto input_string = std::string(info.name);
		std::wstring output_string(input_string.size(), L' ');
		output_string.resize(mbstowcs(&output_string[0], input_string.c_str(), input_string.size()));
		spectatingEntityNames.push_back(output_string);
	}

	int window_width = 0, window_height = 0;
	for (auto name : spectatingEntityNames)
	{
		int width, height;
		interfaces::ISurface->GetTextSize(draw::FontDefault, name.c_str(), &width, &height);
		window_height += height + 5;
		window_width = std::max(10 + width, window_width);
	}

	draw::DrawFillRect(scr_width - window_width, 0, window_width, window_height, iface::Color(0, 0, 0, 100));
	int text_offset_y = 5;
	for (auto name : spectatingEntityNames)
	{
		int char_w, char_h;
		interfaces::ISurface->GetTextSize(draw::FontDefault, name.c_str(), &char_w, &char_h);
		draw::DrawText(draw::FontDefault, name, scr_width - char_w - 5, text_offset_y);
		text_offset_y += char_h + 5;
	}
}