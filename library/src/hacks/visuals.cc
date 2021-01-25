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

void RenderPlayer(sw::iface::IClientEntity* entity)
{
	static sw::iface::HFont baseFont = 0;
	auto bb = EntityBB(entity);
	sw::iface::PlayerInfo playerInfo;

	if (!bb.isValid) return;

	if (baseFont == 0)
	{
		baseFont = sw::interfaces::ISurface->CreateFont();
		sw::interfaces::ISurface->SetFontGlyphSet(baseFont, "Arial", 15, 450, 0, 0, sw::iface::EFontFlags::FONTFLAG_ANTIALIAS);
	}

	sw::interfaces::IVEngineClient->GetPlayerInfo(entity->Index(), playerInfo);


	sw::interfaces::ISurface->DrawSetTextFont(baseFont);
	sw::interfaces::ISurface->DrawSetTextColor(255, 255, 255, 255);

	wchar_t nameBuffer[128];
	MultiByteToWideChar(CP_UTF8, 0, playerInfo.name, 128, nameBuffer, 128);
	sw::interfaces::ISurface->DrawSetTextPos(bb.min.x, bb.min.y - 20);
	sw::interfaces::ISurface->DrawPrintText(nameBuffer, wcslen(nameBuffer));

	sw::interfaces::ISurface->DrawSetTextPos(bb.min.x, bb.min.y - 40);
	sw::interfaces::ISurface->DrawSetTextFont(0xa1);
	sw::interfaces::ISurface->DrawPrintText(L"\x52", 1);
}

void sw::hacks::visuals::Render()
{
	auto localPlayer = interfaces::GetLocalPlayer();

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
	}
}