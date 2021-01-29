#include "hacks/grief.hh"
#include "interfaces.hh"
#include "util.hh"

#define DEG2RAD(v) (v * M_PI / 180)

void sw::hacks::grief::Blockbot(iface::CUserCmd* cmd)
{
	if (!interfaces::IInputSystem->IsButtonDown(iface::MOUSE_4)) return;

	auto localPlayer = interfaces::GetLocalPlayer();
	if (!localPlayer) return;

	int bestDistance = 250.f;
	int entityIndex = -1;

	// Find closest entity
	for (int i = 1; i < interfaces::IVEngineClient->GetMaxClients(); i++)
	{
		auto entity = interfaces::IClientEntityList->GetClientEntity(i);

		if (!entity) continue;
		if (!entity->IsAlive() || entity->IsDormant() || entity == localPlayer) continue;

		float distance = entity->vecOrigin().DistanceTo(localPlayer->vecOrigin());
		if (distance < bestDistance)
		{
			bestDistance = distance;
			entityIndex = i;
		}
	}

	// No entity within 250 units
	if (entityIndex == -1)
	{
		return;
	}

	auto targetEntity = interfaces::IClientEntityList->GetClientEntity(entityIndex);
	if (!targetEntity) return;

	auto vecForward = targetEntity->vecOrigin() - localPlayer->vecOrigin();
	auto localAngles = localPlayer->angEyeAngles();

	cmd->forwardmove = ((sin(DEG2RAD(localAngles.y)) * vecForward.y) + (cos(DEG2RAD(localAngles.y)) * vecForward.x)) * 450;
	cmd->sidemove = ((cos(DEG2RAD(localAngles.y)) * -vecForward.y) + (sin(DEG2RAD(localAngles.y)) * vecForward.x)) * 450;
}