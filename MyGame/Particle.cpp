#include "Particle.h"

#include "EnemyControl.h"
#include"mHelper.h"

#include"CameraControl.h"

void Particle::Init(UINT num)
{
	Texture::LoadTexture(64, L"Resources/2d/BossAttackEffect/BomParticle.png");
	Texture::LoadTexture(65, L"Resources/2d/mainEffect/pipo-hiteffect034.png");
	Texture* l_tex0 = Texture::Create(num);
	parnum = num;

	BeginParColor = {1.f, 1.f, 1.f, 1.f};
	SetParType(7, m_particles[NORMAL], l_tex0);

	SetParType(1, m_particles[BLEATH], l_tex0);
}

#include"PlayerControl.h"
void Particle::Charge(ParParam& parparam)
{
	float dis[20];
	//追跡スピード
	float centerSpeed = 0.2f;

	float disX[20], disY[20], disZ[20];

	XMFLOAT3 mouthPos=EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	if (parparam.phase != UPDA)
	{
		return;
	}

	for (int i = 0; i < parparam.size; i++)
	{
		if (!parparam.f[i])
		{
			parparam.vel[i] = { mouthPos.x+rand()%20-10,mouthPos.y+5.f ,
			mouthPos.z+ rand() % 20 - 10 };
			parparam.scl[i] = { 2.0f, 2.0f };
			parparam.speed[i] = 0.0f;
			parparam.alpha[i] = 1.0f;
			parparam.f[i] = true;

			break;
		}

			disX[i] = mouthPos.x -parparam.vel[i].x;
			disY[i] = mouthPos.y - parparam.vel[i].y;
		disZ[i] = mouthPos.z - parparam.vel[i].z;
			dis[i] = Collision::GetLength(mouthPos, parparam.vel[i]);
			//trad=(PlayerControl::GetInstance()->GetPlayer()->GetPosition().y - it->position.y, PlayerControl::GetInstance()->GetPlayer()->GetPosition().x - it->position.x);

		//座標のセット
			if (dis[i] > 1.0f)
			{
				//座標のセット
				parparam.vel[i] = {
				parparam.vel[i].x + (disX[i] / dis[i]) * centerSpeed,parparam.vel[i].y + (disY[i] / dis[i]) * centerSpeed,
					parparam.vel[i].z + (disZ[i] / dis[i]) * centerSpeed
				};
			}
		parparam.speed[i] += 0.002f; //徐々にスピードを速く
		parparam.alpha[i] -= 0.02f;
		if (parparam.alpha[i] < 0.0f && parparam.EndParUpda[i] == false)
		{
			parparam.f[i] = false;
		}
	}
	for (int i = 0; i < parparam.size; i++)
	{
		parparam.partex[i]->SetPosition(parparam.vel[i]);
		parparam.partex[i]->SetScale({ parparam.scl[i].x, parparam.scl[i].y, 1.0f });
		parparam.partex[i]->SetBillboard(TRUE);
		parparam.partex[i]->SetColor({ 1.f, 1.f, 1.f, parparam.alpha[i] });
		parparam.partex[i]->Update(CameraControl::GetInstance()->GetCamera());
	}
	if (isAryEqual(parparam.alpha) == true)
	{
		//parparam.phase = END;
	}
}

void Particle::UpadaBleath(ParParam& parparam)
{
	//経過フレーム数をカウント
	float dis[20];
	//追跡スピード
	float centerSpeed = 0.2f;

	float disX[20],disY[20],disZ[20];

	//敵とプレイヤーの距離求め
	for (int i = 0; i < 20; i++) {
		disX[i] = PlayerControl::GetInstance()->GetPlayer()->GetPosition().x - m_particles[BLEATH].vel[i].x;
		disY[i] = PlayerControl::GetInstance()->GetPlayer()->GetPosition().y - m_particles[BLEATH].vel[i].y;
		disZ[i] = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - m_particles[BLEATH].vel[i].z;
		dis[i] = Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), m_particles[BLEATH].vel[i]);
		//trad=(PlayerControl::GetInstance()->GetPlayer()->GetPosition().y - it->position.y, PlayerControl::GetInstance()->GetPlayer()->GetPosition().x - it->position.x);

	//座標のセット
		if (dis[i] > 1.0f)
		{
			//座標のセット
			m_particles[BLEATH].vel[i] = {
			m_particles[BLEATH].vel[i].x + (disX[i] / dis[i]) * centerSpeed,m_particles[BLEATH].vel[i].y + (disY[i] / dis[i]) * centerSpeed,
				m_particles[BLEATH].vel[i].z + (disZ[i] / dis[i]) * centerSpeed
			};
		}
	}
}
void Particle::Bleath()
{
	InitNormal(m_particles[BLEATH], createpos);
	Charge(m_particles[BLEATH]);
}

void Particle::Upda(float addspeed, float addalpha )
{
	if (Input::GetInstance()->TriggerButton(Input::Y))
	{
		//	m_particles[ParType::NORMAL].phase = INIT;
	}
	InitNormal(m_particles[NORMAL], createpos);
	UpadaNormal_A(m_particles[NORMAL],addspeed,addalpha);
}

void Particle::Upda_B()
{
	if (Input::GetInstance()->TriggerButton(Input::Y))
	{
		//	m_particles[ParType::NORMAL].phase = INIT;
	}
	InitNormal(m_particles[NORMAL], createpos);
	UpadaNormal_B(m_particles[NORMAL]);
}

#include"imgui.h"

void Particle::Draw()
{
	Texture::PreDraw();
	for (int i = 0; i < m_particles[NORMAL].size; i++)
	{
		m_particles[NORMAL].partex[i]->Draw();
	}
	Texture::PostDraw();
}

void Particle::EndParticleUpda(ParParam& parparam)
{
	if (parparam.size != 0)
	{
		for (int i = 0; i < parparam.size; i++)
		{
			parparam.alpha[i] = 1.0f;
		}
	}
}

void Particle::SetParType(int size, ParParam& parparam, Texture* tex)
{
	parparam.size = size;
	if (parparam.size != 0)
	{
		parparam.angle.resize(size);
		parparam.alpha.resize(size);
		parparam.scl.resize(size);
		parparam.vel.resize(size);
		parparam.speed.resize(size);
		parparam.partex.resize(size);
		parparam.f.resize(size);
		parparam.EndParUpda.resize(size);
		std::vector<Texture*> l_tex0;
		l_tex0.resize(size);

		for (int i = 0; i < size; i++)
		{
			l_tex0[i] = Texture::Create(parnum);

			parparam.partex[i].reset(l_tex0[i]);
			parparam.partex[i]->CreateTexture();

			parparam.partex[i]->SetAnchorPoint({0.5f, 0.5f});
		}
	}
}

void Particle::InitNormal(ParParam& parparam, XMFLOAT3 pos)
{
	if (parparam.phase != INIT)
	{
		return;
	}
	for (int i = 0; i < parparam.size; i++)
	{
		parparam.scl[i] = {BeginParScl};
		parparam.vel[i] = {pos};
		parparam.speed[i] = 0.0f; //徐々にスピードを速く
		parparam.alpha[i] = 0.0f;
		parparam.angle[i] = static_cast<float>(rand() % 360);
	}
	parparam.phase = UPDA;
}

void Particle::UpadaNormal_A(ParParam& parparam, float addspeed , float addalpha )
{
	if (parparam.phase != UPDA)
	{
		return;
	}

	for (int i = 0; i < parparam.size; i++)
	{
		if (!parparam.f[i])
		{
			parparam.vel[i] = createpos;
			parparam.scl[i] = {2.0f, 2.0f};
			parparam.speed[i] = 0.0f;
			parparam.alpha[i] = 1.0f;
			parparam.f[i] = true;

			break;
		}
		parparam.scl[i].x += addspeed;
		parparam.scl[i].y += addspeed;
		parparam.vel[i].x += parparam.speed[i] * cos(parparam.angle[i]); //360度に広がるようにする
		parparam.vel[i].y += parparam.speed[i] * sin(parparam.angle[i]); //360度に広がるようにする
		parparam.speed[i] += 0.002f; //徐々にスピードを速く
		parparam.alpha[i] -= addalpha;
		if (parparam.alpha[i] < 0.0f && parparam.EndParUpda[i] == false)
		{
			parparam.f[i] = false;
		}
	}
	for (int i = 0; i < parparam.size; i++)
	{
		parparam.partex[i]->SetPosition(parparam.vel[i]);
		parparam.partex[i]->SetScale({parparam.scl[i].x, parparam.scl[i].y, 1.0f});
		parparam.partex[i]->SetBillboard(TRUE);
		parparam.partex[i]->SetColor({1.f, 1.f, 1.f, parparam.alpha[i]});
		parparam.partex[i]->Update(CameraControl::GetInstance()->GetCamera());
	}
	if (isAryEqual(parparam.alpha) == true)
	{
		parparam.phase = END;
	}
}


void Particle::UpadaNormal_B(ParParam& parparam)
{
	if (partype == 0)
	{
		if (parparam.phase != UPDA)
		{
			return;
		}

		for (int i = 0; i < parparam.size; i++)
		{
			if (!parparam.f[i])
			{
				parparam.vel[i] = createpos;
				parparam.scl[i] = {BeginParScl};
				parparam.speed[i] = 0;
				parparam.alpha[i] = 1;
				parparam.f[i] = true;

				break;
			}
		}
		for (int i = 0; i < parparam.size; i++)
		{
			if (!parparam.f[i])
			{
				continue;
			}
			parparam.scl[i].x += 0.05f;
			parparam.scl[i].y += 0.05f;
			parparam.vel[i].x += parparam.speed[i] * cos(parparam.angle[i]); //360度に広がるようにする
			parparam.vel[i].y += 0.1f; // parparam.speed[i] * sin(parparam.angle[i]); //360度に広がるようにする
			parparam.speed[i] += 0.002f; //徐々にスピードを速く
			parparam.alpha[i] -= 0.01f;

			parparam.partex[i]->SetPosition(parparam.vel[i]);
			parparam.partex[i]->SetScale({parparam.scl[i].x, parparam.scl[i].y, 1.0f});
			parparam.partex[i]->SetBillboard(TRUE);
			parparam.partex[i]->SetColor({BeginParColor.x, BeginParColor.y, BeginParColor.z, parparam.alpha[i]});
			parparam.partex[i]->Update(CameraControl::GetInstance()->GetCamera());
		}
	}

	//
	if (partype == 1)
	{
		if (parparam.phase != UPDA)
		{
			return;
		}

		for (int i = 0; i < parparam.size; i++)
		{
			if (!parparam.f[i])
			{
				parparam.vel[i] = createpos;
				parparam.scl[i] = {1.0f, 1.0f};
				parparam.speed[i] = 0;
				parparam.alpha[i] = 1;
				parparam.f[i] = true;

				break;
			}
		}
		for (int i = 0; i < parparam.size; i++)
		{
			if (!parparam.f[i])
			{
				continue;
			}
			parparam.scl[i].x += 0.05f;
			parparam.scl[i].y += 0.05f;
			parparam.vel[i].x += parparam.speed[i] * cos(parparam.angle[i]); //360度に広がるようにする
			parparam.vel[i].y += parparam.speed[i] * sin(parparam.angle[i]); //360度に広がるようにする
			parparam.speed[i] += 0.002f; //徐々にスピードを速く
			parparam.alpha[i] -= 0.01f;

			parparam.partex[i]->SetPosition(parparam.vel[i]);
			parparam.partex[i]->SetScale({parparam.scl[i].x, parparam.scl[i].y, 1.0f});
			parparam.partex[i]->SetBillboard(TRUE);
			parparam.partex[i]->SetColor({BeginParColor.x, BeginParColor.y, BeginParColor.z, parparam.alpha[i]});
			parparam.partex[i]->Update(CameraControl::GetInstance()->GetCamera());
		}
	}
	if (isAryEqual(parparam.alpha) == true)
	{
		parparam.phase = END;
	}
}


bool Particle::isAryEqual(std::vector<float> num)
{
	if (num.size() == 0)
	{
		return false;
	}
	if (num.size() == 1)
	{
		return false;
	}
	if (num[0] > 0.0f)
	{
		return false;
	}
	for (int i = 0; i < num.size(); i++)
	{
		if (num[0] != num[i])
		{
			return false;
		}
	}
	return true;
}
