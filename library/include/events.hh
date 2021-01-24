#pragma once

#include <string>
#include <map>
#include "iface/IGameEventListener2.hh"

namespace sw::events
{
	using SimpleEventListenerCallback = void (*)(iface::IGameEvent* event);
	class SimpleEventListener : public iface::IGameEventListener2
	{
	public:
		~SimpleEventListener();
		void FireGameEvent(iface::IGameEvent* event) override;
		void AddCallback(const char* name, SimpleEventListenerCallback callback);
	protected:
		std::map<const char*, SimpleEventListenerCallback> m_callbacks;
	};

	void OnPlayerHurt(iface::IGameEvent* event);

	void AddListeners();
	void UndoListeners();
}