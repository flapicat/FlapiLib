#pragma once
#include "Event.h"

namespace FL
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent()
		{
			SetCategory(EventCategory::Application);
		}
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			:m_width(width), m_height(height)
		{
			SetCategory(EventCategory::Application);
		}

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
	private:
		int m_width, m_height;
	};
}