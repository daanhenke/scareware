#include "hacks/predict.hh"
#include "interfaces.hh"
#include "memory.hh"
#include "util.hh"

int sw::hacks::predict::OldFlags = 0;

void sw::hacks::predict::RunPrediction(iface::CUserCmd* cmd)
{
	auto localPlayer = interfaces::GetLocalPlayer();
	if (!localPlayer) return;

	OldFlags = localPlayer->fFlags();

	*memory::predictSeed = 0;

	auto oldCurrentTime = interfaces::CGlobalVars->currenttime;
	auto oldFrametime = interfaces::CGlobalVars->frametime;

	interfaces::CGlobalVars->currenttime = util::GetServerTime(cmd);
	interfaces::CGlobalVars->frametime = interfaces::CGlobalVars->intervalPerTick;

	memory::IMoveHelper->SetHost(localPlayer);
	interfaces::CPrediction->SetupMove(localPlayer, cmd, memory::IMoveHelper, memory::CMoveData);
	interfaces::IGameMovement->ProcessMovement(localPlayer, memory::CMoveData);
	interfaces::CPrediction->FinishMove(localPlayer, cmd, memory::CMoveData);
	memory::IMoveHelper->SetHost(nullptr);

	*memory::predictSeed = -1;
	interfaces::CGlobalVars->frametime = oldFrametime;
	interfaces::CGlobalVars->currenttime = oldCurrentTime;
}