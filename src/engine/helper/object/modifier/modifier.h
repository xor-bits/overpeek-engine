#pragma once

#include <string>



namespace oe::simple {

	class Object;
	class Modifier {
	protected:
		std::string m_modifier_name;
		Object* m_gameobject;
		static const float timeframe;

	public:
		void __modifierInit(Object* host_gameobject) { m_gameobject = host_gameobject; }

		virtual void OnPreInit();
		virtual void OnInit();
		virtual void OnPostInit();

		virtual void OnPreUpdate();
		virtual void OnUpdate();
		virtual void OnPostUpdate();
		
		virtual void OnCleanup();
		virtual void OnDestroy();

		virtual void OnDraw();

	};

}