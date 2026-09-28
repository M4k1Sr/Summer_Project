#include "EnemyManager.h"

#include "EnemyBase.h"

#include "NormalSlime/NormalSlime.h"
#include "Cactus/Cactus.h"
#include "Thorn/Thorn.h"


EnemyManager::EnemyManager() :
	ActorBase("Data/Parameter/Enemy/")
{
}

void EnemyManager::Load(void)
{
	// スライム生成
	for (int i = 1; ; i++) {
		// その回のパラメーター名
		std::string parameterName = "SlimePos" + std::to_string(i);

		// そのパラメータが存在するかどうか
		if (!IsParameterExist("InitPos", parameterName)) { break; }

		// 存在するならば、その座標にその敵を生成
		AddChildActor(new NormalSlime(GetParameterToVector3("InitPos", parameterName)));
	}

	// サボテン生成
	for (int i = 1; ; i++) {
		// その回のパラメーター名
		std::string parameterName = "CactusPos" + std::to_string(i);

		// そのパラメータが存在するかどうか
		if (!IsParameterExist("InitPos", parameterName)) { break; }

		// 存在するならば、その座標にその敵を生成
		AddChildActor(new NormalSlime(GetParameterToVector3("InitPos", parameterName)));
	}

	// とげ生成
	for (int i = 1; ; i++) {
		// その回のパラメーター名
		std::string parameterName = "ThornPos" + std::to_string(i);

		// そのパラメータが存在するかどうか
		if (!IsParameterExist("InitPos", parameterName)) { break; }

		// 存在するならば、その座標にその敵を生成
		AddChildActor(new Thorn(GetParameterToVector3("InitPos", parameterName)));
	}
}
void EnemyManager::SetPlayerPos(const Vector3* pos)
{
	for (ActorBase* child : GetChildActors()) {

		if (Thorn* thorn = dynamic_cast<Thorn*>(child)) {

			thorn->SetPlayerPos(pos);

			continue;
		}

		if (Cactus* cactus = dynamic_cast<Cactus*>(child)) {

			cactus->SetPlayerPos(pos);

			continue;
		}
	}
}
