#pragma once

#include <cstdint>

namespace sw::iface
{
	struct PlayerInfo
	{
		std::uint64_t version;
		std::uint64_t xuid;
		char name[128];
		int userId;
		char guid[33];
		std::uint32_t friendsId;
		char friendsName[128];
		bool fakeplayer;
		bool hltv;
		int customfiles[4];
		unsigned char filesDownloaded;
	};
}