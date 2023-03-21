#pragma once
#include"BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"
#include"LightGroup.h"
#include"DebugCamera.h"
#include"DirectXCommon.h"

class TitleScene : public BaseScene
{
public:
	TitleScene(SceneManager* sceneManager);
private:
	//テクスチャ[GolemFantasy]
	std::unique_ptr<Sprite> titlesprite = nullptr;
	//フィールドオブジェクト
	std::unique_ptr<Object3d> field = nullptr;
	//天球オブジェクト
	std::unique_ptr<Object3d> celestal = nullptr;
	//テクスチャ[Bでゲーム画面へ]
	std::unique_ptr<Sprite> TitleMenu = {nullptr};
	//ロードシーン
	std::unique_ptr<Sprite> LoadMenuSprite = { nullptr };

	//読み込み中と完了の文字
	struct NowLoadSprite {
		std::unique_ptr<Sprite> LWordSprite = { nullptr };
		XMFLOAT2 Pos;
		float Color;
	};
	std::array<NowLoadSprite, 10>LoadWords;
	//タイトル用カメラ
	std::unique_ptr<DebugCamera> camera = nullptr;

	//チュートリアルボタン押されたか？
	bool menujudg_Play = false;
	//フェード演出用
	bool feedf = false;
	//カメラが後ろに下がる
	bool CameraBackF = false;

	//ボタンテクスチャの座標、スケール、アルファ値
	float menuAlpha = 1.0;
	XMFLOAT2 MenuScale = {0.0f, 0.0f};
	XMFLOAT2 MenuPos = {0.0f, 0.0f};

	//カメラの回転速度
	float CamAngleSpeed;
	float Cangle = 0.0f;
	XMFLOAT2 CameraPos = {0.0f, 0.0f};
	//フィールド回転用
	float FieldRotY = 0.0f;
	bool FadeFlag;
public:
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//開放
	void Finalize() override;
private:
	//ライト更新
	void LightUpdate() override;
	//描画＿オブジェクト
	void MyGameDraw() override;
	//描画＿スプライト
	void SpriteDraw() override;

	bool ff;
	float timef;
private:
	static constexpr float RotSpeed = 0.1f;
	static constexpr XMFLOAT3 FieldScl = {0.15f, 0.15f, 0.15f};
	static constexpr XMFLOAT3 CelestalScl = {30.f, 30.1f, 30.1f};

	//テクスチャの更新と描画
	void TitleTexInit();
	void TitleTexUpda();
	void TitleTexDraw();

	//フィールドオブジェクトの更新と描画
	bool ChangeScene();
	void TitleFieldInit();
	void TitleFieldUpda();
};
