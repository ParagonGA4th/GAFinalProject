#include "ArrowShooterBehavior.h"
#include "ArrowLogic.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	ArrowShooterBehavior::ArrowShooterBehavior(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void ArrowShooterBehavior::Start()
	{
		//ArrowLogic의 Awake에서 자신의 Tag를 이미 "TAG_Arrow"로 설정해놨었을 것이다.
		//모든 Arrow들 받아오기. 한 30개는 되어야 빈틈을 눈치를 못 챌 것이다.
		std::vector<Pg::Data::GameObject*> allObjects = _object->GetScene()->FindObjectsWithTag("TAG_Arrow");
		for (auto& it : allObjects)
		{
			auto tALogic = it->GetComponent<Pg::DataScript::ArrowLogic>();
			if (tALogic != nullptr)
			{
				_arrowVec.push_back(tALogic);
			}
		}
	}

	void ArrowShooterBehavior::Update()
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyN))
		{
			bool tDidShoot = false;

			for (int i = 0; i < _arrowVec.size(); i++)
			{
				//지금 쏘고 있지 않은 컴포넌트만 허용해야.
				if (!(_arrowVec[i]->GetIsNowShooting()))
				{
					using namespace Pg::Math;
					//우리 Forward랑 다른 로직이 된 것 같다. 그러니, Forward를 Rotation을 갖고 Custom으로 구해주자.
					//PlayerBehavior랑 같은 위치. -> 나중에 PhysX 연동은 고쳐져야!
					
					//Z축 향해 뒤집기. 어디에서 불완전한 연결이 일어나는지는 확인해봐야 할 것 같다.
					Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGReflectVectorAgainstAxis(_object->_transform.GetForward(), { 0,0,1 });
					tShouldShootDir = Pg::Math::PGFloat3Normalize(tShouldShootDir);
						
					float tDistanceToSpawnFrom = 3.0f;
					_arrowVec[i]->ShootArrow(_object->_transform._position + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
					tDidShoot = true;
					break;
				}
			}

			if (!tDidShoot)
			{
				//쐈어야 하는데 기존의 것 중에서 준비되어 있는 오브젝트가 존재하지 않는다!
				//기존 것 중에서 오래된 요소를 찾아서, (FixedSizeQueue) 여기 기준으로 마지막을 원래대로 돌려야 한다.
				//여튼, 그건 나중에 할 일.
				PG_TRACE("아직 충분히 반환되지 않음. 나중에 FixedSizeQueue로?");
			}
		}
	}

}