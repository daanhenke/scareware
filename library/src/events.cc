#include "events.hh"
#include "interfaces.hh"

sw::events::SimpleEventListener::~SimpleEventListener()
{

}

void sw::events::SimpleEventListener::FireGameEvent(iface::IGameEvent* event)
{
	if (!event)
	{
		console::WriteFormat("null event???????????????????????!\n");
	}

	MessageBoxA(nullptr, "Hi", "hi", MB_OK);
	//auto eventName = event->GetName();

	//if (m_callbacks.contains(eventName))
	//{
	//	console::WriteFormat("Triggering event: '%s'\n", eventName);
	//	m_callbacks[eventName](event);
	//}
}

void sw::events::SimpleEventListener::AddCallback(const char* name, SimpleEventListenerCallback callback)
{
	m_callbacks[name] = callback;
	console::WriteColorFormat(FOREGROUND_GREEN, "new event listener for event '%s': 0x%x\n", name, callback);
	interfaces::IGameEventManager2->AddListener(this, name);
}

#include "console.hh"
void sw::events::OnPlayerHurt(iface::IGameEvent* event)
{
	console::WriteFormat("Someone got shot!\n");
}

#include <memory>
std::unique_ptr<sw::events::SimpleEventListener> event_listener;
void sw::events::AddListeners()
{
	event_listener = std::make_unique<SimpleEventListener>();

	event_listener->AddCallback("player_hurt", OnPlayerHurt);
}

void sw::events::UndoListeners()
{
	interfaces::IGameEventManager2->RemoveListener(event_listener.get());
}