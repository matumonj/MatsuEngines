#include "CameraControl.h"
#include <algorithm>

#include "AttackCollision.h"
#include"Feed.h"
#include"SceneManager.h"
#include"PlayerControl.h"
#include"ChestControl.h"
#include"EnemyControl.h"
#include"mHelper.h"
#include"UI.h"

#include "FenceControl.h"

CameraControl* CameraControl::GetIns()
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
	startindex = static_cast<size_t>(1.0f);
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
	mCamera = NON; //NON;
	Tstate = PLAYER;
	this->camera = new DebugCamera(WinApp::window_width, WinApp::window_height); //(/*input*/);
	input = Input::GetIns();
}

void CameraControl::Init_Tutorial()
{
	ParamSet();

	dis.x = sinf(angle * (PI / 180.0f)) * 30.0f;
	dis.y = cosf(angle * (PI / 180.0f)) * 30.0f;
	distance.x = dis.x;
	distance.y = dis.y;
}

void CameraControl::Init_Play()
{
	ParamSet();

	this->camera = new DebugCamera(WinApp::window_width, WinApp::window_height); //(/*input*/);
	input = Input::GetIns();

	file.open("Param_CSV/CameraPoints.csv");

	popcom << file.rdbuf();

	file.close();

	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find("Points_Quantity") == 0)
		{
			std::getline(line_stream, word, ',');
			int quantity = static_cast<int>(std::atof(word.c_str()));
			Quantity = quantity;
			break;
		}
	}
	Num.resize(Quantity);
	pos.resize(Quantity);
	for (int i = 0; i < Quantity; ++i)
	{
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find("POP") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				pos[i] = {x, y, z};
				break;
			}
		}
	}
	points.push_back({pos[0].x, pos[0].y, pos[0].z});
	points.push_back({pos[0].x, pos[0].y, pos[0].z});
	for (int i = 1; i < Quantity - 1; i++)
	{
		points.push_back({pos[i].x, pos[i].y, pos[i].z});
	}
	int quantity_end = Quantity - 1;
	points.push_back({pos[quantity_end].x, pos[quantity_end].y, pos[quantity_end].z});
	points.push_back({pos[quantity_end].x, pos[quantity_end].y, pos[quantity_end].z});

	//Load_ChestPosition.resize(Quantity);
	this->camera->SetEye(pos[0]);
	UpdateRange = 200.0f;
	startCount = static_cast<float>(GetTickCount64());


	dis.x = sinf(angle * (PI / 180.0f)) * 30.0f;
	dis.y = cosf(angle * (PI / 180.0f)) * 30.0f;
	distance.x = dis.x;
	distance.y = dis.y;
}

void CameraControl::Init_Boss()
{
	ParamSet();
	this->camera = new DebugCamera(WinApp::window_width, WinApp::window_height); //(/*input*/);
	input = Input::GetIns();

	startCount = static_cast<float>(GetTickCount64());

	dis.x = sinf(angle * (PI / 180.0f)) * 30.0f;
	dis.y = cosf(angle * (PI / 180.0f)) * 30.0f;
	distance.x = dis.x;
	distance.y = dis.y;
}

void CameraControl::Load()
{
}


/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/

void CameraControl::Update_Tutorial()
{
	AngleRotation();
	this->camera->Update();
	//TargetPlayer();
	(this->*targetTable[Tstate])();
}

void CameraControl::Update_Play()
{
	AngleRotation();

	this->camera->Update();

	TargetPlayer();
	//Feed::GetIns()->Update_White(Feed::FEEDOUT);
	(this->*targetTable[Tstate])();
}

void CameraControl::Update_Boss()
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
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	//OldCameraPos = camera->GetEye();
	//OldCameratarget = camera->GetTarget();
	//	encountGuardian = START;
	//	Tstate=ENCOUNTGUARDIAN;
	//}
	//else {
	if (AttackSceneF)
	{
		Feed::GetIns()->Update_White(Feed::FEEDOUT);
		if (Feed::GetIns()->GetAlpha() <= 0.0f)
		{
			AttackSceneF = false;
		}
	}

	this->camera->SetTarget({PlayerControl::GetIns()->GetPlayer()->GetPosition()});
	if (input->TiltPushStick(Input::R_RIGHT) || input->TiltPushStick(Input::R_LEFT))
	{
		if (input->TiltPushStick(Input::R_RIGHT))
		{
			angle += 2.0f;
			charaAngle += 2.0f;
			PlayerControl::GetIns()->GetPlayer()->SetAngle(charaAngle);
		}
		if (input->TiltPushStick(Input::R_LEFT))
		{
			angle -= 2.0f;
			charaAngle -= 2.0f;
			PlayerControl::GetIns()->GetPlayer()->SetAngle(charaAngle);
		}
		dis.x = sinf(angle * (PI / 180.0f)) * 30.0f;
		dis.y = cosf(angle * (PI / 180.0f)) * 30.0f;
		distance.x = dis.x;
		distance.y = dis.y;
	}
	if (PlayerControl::GetIns()->GetPlayer()->GetHP() < 0)
	{
		if (pdcamera == NON_PDEATH)
		{
			CameraPosition = {ppos.x, ppos.y + 30.f, ppos.z - 10.f};
			pdcamera = UPDA_PDEATH;
		}
		if (pdcamera == UPDA_PDEATH)
		{
			CameraPosition.x += 0.02f;
			CameraPosition.y -= 0.02f;
		}
	}
	else
	{
		pdcamera = NON_PDEATH;
		CameraPosition = {ppos.x + distance.x, ppos.y + 6.0f, ppos.z + distance.y};
	}
	//	camera->SetTarget(player_shadow->GetCameraPos(angle));
	camera->SetEye(CameraPosition);

	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		if (Task::GetIns()->GetAllTaskClear() && FenceControl::GetIns()->GetBossGateFence()->FenceYposMin() ==
			FALSE)
		{
			Tstate = MOVEBOSSAREA;
		}
		
	}
	if (Tstate == PLAYER)
	{
		if (ZoomF)
		{
			Tstate = BATTLEATART;
		}
		sCamera = PLAYCUTSTART;
		rCamera = NON_RUSH;
		bscamera = NON_BATTLESTART;
		mCamera = NON;
	}
	//}
}

/*------------------------*/
/*--------ターゲット---------*/
/*---------field---------*/
void CameraControl::TargetBossField()
{
	//if (Tstate == PLAYER)return;
	switch (mCamera)
	{
	case NON:
		if (FenceControl::GetIns()->GetBossGateFence()->FenceYposMin() == FALSE)
		{
			mCamera = FEED_BOSS;
		}
		break;
	case FEED_BOSS:
		Feed::GetIns()->Update_Black(Feed::FEEDIN);
		this->camera->SetTarget(PlayerControl::GetIns()->GetPlayer()->GetPosition());
		if (Feed::GetIns()->GetAlpha() >= 1.0f)
		{
			CameraPosition = {60.0f, 15.0f, 350.0f};

			mCamera = TARGETFENCE;
		}
		break;
	case TARGETFENCE:
		CameraPosition = {60.0f, -30.0f, 350.0f};

		this->camera->SetTarget({60.0f, -35.0f, 450.0f});

		Feed::GetIns()->Update_Black(Feed::FEEDOUT);

		if (Feed::GetIns()->GetAlpha() <= 0.0f)
		{
			mCamera = TARGETPLAYER;
		}
		break;
	case TARGETPLAYER:
		CameraPosition = {60.0f, -30.0f, 350.0f};

		this->camera->SetTarget({60.0f, -35.0f, 450.0f});

		if (FenceControl::GetIns()->GetBossGateFence()->FenceYposMin() == TRUE)
		{
			Feed::GetIns()->Update_Black(Feed::FEEDIN);
		}
		if (Feed::GetIns()->GetAlpha() >= 1.0f)
		{
			mCamera = END_BOSS;
		}
		break;
	case END_BOSS:
		//if (FenceControl::GetIns()->GetBossGateFence()->FenceYposMin() == TRUE) {
			if (Feed::GetIns()->GetAlpha() > 0.0f)
			{
				if (Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(), { 60, -35, 400 }) >= 50)
				{
					Feed::GetIns()->Update_Black(Feed::FEEDOUT);
				}
			} else
			{
				Tstate = PLAYER;
			}
		//}
		break;
	default:
		break;
	}
	camera->SetEye(CameraPosition);
}

void CameraControl::BossDeathStart()
{
}

void (CameraControl::* CameraControl::targetTable[])() = {
	nullptr,
	&CameraControl::TargetPlayer,
	nullptr,
	&CameraControl::TargetBossField,
	&CameraControl::PlaySceneStart,
	&CameraControl::GuardianCutScene,
	&CameraControl::BossSceneStart,
	&CameraControl::RushTargetBoss,
	&CameraControl::BossDeathStart,
	&CameraControl::BattleStart,
};

/*------------------------*/
/*--------カメラ位置--------*/
/*---------spline---------*/
XMVECTOR CameraControl::SplinePosition(const std::vector<XMVECTOR>& points, size_t startindex, float t)
{
	size_t n = points.size() - 2;
	if (startindex > n)
	{
		return points[n];
	}
	if (startindex < 1)
	{
		return points[1];
	}

	XMVECTOR p0 = points[startindex - 1];
	XMVECTOR p1 = points[startindex];
	XMVECTOR p2 = points[startindex + 1];
	XMVECTOR p3 = points[startindex + 2];

	XMVECTOR position = {
		2 * p1 + (-p0 + p2) * t +
		(2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) +
		(-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t)
	};
	position *= 0.5f;
	return position;
}


/*------------------------*/
/*--------ターゲット-------*/
/*---------boss---------*/
void CameraControl::BossSceneStart()
{
	const size_t size = 5;
	XMFLOAT3 BossPos = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	if (point.size() == 0)
	{
		point.push_back({BossPos.x, BossPos.y + 100.0f, BossPos.z - 10.0f});
		point.push_back({BossPos.x, BossPos.y + 100.0f, BossPos.z - 10.0f}); //上から
		point.push_back({BossPos.x, BossPos.y + 100.0f, BossPos.z - 50.0f}); //下へ
		point.push_back({BossPos.x, BossPos.y + 60.0f, BossPos.z - 10.0f}); //から右
		point.push_back({BossPos.x + 20.0f, BossPos.y + 40.0f, BossPos.z + 10.0f}); //後ろ回って
		point.push_back({BossPos.x + 0.0f, BossPos.y + 40.0f, BossPos.z + 30.0f}); //左くる
		point.push_back({BossPos.x - 20.0f, BossPos.y + 40.0f, BossPos.z + 10.0f});
		point.push_back({BossPos.x, BossPos.y + 40.0f, BossPos.z - 20.0f});
		point.push_back({BossPos.x, BossPos.y + 40.0f, BossPos.z - 20.0f});
	}
	switch (bCamera)
	{
	case BOSSCUTSTART:
		Feed::GetIns()->Update_White(Feed::FEEDOUT);

		BossCutScene_Start(BossPos);
		break;
	case CAMERADOWN:
		Feed::GetIns()->Update_White(Feed::FEEDOUT);

		BossCutScene_Spline();
		if (startindex >= 7)
		{
			bCamera = BOSSCUTEND;
		}
		break;

	case BOSSCUTEND:

		AttackSceneF = true;

		CameraPosition.z -= 1.5f;

		Feed::GetIns()->Update_White(Feed::FEEDIN);
		if (Feed::GetIns()->GetAlpha() >= 0.9f)
		{
			PlayerControl::GetIns()->GetPlayer()->SetStopFlag(false);

			Tstate = PLAYER;
		}

		break;
	default:
		break;
	}


	camera->SetTarget({BossPos.x, BossPos.y + BCutCameraHeight, BossPos.z});
	camera->SetEye(CameraPosition);
}

void CameraControl::BossCutScene_Spline()
{
	nowCount = static_cast<float>(GetTickCount64());
	elapsedCount = nowCount - startCount;
	elapsedTime = elapsedCount / 320.0f;

	timerate = elapsedTime / maxtime;
	if (timerate >= 1)
	{
		if (startindex < points.size() - 3)
		{
			startindex++;
			timerate -= 1;

			startCount = static_cast<float>(GetTickCount64());
		}
		else
		{
			timerate = 1;
		}
	}
	if (startindex < 7)
	{
		CameraPosition = {
			SplinePosition(point, startindex, timerate).m128_f32[0],
			SplinePosition(point, startindex, timerate).m128_f32[1],
			SplinePosition(point, startindex, timerate).m128_f32[2]
		};
	}
}

void CameraControl::BossCutScene_Start(XMFLOAT3 BossPos)
{
	PlayerControl::GetIns()->GetPlayer()->SetStopFlag(true);

	Feed::GetIns()->Update_White(Feed::FEEDOUT);

	CutCount[0]++;
	if (CutCount[0] > 10)
	{
		bCamera = CAMERADOWN;
	}
	CameraPosition = {BossPos.x, BossPos.y + 100.0f, BossPos.z - 10.0f};
}

/*------------------------*/
/*--------ターゲット---------*/
/*---------field---------*/
void CameraControl::PlaySceneStart()
{
	switch (sCamera)
	{
	case PLAYCUTSTART:
		PlayerControl::GetIns()->GetPlayer()->SetStopFlag(true);

		UI::GetIns()->SetTurnoffUIDraw(true);
		Feed::GetIns()->Update_White(Feed::FEEDOUT);

		if (Feed::GetIns()->GetAlpha() <= 0.9f)
		{
			sCamera = SPLINE;
		}
		break;
	case SPLINE:
		nowCount = static_cast<float>(GetTickCount64());
		elapsedCount = nowCount - startCount;
		elapsedTime = elapsedCount / 570.0f;

		timerate = elapsedTime / maxtime;
		if (timerate >= 1)
		{
			if (startindex < points.size() - 3)
			{
				startindex++;
				timerate -= 1;

				startCount = static_cast<float>(GetTickCount64());
			}
			else
			{
				timerate = 1;
			}
		}

		camera->SetEye({
			SplinePosition(points, startindex, timerate).m128_f32[0],
			SplinePosition(points, startindex, timerate).m128_f32[1],
			SplinePosition(points, startindex, timerate).m128_f32[2]
		});

	//カメラが一定距離近づいたらフェード
		if (Collision::GetLength(camera->GetEye(), PlayerControl::GetIns()->GetPlayer()->GetPosition()) < 30.0f)
		{
			Feed::GetIns()->Update_Black(Feed::FEEDIN);
		}
		else
		{
			Feed::GetIns()->Update_Black(Feed::FEEDOUT);
		}
		if (Feed::GetIns()->GetAlpha() >= 1.0f)
		{
			sCamera = PLAYCUTEND;
		}
		break;

	case PLAYCUTEND:
		this->camera->SetEye(CameraPosition);
		Feed::GetIns()->Update_Black(Feed::FEEDOUT);
		if (Feed::GetIns()->GetAlpha() <= 0.0f)
		{
			PlayerControl::GetIns()->GetPlayer()->SetStopFlag(false);

			UI::GetIns()->SetTurnoffUIDraw(false);
			Tstate = PLAYER;
		}
		break;
	default:
		break;
	}
	this->camera->SetTarget({PlayerControl::GetIns()->GetPlayer()->GetPosition()});
}

void CameraControl::SetCameraP_toE(Enemy* enemy)
{
}


void CameraControl::GuardianCutScene()
{
	/*switch (gcamera)
	{
	case GuardianCamera::NON_GUAR:

		break;

	case GuardianCamera::FEED_GUAR:
		Feed::GetIns()->Update_Black(Feed::FEEDIN);
		if(Feed::GetIns()->GetAlpha()>=1.0f)
		{
			
		}
		break;

	case GuardianCamera::PEDESTALMOVING:

		break;

	case GuardianCamera::GUARDIANAPPEAR:

		break;

	case GuardianCamera::END_GUAR:

		break;
	}*/
}

/*------------------------*/
/*--------シェイク---------*/
/*------------------------*/
void CameraControl::ShakeCamera()
{
	if (shaketime == 0.0f)
	{
		shaketime = 10.0f;
	}
	if (shaketime != 0)
	{
		shake = static_cast<float>(rand() % 5 - 2);
		shakex = static_cast<float>(rand() % 5 - 2);
		shakey = static_cast<float>(rand() % 5 - 2);
		shakex -= shake;
		shakey -= shake;
		shaketime--;
		//}
		//シェイク値を０に
	}
	else if (shaketime == 0.0f)
	{
		shakex = 0.0f;
		shakey = 0.0f;
	}
	camera->SetEye({CameraPosition.x + shakex, CameraPosition.y + shakey, CameraPosition.z});
}

void CameraControl::RushTargetBoss()
{
	Enemy* boss = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();

	if (rCamera == NON_RUSH)
	{
		OldPos = camera->GetEye();
		OldTarget = camera->GetTarget();

		PlayerControl::GetIns()->GetPlayer()->SetStopFlag(true);
		rCamera = UPBOSS;
	}
	if (rCamera == UPBOSS)
	{
		rCameraEtime += 0.01f;
		if (rCameraEtime < 1.0f)
		{
			TargetPos.x = Easing::EaseOut(rCameraEtime, OldTarget.x, boss->GetPosition().x);
			TargetPos.y = Easing::EaseOut(rCameraEtime, OldTarget.y, OldTarget.y + 5);

			TargetPos.z = Easing::EaseOut(rCameraEtime, OldTarget.z, boss->GetPosition().z);
		}
		else if (rCameraEtime > 2.0f)
		{
			rCamera = RETURNPLAYER;
		}
	}
	if (rCamera == RETURNPLAYER)
	{
		rCameraEtime -= 0.01f;

		if (rCameraEtime > 0.0f)
		{
			if (boss != nullptr)
			{
				TargetPos.y = Easing::EaseOut(rCameraEtime, OldTarget.y, OldTarget.y + 5);

				TargetPos.x = Easing::EaseOut(rCameraEtime, OldTarget.x, boss->GetPosition().x);
				TargetPos.z = Easing::EaseOut(rCameraEtime, OldTarget.z, boss->GetPosition().z);
			}
		}
		else
		{
			PlayerControl::GetIns()->GetPlayer()->SetStopFlag(false);

			Tstate = PLAYER;
		}
	}

	camera->SetTarget(TargetPos);
}

void CameraControl::BattleStart()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	if (bscamera== NON_BATTLESTART)
	{
		OldPos = camera->GetEye();
		OldTarget = camera->GetTarget();
		CameraTarget = OldTarget;
		bscamera = ZOOM_BATTLESTART;
	}
	else if (bscamera == ZOOM_BATTLESTART)
	{
		
		if (CameraPosMovingEaseT > 1.f) {
			
	
			if (ZoomF == false)
			{
				bscamera = RETURN_BATTLESTART;
			}
		}
		else
		{
			CameraPosMovingEaseT += 0.02f;
		}

		XMFLOAT3 position = { (OldPos.x + ZoomTarget.x) / 2.f,(OldPos.y + ZoomTarget.y) / 2.f,(OldPos.z+ ZoomTarget.z) / 2.f };           //CenterPointのポジションを中心に配置

		float radx = sqrtf((position.x - ZoomTarget.x) * (position.x - ZoomTarget.x));
		float radz = sqrtf((position.z - ZoomTarget.z) * (position.z - ZoomTarget.z));

		float disx = radx / sinf(OldCameraPos.x * 0.5f );
		float disz = radz / sinf(OldCameraPos.z * 0.5f);
		CameraTarget.x = Easing::EaseOut(CameraPosMovingEaseT, ppos.x, ((OldTarget.x + ZoomTarget.x) / 2.f));
			CameraTarget.z = Easing::EaseOut(CameraPosMovingEaseT, ppos.z, ((OldTarget.z + ZoomTarget.z) / 2.f) );
	
	}
	else if(bscamera==RETURN_BATTLESTART)
	{
		if (CameraPosMovingEaseT <= 0.f)
			bscamera = END_BATTLESTART;

		CameraPosMovingEaseT -= 0.02f;

		//OldPos = camera->GetEye();
		//bscamera = ZOOM_BATTLESTART;
		CameraTarget.x = Easing::EaseOut(CameraPosMovingEaseT,ppos.x , ((OldTarget.x + ZoomTarget.x) / 2.f));
		CameraTarget.z = Easing::EaseOut(CameraPosMovingEaseT, ppos.z, ((OldTarget.z + ZoomTarget.z) / 2.f));
		//	
	}
	else if(bscamera == END_BATTLESTART)
	{
		ZoomF = false;
		Tstate = PLAYER;
	}
	
	camera->SetTarget(CameraTarget);
	camera->SetEye(CameraPosition);
}


/*更新処理*/
void CameraControl::Draw_Tutorial()
{
}

void CameraControl::Draw_Play()
{
}

void CameraControl::Draw_Boss()
{
	/*ImGui::Begin("cam");
	ImGui::Text("size %d", point.size());
	ImGui::End();*/
}
