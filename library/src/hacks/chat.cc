#include "hacks/chat.hh"
#include "interfaces.hh"

void sw::hacks::chat::LogVoteStart(iface::IGameEvent* ev)
{
	auto chat = interfaces::ClientModeShared->GetHudChat();

	if (chat == nullptr) return;

	chat->Printf(0, "Player '%s' started a %s vote", "TestPlayer", "TestVote");
}

void sw::hacks::chat::LogVoteCast(iface::IGameEvent* ev)
{
	auto chat = interfaces::ClientModeShared->GetHudChat();
	if (chat == nullptr) return;

	auto player = interfaces::IClientEntityList->GetClientEntity(ev->GetInt("entityid"));
	if (!player || !player->IsPlayer()) return;

	auto vote_option = ev->GetInt("vote_option");
	
	iface::PlayerInfo info;
	interfaces::IVEngineClient->GetPlayerInfo(player->Index(), info);

	chat->Printf(0, "Player '%s' voted: %s", info.name, vote_option == 0 ? "\x04YES" : "\x02NO");
}