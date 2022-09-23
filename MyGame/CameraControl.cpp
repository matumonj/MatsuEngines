
#include "CameraControl.h"
#include<windows.h>
#include"Feed.h"
#include"SceneManager.h"
#include"PlayerControl.h"
#include"ChestControl.h"
#include"EnemyControl.h"
#include"imgui.h"
#include"mHelper.h"
#include"UI.h"
CameraControl* CameraControl::GetInstance()
{
	static CameraControl instance;
	return &instance;
}

void CameraControl::Finalize()
{
	Destroy(camera);
	Num.clear();
	pos.clear();
	points.clear();
}

void CameraControl::Initialize(DebugCamera* camera)
{
	startindex = 1.0f;
	startCount = 0.0f;
	nowCount = 0.0f;
	elapsedCount = 0.0f;

	EaseTime = 0.0f;
	shakex = 0.0f;
	shakey = 0.0f;
	shake = 0.0f;
	shaketime = 0.0f;

	maxtime = 6.0f;
	time = 0.00000f;
	j = time;
	elapsedTime = 0.0f;
	bCamera = BOSSCUTSTART;
	sCamera = PLAYCUTSTART;
	mCamera = NON;
	Tstate = PLAYER;

	this->camera = new DebugCamera(WinApp::window_width, WinApp::window_height);//(/*input*/);
	input = Input::GetInstance();
}

void CameraControl::Load(DebugCamera* camera)
{
	file.open("Param_CSV/CameraPoints.csv");

	popcom << file.rdbuf();

	file.close();

	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("Points_Quantity") == 0) {
			std::getline(line_stream, word, ',');
			int quantity = (int)std::atof(word.c_str());
			Quantity = quantity;
			break;
		}
	}
	Num.resize(Quantity);
	pos.resize(Quantity);
	for (int i = 0; i < Quantity; ++i) {
		while (std::getline(popcom, line)) {
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}
			if (word.find("POP") == 0) {
				std::getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());

				std::getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());

				std::getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str());

				pos[i] = { x,y,z };
				break;
			}
		}
	}
	points.push_back(pos[0]);
	points.push_back(pos[0]);
	for (int i = 1; i < Quantity - 1; i++) {
		points.push_back(pos[i]);
	}
	int quantity_end = Quantity - 1;
	points.push_back(pos[quantity_end]);
	points.push_back(pos[quantity_end]);

	//Load_ChestPosition.resize(Quantity);
	this->camera->SetEye(pos[0]);
	UpdateRange = 200;
	startCount = GetTickCount64();
}

void CameraControl::Update(DebugCamera* camera)
{
	if (input->Pushkey(DIK_RIGHT)) {
		charaAngle += 0.5f;
		cameraAngle -= 0.5f;
		PlayerControl::GetInstance()->GetPlayer()->SetCharaRotation(charaAngle);
	} else if (input->Pushkey(DIK_LEFT) || input->RightTiltStick(input->Left)) {
		cameraAngle += 0.5f;
		charaAngle -= 0.5f;
		PlayerControl::GetInstance()->GetPlayer()->SetCharaRotation(charaAngle);
	}
	if (cameraAngle >= 360 + 90 || cameraAngle <= -360) {
		cameraAngle = 0;
	}

	this->camera->Update();

	if (Tstate == PLAYER) {
		if (AttackSceneF) {
			Feed::GetInstance()->Update_White(Feed::FEEDOUT);
			if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
				AttackSceneF = false;
			}
		}
		CameraPosition.x = PlayerControl::GetInstance()->GetPlayer()->GetPosition().x + cosf((float)(cameraAngle) * 3.14f / 180.0f) * 25;
		CameraPosition.z = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z + sinf((float)(cameraAngle) * 3.14f / 180.0f) * 35;
		CameraPosition.y = PlayerControl::GetInstance()->GetPlayer()->GetPosition().y + CameraHeight;
		this->camera->SetTarget({ PlayerControl::GetInstance()->GetPlayer()->GetPosition() });
		
		this->camera->SetEye(CameraPosition);
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			if (ChestControl::GetInstance()->ChestCount() >= 5) {
				Tstate = MOVEBOSSAREA;
			}
		}
	}
	
	else if (Tstate == MOVEBOSSAREA) {
			switch (mCamera)
			{
			case NON:
				countAreaMove = 0;
				mCamera = SHAKESTART;
				break;
			case SHAKESTART:
				ShakeCamera();
				countAreaMove++;
				if (countAreaMove >= 120) {
					mCamera = YPOSUP;
				}
				OldCameraPosY = CameraPosition.y;
				break;
			case YPOSUP:
				ShakeCamera();
				countAreaMove = 0;
				EaseTime += 1.0f / 120.0f;
				CameraPosition.y = Easing::EaseOut(EaseTime, OldCameraPosY, OldCameraPosY + 50);
				if (EaseTime >= 1.0f) {
					mCamera = TARGETPLAYER;
				}
				break;
			case TARGETPLAYER:
				ShakeCamera();
				EaseTime = 0.0f;
				countAreaMove++;
				if (countAreaMove >= 120) {
					Feed::GetInstance()->Update_White(Feed::FEEDIN);
				}
				break;
			default:
				break;
			}
			this->camera->SetTarget({ PlayerControl::GetInstance()->GetPlayer()->GetPosition() });
	}


	else if (Tstate == BOSSCUTSCENE) {
		BossSceneStart();
	}
	else if (Tstate == PLAYCUTSCENE) {
		
		PlaySceneStart();
	}
}

XMFLOAT3 CameraControl::SplinePosition(const std::vector<XMFLOAT3>& points, size_t startindex, float t)
{
	size_t n = points.size() - 2;
	if (startindex > n)return points[n];
	if (startindex < 1)return points[1];

	XMFLOAT3 p0 = points[startindex - 1];
	XMFLOAT3 p1 = points[startindex];
	XMFLOAT3 p2 = points[startindex + 1];
	XMFLOAT3 p3 = points[startindex + 2];

	XMFLOAT3 a, b, c, d, e, f, g, h, i;
	XMFLOAT3 a1, b1, c1, d1, b4;
	float ext, ext2;
	ext = (t * t);
	ext2 = (t * t * t);
	a = XMFLOAT3(p1.x * 2, p1.y * 2, p1.z * 2);
	b = XMFLOAT3((-p0.x + p2.x) * t, (-p0.y + p2.y) * t, (-p0.z + p2.z) * t);
	c = XMFLOAT3(2 * p0.x, 2 * p0.y, 2 * p0.z);
	d = XMFLOAT3(5 * p1.x, 5 * p1.y, 5 * p1.z);
	e = XMFLOAT3(4 * p2.x, 4 * p2.y, 4 * p2.z);
	f = XMFLOAT3(-p0.x, -p0.y, -p0.z);
	g = XMFLOAT3(3 * p1.x, 3 * p1.y, 3 * p1.z);
	h = XMFLOAT3(3 * p2.x, 3 * p2.y, 3 * p2.z);

	//1ぎょうめ
	a1 = { a.x + b.x,a.y + b.y,a.z + b.z };
	//2
	b1 = { c.x - d.x,c.y - d.y ,c.z - d.z };
	c1 = { (b1.x + e.x - p3.x) * ext ,(b1.y + e.y - p3.y) * ext  ,(b1.z + e.z - p3.z) * ext };

	d1 = { (f.x + g.x - h.x + p3.x) * ext2,(f.y + g.y - h.y + p3.y) * ext2,(f.z + g.z - h.z + p3.z) * ext2 };
	b4 = { (a1.x + c1.x),(a1.y + c1.y)  ,(a1.z + c1.z) };
	XMFLOAT3 ys;
	ys = { b4.x + d1.x,b4.y + d1.y ,b4.z + d1.z };
	XMFLOAT3 position = { 0.5f * ys.x,0.5f * ys.y ,0.5f * ys.z };

	return position;
}

void CameraControl::Draw()
{
}

void CameraControl::SetColor(XMFLOAT4 color)
{

}

void CameraControl::BossSceneStart()
{
	XMFLOAT3 BossPos = EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition();
	switch (bCamera)
	{
	case BOSSCUTSTART:
		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);

		Feed::GetInstance()->Update_White(Feed::FEEDOUT);
		
		CutCount[0]++;
		if (CutCount[0] > 190) {
			bCamera = ROLL;
		}
		
		break;
	case ROLL:
		CutCount[0] = 0;
		
		if (BossCutAngle >= 270.0f+360.0f) {
			bCamera = BOSSCUTEND;
		}
		else {
			BCutCameraHeight += 0.01f;
			BossCutAngle += XMConvertToRadians(45);
		}
		break;
	case BOSSCUTEND:
		AttackSceneF = true;
		if (EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetFbxTime() > 2.0f) {
			CameraDis += 1.5f;
		}
		Feed::GetInstance()->Update_White(Feed::FEEDIN);
		if (Feed::GetInstance()->GetAlpha() >= 0.9f) {
			PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(false);

			Tstate = PLAYER;
		}

		break;
	default:
		break;
	}
	CameraPosition.x = BossPos.x + cosf((float)(BossCutAngle) * 3.14f / 180.0f) * 15;
	CameraPosition.y = BossPos.y + BCutCameraHeight;

	CameraPosition.z = BossPos.z + sinf((float)(BossCutAngle) * 3.14f / 180.0f) * CameraDis;

	camera->SetTarget({ BossPos.x, BossPos.y + BCutCameraHeight, BossPos.z });
	camera->SetEye(CameraPosition);
}


void CameraControl::PlaySceneStart()
{
	switch (sCamera)
	{
	case PLAYCUTSTART:
		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);

		UI::GetInstance()->SetTurnoffUIDraw(true);
		Feed::GetInstance()->Update_White(Feed::FEEDOUT);

		if (Feed::GetInstance()->GetAlpha() <= 0.9f) {
			sCamera = SPLINE;
		}
		break;
	case SPLINE:
		
		nowCount = GetTickCount64();
		elapsedCount = nowCount - startCount;
		 elapsedTime = static_cast<float>(elapsedCount) / 1000.0f;

		timerate = elapsedTime / maxtime;
		if (timerate >= 1) {
			if (startindex < points.size() - 3)
			{
				startindex++;
				timerate -= 1;

				startCount = GetTickCount64();
			} else
			{
				timerate = 1;
			}
		}

		camera->SetEye(SplinePosition(points, startindex, timerate));

		//カメラが一定距離近づいたらフェード
		if (Collision::GetLength(camera->GetEye(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 50) {
			Feed::GetInstance()->Update_Black(Feed::FEEDIN);
		}
		else {
			Feed::GetInstance()->Update_Black(Feed::FEEDOUT);
		}
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {
			sCamera = PLAYCUTEND;
		}
		break;

	case PLAYCUTEND:
		this->camera->SetEye(CameraPosition);
		Feed::GetInstance()->Update_Black(Feed::FEEDOUT);
		if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
			PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(false);

			UI::GetInstance()->SetTurnoffUIDraw(false);
			Tstate = PLAYER;
		}
		break;
	default:
		break;
	}
	this->camera->SetTarget({ PlayerControl::GetInstance()->GetPlayer()->GetPosition() });

}

void CameraControl::ShakeCamera()
{
	if (shaketime == 0) {

		shaketime = 40;
	}
	if (shaketime != 0) {
		shake = rand() % 3 - 6;
		shakex = rand() % 3 - 6;
		shakey = rand() % 3 - 6;
		shakex -= shake;
		shakey -= shake;
		shaketime--;
		//}
		//シェイク値を０に
	} else if (shaketime == 0) {
		shakex = 0;
		shakey = 0;
	}
	camera->SetEye({ CameraPosition.x+shakex,	CameraPosition.y + shakey,CameraPosition.z });
}