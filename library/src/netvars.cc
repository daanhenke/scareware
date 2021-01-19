#include "netvars.hh"
#include "interfaces.hh"

#include "console.hh"

sw::netvars::NetvarManager::NetvarManager()
{
	for (auto clientClass = interfaces::IBaseClientDLL->GetAllClasses(); clientClass; clientClass = clientClass->next)
	{
		WalkTable(clientClass->networkName, clientClass->recvTable);
	}
}

void sw::netvars::NetvarManager::WalkTable(const char* networkName, iface::RecvTable* recvTable, const std::size_t offset)
{
	//console::WriteColorFormat(FOREGROUND_RED, "Found table: %s\n", networkName);

	for (int i = 0; i < recvTable->propCount; i++)
	{
		auto& prop = recvTable->props[i];

		if (isdigit(prop.name[0]))
			continue;

		if (prop.type == 6 && prop.dataTable && prop.dataTable->netTableName[0] == 'D')
		{
			WalkTable(networkName, prop.dataTable, prop.offset + offset);
		}

		m_netvar_map[std::string(networkName) + "::" + std::string(prop.name)] = offset + prop.offset;
		if (std::string(networkName) == "CBaseCombatCharacter")
		{
			console::WriteColorFormat(FOREGROUND_GREEN, "Found CBaseCombatCharacter prop: %s::%s: %x\n", networkName, prop.name, offset + prop.offset);
		}
	}
}

uint16_t sw::netvars::NetvarManager::GetOffset(std::string name)
{
	if (m_netvar_map.contains(name))
	{
		//console::WriteColorFormat(FOREGROUND_RED, "Resolved netvar '%s': %x\n", name, m_netvar_map[name]);
		return m_netvar_map[name];
	}

	console::WriteColorFormat(FOREGROUND_RED, "Couldn't resolve netvar '%s'\n", name);
	MessageBoxA(nullptr, "Will probably crash now", "Too bad, check console?", MB_OK);

	return 0;
}

sw::netvars::NetvarManager* sw::netvars::manager = nullptr;