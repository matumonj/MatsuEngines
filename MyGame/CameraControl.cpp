
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
#include"Field.h"
CameraControl* CameraControl::GetInstance()
{
	static CameraControl instance;
	return &instance;
}


/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void CameraControl::Finalize()
{
	Destroy(camera);
	Num.clear();
	pos.clear();
	points.clear();
}


/*------------------------*/
/*--------読込処理---------*/
/*----------csv-----------*/
void CameraControl::ParamSet()
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

void CameraControl::Init_Tutorial(DebugCamera* camera)
{
	ParamSet();
	
	dis.x = sinf(angle * (PI / 180)) * 30.0f;
	dis.y = cosf(angle * (PI / 180)) * 30.0f;
	distance.x = dis.x;
	distance.y = dis.y;
}

void CameraControl::Init_Play(DebugCamera* camera)
{
	ParamSet();

	this->camera = new DebugCamera(WinApp::window_width, WinApp::window_height);//(/*input*/);
	input = Input::GetInstance();

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


	dis.x = sinf(angle * (PI / 180)) * 30.0f;
	dis.y = cosf(angle * (PI / 180)) * 30.0f;
	distance.x = dis.x;
	distance.y = dis.y;
}
void CameraControl::Init_Boss(DebugCamera* camera)
{
	ParamSet();
	this->camera = new DebugCamera(WinApp::window_width, WinApp::window_height);//(/*input*/);
	input = Input::GetInstance();


	dis.x = sinf(angle * (PI / 180)) * 30.0f;
	dis.y = cosf(angle * (PI / 180)) * 30.0f;
	distance.x = dis.x;
	distance.y = dis.y;
}
void CameraControl::Load(DebugCamera* camera)
{
	
}


/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/

void CameraControl::Update_Tutorial(DebugCamera* camera)
{
	AngleRotation();
	this->camera->Update();
	//TargetPlayer();
	(this->*targetTable[Tstate])();
}

void CameraControl::Update_Play(DebugCamera* camera)
{
	AngleRotation();

	this->camera->Update();

	TargetPlayer();
	//Feed::GetInstance()->Update_White(Feed::FEEDOUT);
	(this->*targetTable[Tstate])();
}

void CameraControl::Update_Boss(DebugCamera* camera)
{
	AngleRotation();

	this->camera->Update();

	(this->*targetTable[Tstate])();
}

/*------------------------*/
/*--------カメラ回転---------*/
/*------------------------*/
void CameraControl::AngleRotation()
{


}

/*------------------------*/
/*--------ターゲット---------*/
/*---------player---------*/
void CameraControl::TargetPlayer()
{
	EncountFlag = false;
			//OldCameraPos = camera->GetEye();
			//OldCameratarget = camera->GetTarget();
		//	encountGuardian = START;
		//	Tstate=ENCOUNTGUARDIAN;
		//}
		//else {
			if (AttackSceneF) {
				Feed::GetInstance()->Update_White(Feed::FEEDOUT);
				if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
					AttackSceneF = false;
				}
			}
			
			this->camera->SetTarget({ PlayerControl::GetInstance()->GetPlayer()->GetPosition() });
			if (input->TiltPushStick(Input::R_RIGHT) || input->TiltPushStick(Input::R_LEFT)) {
				if (input->TiltPushStick(Input::R_RIGHT)) {
					angle += 1;
					charaAngle += 1.0f;
					PlayerControl::GetInstance()->GetPlayer()->Setangle(charaAngle);
				
			}
				if (input->TiltPushStick(Input::R_LEFT)) {
					angle -= 1;
					charaAngle -= 1.0f;
					PlayerControl::GetInstance()->GetPlayer()->Setangle(charaAngle);
				}
				dis.x = sinf(angle * (PI / 180)) * 30.0f;
				dis.y = cosf(angle * (PI / 180)) * 30.0f;
				distance.x = dis.x;
				distance.y = dis.y;

			}
			XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
			//	camera->SetTarget(player_shadow->GetCameraPos(angle));
			CameraPosition = { ppos.x + distance.x,ppos.y + 10.0f,ppos.z + distance.y };
			camera->SetEye(CameraPosition);

				if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
				if (ChestControl::GetInstance()->ChestCount() >= 5) {
					Tstate = MOVEBOSSAREA;
				}
			}
		//}
}

/*------------------------*/
/*--------ターゲット---------*/
/*---------field---------*/
void CameraControl::TargetBossField()
{
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

void (CameraControl::* CameraControl::targetTable[])() = {
	nullptr,
		&CameraControl::TargetPlayer,
		nullptr,
		&CameraControl::GuardianBattleStart,
		&CameraControl::TargetBossField,
		&CameraControl::PlaySceneStart,
		&CameraControl::BossSceneStart
};

/*------------------------*/
/*--------カメラ位置--------*/
/*---------spline---------*/
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



/*------------------------*/
/*--------ターゲット-------*/
/*---------boss---------*/
void CameraControl::BossSceneStart()
{
	XMFLOAT3 BossPos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
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
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetFbxTime() > 2.0f) {
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

/*------------------------*/
/*--------ターゲット---------*/
/*---------field---------*/
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
		if (Collision::GetLength(camera->GetEye(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 30) {
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

/*------------------------*/
/*--------シェイク---------*/
/*------------------------*/
void CameraControl::ShakeCamera()
{
	if (shaketime == 0) {

		shaketime = 10;
	}
	if (shaketime != 0) {
		shake = rand() % 4 - 8;
		shakex = rand() % 4 - 8;
		shakey = rand() % 4 - 8;
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

Enemy* CameraControl::GuardianEnemy()
{
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	Enemy* encountEnemy;
	EnemyControl::EnemyType etype;
	//エンカウントする範囲
	const float range = 300.0f;
	//エンカウント範囲入ったら
	
	
	//for (int i = 0; i < 4) {
	//	if (EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetHP() <= 0) {
		//	continue;
		//}
		// if(Collision::GetLength(EnemyControl::GetInstance()->GetEnemyindex0()[i]->GetPosition(), pPos) < range){
		// EncountFlag=true;
		// }
		// else{
		// EncountFlag=false;
		// }
		// if (EncountFlag) {
			//return EnemyControl::GetInstance()->GetEnemyindex(0)[i].get();
		//}
		// else{
		// i++;
		// }
		// 
		//EncountJudg = Collision::GetLength(EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetPosition(), pPos) < range;
	//}
	if (EncountFlag) {
		return EnemyControl::GetInstance()->GetEncountEnemy();
	}

}
void CameraControl::GuardianBattleStart()
{
	switch (encountGuardian)
	{
	case CameraControl::START:
		EncountGuardian_Start();
		break;
	case CameraControl::BATTLE:
		EncountGuardian_Battle();
		break;
	case CameraControl::END:
		EncountGuardian_End();
		break;
	default:
		break;
	}
}

void CameraControl::EncountGuardian_Start()
{
	PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);

	XMFLOAT3 PlayerPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	XMFLOAT3 EnemyPos = GuardianEnemy()->GetPosition();

	XMFLOAT3 cameraPos = { Easing::EaseOut(EaseTime_EnGuar_pos.x, OldCameraPos.x, PlayerPos.x+(EnemyPos.x-PlayerPos.x)/2),
						   Easing::EaseOut(EaseTime_EnGuar_pos.y, OldCameraPos.y, PlayerPos.y + (EnemyPos.y - PlayerPos.y)/2) ,
						   Easing::EaseOut(EaseTime_EnGuar_pos.z, OldCameraPos.z, PlayerPos.z + (EnemyPos.z - PlayerPos.z)/2)
						};

	XMFLOAT3 cameraTarget = {
						   Easing::EaseOut(EaseTime_EnGuar_target.x, OldCameratarget.x, EnemyPos.x),
						   Easing::EaseOut(EaseTime_EnGuar_target.y, OldCameratarget.y, EnemyPos.y) ,
						   Easing::EaseOut(EaseTime_EnGuar_target.z, OldCameratarget.z, EnemyPos.z) 
						};

	{//ターゲット
		if (EaseTime_EnGuar_target.x < 1.0f) {
			EaseTime_EnGuar_target.x += 0.01f;
			EaseTime_EnGuar_target.y += 0.01f;
			EaseTime_EnGuar_target.z += 0.01f;
		}
	}
	{//カメラ位置
		if (EaseTime_EnGuar_pos.x < 1.0f) {
			EaseTime_EnGuar_pos.x += 0.01f;
			EaseTime_EnGuar_pos.y += 0.01f;
			EaseTime_EnGuar_pos.z += 0.01f;
		}
	}
	camera->SetTarget(cameraTarget);
	camera->SetEye(cameraPos);

	if (EaseTime_EnGuar_pos.x >= 1.0f && EaseTime_EnGuar_target.x >= 1.0f) {
		waitCount++;
		if (waitCount > 120) {
			encountGuardian = BATTLE;
		}
	}
}

void CameraControl::EncountGuardian_Battle()
{
	waitCount = 0;
	XMFLOAT3 PlayerPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	XMFLOAT3 EnemyPos = GuardianEnemy()->GetPosition();

	XMFLOAT3 cameraPos = { Easing::EaseOut(EaseTime_EnGuar_pos.x, OldCameraPos.x, PlayerPos.x + (EnemyPos.x - PlayerPos.x) / 2),
						   Easing::EaseOut(EaseTime_EnGuar_pos.y, OldCameraPos.y, PlayerPos.y + (EnemyPos.y - PlayerPos.y) / 2) ,
						   Easing::EaseOut(EaseTime_EnGuar_pos.z, OldCameraPos.z, PlayerPos.z + (EnemyPos.z - PlayerPos.z) / 2)
	};

	XMFLOAT3 cameraTarget = {
						   Easing::EaseOut(EaseTime_EnGuar_target.x, OldCameratarget.x, EnemyPos.x),
						   Easing::EaseOut(EaseTime_EnGuar_target.y, OldCameratarget.y, EnemyPos.y) ,
						   Easing::EaseOut(EaseTime_EnGuar_target.z, OldCameratarget.z, EnemyPos.z)
	};

	{//ターゲット
		if (EaseTime_EnGuar_target.x > 0.0f) {
			EaseTime_EnGuar_target.x -= 0.01f;
			EaseTime_EnGuar_target.y -= 0.01f;
			EaseTime_EnGuar_target.z -= 0.01f;
		}
	}
	{//カメラ位置
		if (EaseTime_EnGuar_pos.x >0.0f) {
			EaseTime_EnGuar_pos.x -= 0.01f;
			EaseTime_EnGuar_pos.y -= 0.01f;
			EaseTime_EnGuar_pos.z -= 0.01f;
		}
	}
	camera->SetTarget(cameraTarget);
	camera->SetEye(cameraPos);

	if (EaseTime_EnGuar_pos.x <= 0.0f && EaseTime_EnGuar_target.x <= 0.0f) {
			encountGuardian = END;
	}
}

void CameraControl::EncountGuardian_End()
{

	EncountFlag = false;
	//移動可能に
	PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(false);
	Tstate = PLAYER;
}


/*更新処理*/
void CameraControl::Draw_Tutorial(){}

void CameraControl::Draw_Play() {}

void CameraControl::Draw_Boss() {}