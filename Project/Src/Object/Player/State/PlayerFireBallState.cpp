#include "PlayerFireBallState.h"

#include "../../../Manager/Input/InputManager.h"

#include "../Wepon/FireBall/PlayerFireBallCollOperator.h"

#include "../../Common/Transform/Transform.h"




PlayerFireBallState::PlayerFireBallState(
	float COLL_START_TIME,
	float COLL_END_TIME,

	const std::vector<PlayerFireBallCollOperator*>& collOperators,

	std::function<void(void)> playAnimeAttack,
	std::function<float(void)> getAnimeRatio,

	std::function<void(void)> changeStateIdle
) :
	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),

	collOperators(collOperators),

	playAnimeAttack(playAnimeAttack),
	getAnimeRatio(getAnimeRatio),

	changeStateIdle(changeStateIdle),

	step()
{
}


void PlayerFireBallState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down) {
		OwnChangeState();
	}
}

void PlayerFireBallState::Enter(void)
{
	// ƒXƒeƒbƒv‚ğu‘OŒ„v‚Ö
	step = STEP::Startup;

	// “–‚½‚è”»’è‚ğÁ‹
	/*for (auto* op : collOperators) {
		if (op) { op->Off(); }
	}*/
	// ª‚±‚ê‚Í‚¢‚ç‚È‚¢
	//   ”­¶’†‚Ìƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ğÁ‚·‚±‚Æ‚É‚È‚é

	// UŒ‚ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
	playAnimeAttack();
}

void PlayerFireBallState::Update(void)
{
	// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡‚ğæ“¾
	const float animeRatio = getAnimeRatio();

	// ƒXƒeƒbƒv•ÊXV
	switch (step) {

	case PlayerFireBallState::STEP::Startup: {
		// ‘OŒ„

		// UŒ‚”»’è”­¶ŠJn
		if (COLL_START_TIME <= animeRatio) {

			// ƒXƒeƒbƒv‚ğuUŒ‚”»’è”­¶’†v‚Ö
			step = STEP::Active;

			// “–‚½‚è”»’è‚ğ”­¶

			// ‚â‚È‚¬ˆ—``````````````````````````````````
			//for (auto op : collOperators) {
			//	
			//	op->On();
			//}
			// ``````````````````````````````````‚â‚È‚¬ˆ—

			// ª‚±‚ê‚¾‚Æ”z—ñ‚ÉŠi”[‚³‚ê‚Ä‚¢‚éƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ğ‚·‚×‚Ä‚ğ”­¶‚Á‚Ä‚±‚Æ‚É‚È‚é
			// ‚»‚¤‚Å‚Í‚È‚­‚ÄA–¢g—pƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ğ1‚Â”­¶‚Á‚Ä‚±‚Æ‚É‚µ‚½‚¢

			// V‹Kˆ—``````````````````````````````````````

			// –¢g—pƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ªŒ©‚Â‚©‚Á‚½‚©‚Ç‚¤‚©‚ğ‹L˜^‚·‚éˆê•Ï”
			bool serch = false;

			// •ø‚¦‚Ä‚¢‚éƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ğ”ÍˆÍfor•¶‚Å’Tõ‚µ‚Ä‚¢‚­
			for (auto op : collOperators) {

				// ‚·‚Å‚Ég‚í‚ê‚Ä‚¢‚é‚Ì‚Å‚±‚Ìƒtƒ@ƒCƒA[ƒ{[ƒ‹‚Íg‚í‚È‚¢
				if (op->IsAlive()) { continue; }

				// g‚í‚ê‚Ä‚¢‚È‚¢‚à‚Ì‚ªŒ©‚Â‚©‚Á‚½‚±‚Æ‚ğ‹L˜^‚·‚é
				serch = true;

				// ”­¶
				op->On();

				// 1‚Â”­¶‚³‚¹‚ÄI—¹
				break;
			}

			// ‚·‚×‚Ä‚Ìƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ğ’Tõ‚µ‚ÄA
			// –¢g—pƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ª1‚Â‚àŒ©‚Â‚©‚ç‚È‚©‚Á‚½ê‡
			if (!serch) {

				// ‚æ‚Ù‚Ç‚Ì‚±‚Æ‚ª‚È‚¢ŒÀ‚è‚±‚Ìif•¶‚É“ü‚é‚±‚Æ‚Í‚È‚¢‚Æv‚¤‚©‚çA‚Ç‚¤‚·‚é‚©‚Í”C‚¹‚é

				// ‚Ï‚Á‚Æv‚¢‚Â‚­ˆ—‚Í2ƒpƒ^[ƒ“

				// ƒpƒ^[ƒ“1``````````````````````````

				// ¶¬¸”s‚Æ‚µ‚Ä‰½‚à‚µ‚È‚¢
				// ‚»‚Ì•ûŒü‚Ås‚­‚È‚çˆê•Ï”(serch)‚²‚ÆÁ‚µ‚Ä‘åä•v

				// ``````````````````````````ƒpƒ^[ƒ“1


				// ƒpƒ^[ƒ“2```````````````````````````````````````````

				// ƒtƒ@ƒCƒA[ƒ{[ƒ‹‚Ì¶‘¶ƒ^ƒCƒ}[(lifeTimer)‚Ì’l‚ª1”Ô’á‚¢
				// ‚Â‚Ü‚è¶¬‚µ‚Ä1”ÔŠÔ‚ªŒo‰ß‚µ‚Ä‚¢‚éƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ğã‘‚«¶¬‚·‚é

				// ã‘‚«‚·‚é”z—ñ“Yšˆê•Ï”i‰Šú’l‚ğ0‚Æ‚µ”z—ñæ“ª‚©‚ç’Tõ‚µ‚Ä‚¢‚­j
				size_t overrideIndex = 0;

				// •ø‚¦‚Ä‚¢‚éƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ğfor•¶‚Å‰ñ‚·
				for (size_t i = 1; i < collOperators.size(); i++) {
					// ”ä‚×‚Ä‚¢‚Á‚Äc‚è‚Ì¶‘¶ŠÔ‚ª1”Ô’Z‚¢ƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ğ’T‚·
					if (collOperators.at(overrideIndex)->GetLifeTimer() > collOperators.at(i)->GetLifeTimer()) {
						overrideIndex = i;
					}
				}

				// ÅI“I‚Éc‚è‚Ì¶‘¶ŠÔ‚ª1”Ô’Z‚¢ƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ğã‘‚«‚·‚é‚©‚½‚¿‚Å¶¬‚·‚é
				collOperators.at(overrideIndex)->On();

				// ```````````````````````````````````````````ƒpƒ^[ƒ“2
			}

			// ``````````````````````````````````````V‹Kˆ—
		}

		break;
	}

	case PlayerFireBallState::STEP::Active: {
		// UŒ‚”»’è”­¶’†

		// “–‚½‚è”»’è‚ğ’Ç]

		if (COLL_END_TIME <= animeRatio) {

			// ƒXƒeƒbƒv‚ğuŒãŒ„v‚Ö
			step = STEP::Recovery;

		}

		break;
	}

	case PlayerFireBallState::STEP::Recovery: {
		// ŒãŒ„

		// ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶I—¹‚Å‹­§“I‚É‘Ò‹@ó‘Ô‚Ö
		if (1.0f <= animeRatio) {

			// ‘Ò‹@ó‘Ô‚Ö‘JˆÚ
			changeStateIdle();
		}

		break;
	}

	}
}

void PlayerFireBallState::Exit(void)
{

}