#include "BossMap.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"imgui.h"
BossMap* BossMap::GetInstance()
{
	static BossMap ins;
	return&ins;
}
void BossMap::Init()
{
	
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	for (int i = 0; i < mapHight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			mapSize[i][j] = BLOCK;
			mapobj[i][j] = std::make_unique<Object3d>();
			//フィールドにモデル割り当て
			mapobj[i][j]->Initialize(camera);
			mapobj[i][j]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BOX));
			nailobj[i][j] = std::make_unique<Object3d>();
			//フィールドにモデル割り当て
			nailobj[i][j]->Initialize(camera);
			nailobj[i][j]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NAIL));

			par[i][j] = std::make_unique<Particle>();
			par[i][j]->Init();
			//フィールドにモデル割り当て
			nailobj[i][j]->Initialize(camera);
			nailobj[i][j]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NAIL));

		}
	}
}
#include"EnemyControl.h"
#include"AltAttack.h"
void BossMap::Upda()
{
	Enemy* boss=EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	Player* player = PlayerControl::GetInstance()->GetPlayer();
	RushArea.start = { boss->GetPosition().x ,boss->GetPosition().z};
	RushArea.end = { player->GetPosition().x ,player->GetPosition().z };
	// 頂点間の長さを測る
		// 頂点間の長さを測る
	float line_length =Collision::CalculationVertexLength(RushArea.start, RushArea.end);
	
	

	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	for (int i = 0; i < mapHight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			//フィールドにモデル割り当て
			mapobj[i][j]->SetPosition({ j * BlockSize+cpos.x,cpos.y,i * BlockSize+cpos.z });
			mapobj[i][j]->SetScale(bsize);

			bpoint[i][j].x = mapobj[i][j]->GetPosition().x;
			bpoint[i][j].y = mapobj[i][j]->GetPosition().z;

			mapobj[i][j]->Update({1,0,0,1},camera);
			
			if (Collision::IsCollidingLineAndCircle(RushArea, bpoint[i][j])==true)
			{
				mapSize[i][j] = DAMAGEBLOCK;
			}
			else {
				mapSize[i][j] = BLOCK;
			}
		}
	}
	if (Input::GetInstance()->TriggerButton(Input::X)) {
		p = phase::INI;
	}
	DamageBlockMove();
	SetDamageArea();

	for (int i = 0; i < mapHight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			if (mapSize[i][j] == DAMAGEBLOCK) {
				
				nailobj[i][j]->SetPosition(nailPos[i][j]);
				nailobj[i][j]->SetScale({5.9,6.0,5.0});
				nailobj[i][j]->SetRotation({ 180,0,0 });
				nailobj[i][j]->Update({ 1,0,0,1 }, camera);
				nailobj[i][j]->SetColor({ 1,1,1,nailalpha[i][j]});
				//mapobj[i][j]->SetUVf(true);
				mapobj[i][j]->SetColor({ 1,0,0,0.5 });
			}
			else {
				mapobj[i][j]->SetUVf(false);
				mapobj[i][j]->SetColor({ 1,1,1,1 });
			}
			}
		}
}

void BossMap::DamageBlockMove()
{
	

	if (p == phase::INI) {
		for (int i = 0; i < mapHight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				if (mapSize[i][j] == DAMAGEBLOCK) {
					nailPos[i][j] = mapobj[i][j]->GetPosition();
						nailalpha[i][j] = 1.05f;
					

				}
			}
		}
		p = phase::UPDA;
	}

	if(p == phase::UPDA){
		for (int i = 0; i < mapHight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				if (mapSize[i][j] == DAMAGEBLOCK) {
					nailPos[i][j].y+=0.5f;
				}
			
				nailPos[i][j].y = min(nailPos[i][j].y, 15);
				nailPos[i][j].y = max(nailPos[i][j].y, -6);

			}
		}
			rockappertime++;
			if (rockappertime > 240) {
				p = phase::END;
			}
		
	}

	if (p == phase::END)
	{
		
		for (int i = 0; i < mapHight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				if (mapSize[i][j] == DAMAGEBLOCK) {
					par[i][j]->CreateParticle(true, mapobj[i][j]->GetPosition());
					par[i][j]->Upda();
					nailalpha[i][j] -= 0.05f;
				}


				nailPos[i][j].y = min(nailPos[i][j].y, 15);
				nailPos[i][j].y = max(nailPos[i][j].y, -16);

			}
		}
	}
	
}

void BossMap::SetDamageArea()
{

}
void BossMap::Draw()
{
	Object3d::PreDraw();
	for (int i = 0; i < mapHight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			mapobj[i][j]->Draw();
			if (mapSize[i][j] == DAMAGEBLOCK) {
				if (nailalpha[i][j] > 0.0f) {
					nailobj[i][j]->Draw();
				}
				
			}
		}
	}
	Object3d::PostDraw();
	Texture::PreDraw();
	for (int i = 0; i < mapHight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			if (mapSize[i][j] == DAMAGEBLOCK) {
				par[i][j]->Draw();
			}
		}
	}
	Texture::PostDraw();
	ImGui::Begin("map");
	ImGui::Text("%d", p);
	ImGui::SliderFloat("posX", &lineendpos.x,-200,200);
	//ImGui::SliderFloat("posY", &cpos.y, -200, 200);
	ImGui::SliderFloat("posZ", &lineendpos.y, -200, 200);

	ImGui::SliderFloat("sizeX", &bsize.x, 0, 10);
	ImGui::SliderFloat("sizeY", &bsize.y, 0, 10);
	ImGui::SliderFloat("sizeZ", &bsize.z, 0, 10);
	ImGui::End();
}