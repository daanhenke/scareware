#include "hacks/visuals.hh"
#include "interfaces.hh"
#include "iface/Vector2D.hh"
#include "util.hh"
#include <algorithm>
#include "interfaces.hh"
#include "console.hh"

#undef min
#undef max

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

int sw::hacks::visuals::Velocity = -1;

static sw::iface::HFont baseFont = 0;
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


	interfaces::ISurface->DrawSetTextFont(baseFont);
	interfaces::ISurface->DrawSetTextColor(255, 255, 255, 255);

	wchar_t nameBuffer[128];
	MultiByteToWideChar(CP_UTF8, 0, playerInfo.name, 128, nameBuffer, 128);
	interfaces::ISurface->DrawSetTextPos(bb.min.x, bb.min.y - 20);
	interfaces::ISurface->DrawPrintText(nameBuffer, wcslen(nameBuffer));

	interfaces::ISurface->DrawSetTextPos(bb.min.x, bb.min.y - 40);
	interfaces::ISurface->DrawSetTextFont(0xa1);
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
	int velocity = vecVelocity.Length();

	std::wstring velocityString = std::to_wstring(velocity);

	sw::interfaces::ISurface->DrawSetTextFont(baseFont);
	sw::interfaces::ISurface->DrawSetTextColor(255, 255, 255, 255);

	int width, height;
	sw::interfaces::IVEngineClient->GetScreenSize(width, height);
	sw::interfaces::ISurface->DrawSetTextPos(width / 2, height / 4 * 3);
	sw::interfaces::ISurface->DrawPrintText(velocityString.c_str(), velocityString.length());

	auto weapon = localPlayer->GetActiveWeapon();
	if (!weapon) return;

	weapon->nFallbackStatTrak() = velocity;
	weapon->PostDataUpdate(0);
}

void sw::hacks::visuals::Render()
{
	auto localPlayer = interfaces::GetLocalPlayer();
	if (!localPlayer) return;

	if (baseFont == 0)
	{
		baseFont = sw::interfaces::ISurface->CreateFont();
		sw::interfaces::ISurface->SetFontGlyphSet(baseFont, "Arial", 15, 450, 0, 0, sw::iface::EFontFlags::FONTFLAG_ANTIALIAS);
	}

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
}