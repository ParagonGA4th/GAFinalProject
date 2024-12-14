//#pragma once
//#include "ScriptInterface.h"
//
//#include <vector>
//
//namespace Pg::DataScript
//{
//	class ArrowLogic;
//}
//
//namespace Pg::API
//{
//	namespace Input
//	{
//		class PgInput;
//	};
//}
//
///// <summary>
///// 현재로서는 자기 자신이 Player여야 동작한다.
///// </summary>
//
//namespace Pg::DataScript
//{
//	class ArrowShooterBehavior : public ScriptInterface<ArrowShooterBehavior>
//	{
//		DEFINE_PARAGON_SCRIPT(ArrowShooterBehavior);
//
//	public:
//		ArrowShooterBehavior(Pg::Data::GameObject* obj);
//
//		virtual void Start() override;
//		virtual void Update() override;
//
//	private:
//		Pg::API::Input::PgInput* _pgInput;
//		std::vector<ArrowLogic*> _arrowVec;
//	};
//}
//
//
//
