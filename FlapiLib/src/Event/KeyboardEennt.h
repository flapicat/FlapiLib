#pragma once
#include "Event.h"

namespace FL
{
	class KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_KeyCode; }
	protected:
		KeyEvent(const int keycode)
			: m_KeyCode(keycode) {
		}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const int keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {
			SetCategory(EventCategory::KeyBoard);
			SetType(EventType::KeyPressed);
		}

		bool IsRepeat() const { return m_IsRepeat; }

	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const int keycode)
			: KeyEvent(keycode) {
			SetCategory(EventCategory::KeyBoard);
			SetType(EventType::KeyReleased);
		}

	};

}