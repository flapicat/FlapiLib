#pragma once
#include "Event.h"

namespace FL
{
	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(int xOffset, int yOffset)
			:m_xOffset(xOffset), m_yOffset(yOffset)
		{
			SetCategory(EventCategory::Mouse);
		}

		int GetXoffset() const { return m_xOffset; }
		int GetYoffset() const { return m_yOffset; }
	private:
		int m_xOffset, m_yOffset;
	};
}