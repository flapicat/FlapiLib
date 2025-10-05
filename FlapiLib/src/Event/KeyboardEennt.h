#pragma once
#include "Event.h"

namespace FL
{
	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(int keycode)
			:m_KeyCode(keycode)
		{
			SetCategory(EventCategory::KeyBoard);
		}

		int GetKeyCode() const { return m_KeyCode; };
	private:
		int m_KeyCode;
	};
}