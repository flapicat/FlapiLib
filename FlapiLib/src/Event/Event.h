#pragma once

namespace FL
{
	enum EventCategory
	{
		KeyBoard,
		Mouse,
		Application
	};

	enum EventType
	{
		WindowClose, WindowResize,
		KeyPressed,
		MouseScroll,
	};

	class Event
	{
	public:
		virtual ~Event() = default;

		bool IsInCategory(EventCategory category) { return m_Category == category; };
		void SetCategory(EventCategory category) { m_Category = category; };

		bool Handled = false;
		EventType m_Type;
		EventCategory m_Category;
	};

	class EventHandler
	{
	public:
		EventHandler(Event& event)
			: m_Event(event) {
		}

		template<typename T, typename F>
		bool Handle(const F& func)
		{
			if (T* casted = dynamic_cast<T*>(&m_Event))
			{
				func(*casted);
				m_Event.Handled = true;
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
}