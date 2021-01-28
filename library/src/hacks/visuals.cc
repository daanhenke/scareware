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


	interfaces::ISurface->DrawSetTextFont(draw::FontDefault);
	interfaces::ISurface->DrawSetTextColor(255, 255, 255, 255);

	wchar_t nameBuffer[128];
	MultiByteToWideChar(CP_UTF8, 0, playerInfo.name, 128, nameBuffer, 128);
	interfaces::ISurface->DrawSetTextPos(bb.min.x, bb.min.y - 20);
	interfaces::ISurface->DrawPrintText(nameBuffer, wcslen(nameBuffer));

	interfaces::ISurface->DrawSetTextPos(bb.min.x, bb.min.y - 40);
	interfaces::ISurface->DrawSetTextFont(draw::FontCSGOIcons);
	interfaces::ISurface->DrawPrintText(L"\x52", 1);

	//interfaces::ISurface->DrawSetColor(255, 0, 0, 255);
	//interfaces::ISurface->DrawFilledRect(bb.min.x, bb.min.y, bb.max.x, bb.max.y);
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

	draw::DrawShadedText(draw::FontDefault, velocityString, width / 2 - textW / 2, height / 5 * 4, Result);
	draw::DrawShadedText(draw::FontDefault, lastVelocityString, width / 2 - textW2 / 2, height / 5 * 4 + textH + 5, lastResult);

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
		if (localPlayer == entity) continue;
		if (entity->IsDormant() || !entity->IsAlive()) continue;

		if (entity->IsPlayer())
		{
			RenderPlayer(entity);
		}

		BYTE* fastRenderPath = reinterpret_cast<BYTE*>(entity->flFrozen()) + 4;
		fastRenderPath = 0;
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

	draw::DrawFillRect(scr_width - window_width, 0, window_width, window_height, iface::Color(50, 50, 50, 255));
	int text_offset_y = 5;
	for (auto name : spectatingEntityNames)
	{
		int char_w, char_h;
		interfaces::ISurface->GetTextSize(draw::FontDefault, name.c_str(), &char_w, &char_h);
		draw::DrawText(draw::FontDefault, name, scr_width - char_w - 5, text_offset_y);
		text_offset_y += char_h + 5;
	}
}