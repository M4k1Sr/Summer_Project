#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

class CollisionManager;
class Camera;
class ActorBase;
struct Vector2I;

class SceneBase
{
public:
	// シーンのライフサイクル状態
	enum class STATE
	{
		Created,     // コンストラクタ直後
		Loaded,      // Load完了
		Initialized, // Init完了。Update / Draw可能
		Released,    // Release完了
	};

	// コンストラクタ
	SceneBase(void);
	// デストラクタ
	virtual ~SceneBase(void) = default;

	// コピー / ムーブ操作を禁止
	SceneBase(const SceneBase&) = delete;
	SceneBase& operator=(const SceneBase&) = delete;
	SceneBase(SceneBase&&) = delete;
	SceneBase& operator=(SceneBase&&) = delete;

	// 読み込み。派生先ではoverrideせずSubPreLoad / SubPostLoadを使う
	void Load(void);
	// 初期化。派生先ではoverrideせずSubPreInit / SubPostInitを使う
	void Init(void);
	// 更新。派生先ではoverrideせずSubPreUpdate / SubPostUpdateを使う
	void Update(void);
	// 描画。派生先ではoverrideせずSubDraw等を使う
	void Draw(void);
	// 解放。複数回呼ばれても安全
	void Release(void);

	// 現在のライフサイクル状態
	STATE GetState(void)const { return state; }

	// Update / Draw可能な状態か
	bool IsInitialized(void)const { return state == STATE::Initialized; }

	// カメラを取得する。カメラを使用しない場合はnullptrを返す。
	Camera* GetCamera(void)const { return camera; }

#pragma region シーンスタック設定

	/// <summary>
	/// 下にあるシーンも描画するかどうか（デフォルトはする）
	/// </summary>
	virtual bool IsLowerSceneDraw(void)const { return true; }

	/// <summary>
	/// 下にあるシーンも更新するかどうか（デフォルトはしない）
	/// </summary>
	virtual bool IsLowerSceneUpdate(void)const { return false; }

	/// <summary>
	/// シーン読み込み時にLoadingクラスを使用するかどうか。
	/// 重い通常シーンはtrue、軽いポーズ画面はfalseを返す。
	/// </summary>
	virtual bool IsUseLoadingScreen(void)const { return true; }

#pragma endregion

#pragma region 画面演出

	// ヒットストップ演出
	static void HitStop(int time = 20);

	// スロー演出（interフレームに1回だけゲーム更新する）
	static void Slow(int time = 10, int inter = 5);

	// 画面揺れの種類
	enum ShakeKinds
	{
		Wid,   // 横揺れ
		Hid,   // 縦揺れ
		Diag,  // 斜め揺れ
		Round, // 円を描くように揺れる
	};

	// 画面揺れの大きさ
	enum ShakeSize
	{
		Small = 3,
		Memium = 5,
		Big = 8,
	};

	/// <summary>
	/// 画面揺れを開始する。
	/// </summary>
	/// <param name="kinds">揺れ方</param>
	/// <param name="size">揺れの大きさ</param>
	/// <param name="time">揺れるフレーム数</param>
	static void Shake(ShakeKinds kinds = ShakeKinds::Diag, ShakeSize size = ShakeSize::Memium, int time = 20);

#pragma endregion

protected:


	// オブジェクトを追加する
	void ObjAdd(ActorBase* newObj);

	// オブジェクトを追加した直後に呼ばれる。派生先でoverrideして使用する。
	virtual void SubPostObjectAdd(ActorBase& actor) {}

	// シーンが所有する全てのオブジェクトに対して、引数指定の関数を実行する
	void ForEachObject(std::function<void(ActorBase&)> func) {
		for (auto& obj : objects) { func(*obj); }
	}

#pragma region 派生先の主要関数

	// 読み込み（メイン処理の前）
	virtual void SubPreLoad(void) {}
	// 読み込み（メイン処理の後）
	virtual void SubPostLoad(void) {}

	// 初期化（メイン処理の前）
	virtual void SubPreInit(void) {}
	// 初期化（メイン処理の後）
	virtual void SubPostInit(void) {}

	// 更新（メイン処理の前）
	virtual void SubPreUpdate(void) {}
	// 更新（メイン処理の後）
	virtual void SubPostUpdate(void) {}

	// 描画（メイン処理の前）
	virtual void SubPreDraw(void) {}
	// 描画（メイン処理の後）
	virtual void SubPostDraw(void) {}
	// 半透明描画（メイン処理の前）
	virtual void SubPreAlphaDraw(void) {}
	// 半透明描画（メイン処理の後）
	virtual void SubPostAlphaDraw(void) {}
	// UI描画（メイン処理の前）
	virtual void SubPreUiDraw(void) {}
	// UI描画（メイン処理の後）
	virtual void SubPostUiDraw(void) {}

	// 解放（メイン処理の前）
	virtual void SubPreRelease(void) {}
	// 解放（メイン処理の後）
	virtual void SubPostRelease(void) {}

#pragma endregion

	// カメラを使用するかどうか
	virtual bool UseCamera(void)const { return true; }

	// 当たり判定管理クラスを使用するかどうか
	virtual bool UseCollisionManager(void)const { return true; }

private:

	// 二重Load / 二重Releaseを防止する「状態」
	STATE state;

	// 当たり判定管理
	CollisionManager* collision;

	// ゲーム更新を実行するフレームか判定する
	bool IsUpdateFrame(void);
	// 画面揺れによる描画位置を値で返す
	Vector2I GetShakePoint(void);

#pragma region 画面演出用

	static int hitStop;
	static int slow;
	static int slowInter;
	static int slowCounter;

	static int shake;
	static ShakeKinds shakeKinds;
	static ShakeSize shakeSize;

	// 画面揺れを適用するためのメインスクリーン
	int mainScreen;

#pragma endregion

protected:

	// カメラ
	Camera* camera;

	// Actor格納用配列
	std::vector<ActorBase*> objects;
};

using ShakeKinds = SceneBase::ShakeKinds;
using ShakeSize = SceneBase::ShakeSize;