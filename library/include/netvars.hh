#pragma once

#include <map>
#include <string>
#include "iface/RecvProp.hh"
#include "console.hh"

namespace sw::netvars
{
	class NetvarManager
	{
	public:
		NetvarManager();
		uint16_t GetOffset(std::string name);
	private:
		void WalkTable(const char* networkName, iface::RecvTable* recvTable, const std::size_t offset = 0);
		std::map<std::string, uint16_t> m_netvar_map;
	};

	extern NetvarManager* manager;
}

//#define NETVAR_OFFSET(funcname, classname, varname, type) \
//[[nodiscard]] std::add_lvalue_reference_t<type> funcname() \
//{ \
//	/*return *reinterpret_cast<std::add_pointer_t<type>>(this + sw::netvars::manager->GetOffset(classname "::" varname));*/ \
//	std::add_lvalue_reference_t<type> res = *reinterpret_cast<std::add_pointer_t<type>>(this + sw::netvars::manager->GetOffset(classname "::" varname)); \
//	sw::console::WriteColorFormat(FOREGROUND_RED, "Looked up netvar '%s', relative offset: 0x%x, instance base: 0x%x\n", classname "::" varname, res, this); \
//	return res; \
//}

#define NETVAR_OFFSET(funcname, class_name, var_name, offset, type) \
[[nodiscard]] std::add_lvalue_reference_t<type> funcname() noexcept \
{ \
    return *reinterpret_cast<std::add_pointer_t<type>>(this + sw::netvars::manager->GetOffset(class_name "::" var_name) + offset); \
}

#define NETVAR(funcname, class_name, var_name, type) \
    NETVAR_OFFSET(funcname, class_name, var_name, 0, type)

#define PNETVAR_OFFSET(funcname, classname, varname, type) \
[[nodiscard]] auto funcname() \
{ \
	return reinterpret_cast<std::add_pointer_t<type>>(this + sw::netvars::manager->GetOffset(classname "::" varname); \
}

#define PNETVAR(funcname, classname, varname, type) PNETVAR_OFFSET(funcname, classname, varname, type)