#include "Particle.h"

#include "EnemyControl.h"
#include"mHelper.h"

#include"CameraControl.h"

void Particle::Init(UINT num)
{
	Texture* l_tex0 = Texture::Create(num);
	parnum = num;

	BeginParColor = {1.f, 1.f, 1.f, 1.f};
	SetParType(7, m_particles[NORMAL], l_tex0);


	SetParType(10, m_particles[FIRE], l_tex0);
}

#include"PlayerControl.h"

void Particle::Charge(ParParam& parparam)
{
	float dis[20];
	//追跡スピード
	float centerSpeed = 0.2f;

	float disX[20], disY[20], disZ[20];

	XMFLOAT3 mouthPos = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	if (parparam.phase != UPDA)
	{
		return;
	}

	for (int i = 0; i < parparam.size; i++)
	{
		if (!parparam.f[i])
		{
			parparam.vel[i] = {
				mouthPos.x + rand() % 20 - 10, mouthPos.y + 5.f,
				mouthPos.z + rand() % 20 - 10
			};
			parparam.scl[i] = {2.0f, 2.0f};
			parparam.speed[i] = 0.0f;
			parparam.alpha[i] = 1.0f;
			parparam.f[i] = true;

			break;
		}

		disX[i] = mouthPos.x - parparam.vel[i].x;
		disY[i] = mouthPos.y - parparam.vel[i].y;
		disZ[i] = mouthPos.z - parparam.vel[i].z;
		dis[i] = Collision::GetLength(mouthPos, parparam.vel[i]);
		//trad=(PlayerControl::GetIns()->GetPlayer()->GetPosition().y - it->position.y, PlayerControl::GetIns()->GetPlayer()->GetPosition().x - it->position.x);

		//座標のセット
		if (dis[i] > 1.0f)
		{
			//座標のセット
			parparam.vel[i] = {
				parparam.vel[i].x + (disX[i] / dis[i]) * centerSpeed,
				parparam.vel[i].y + (disY[i] / dis[i]) * centerSpeed,
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
		parparam.partex[i]->SetScale({parparam.scl[i].x, parparam.scl[i].y, 1.0f});
		parparam.partex[i]->SetBillboard(TRUE);
		parparam.partex[i]->SetColor({1.f, 1.f, 1.f, parparam.alpha[i]});
		parparam.partex[i]->Update(CameraControl::GetIns()->GetCamera());
	}
	if (isAryEqual(parparam.alpha) == true)
	{
		//parparam.phase = END;
	}
}

void Particle::UpadaBleath(ParParam& parparam)
{
	float f[10];
	const float rnd_vel[10] = {0.5f};
	XMFLOAT3 dvel[10]{};
	for (int i = 0; i < 10; i++)
	{
	}
	for (int i = 0; i < parparam.size; i++)
	{
		if (!parparam.f[i])
		{
			parparam.scl[i] = {2.0f, 2.0f};
			parparam.speed[i] = 0.0f;
			parparam.alpha[i] = 1.0f;
			parparam.ParColor[i] = {0.2f, 1.f, 1.f, 1.f};
			parparam.Frame[i] = 0.f;
			dvel[i] = {
				static_cast<float>(rand()) / RAND_MAX * rnd_vel[i] - rnd_vel[i] / 2.0f,
				static_cast<float>(rand()) / RAND_MAX * rnd_vel[i] * 2.0f, 1.f
			};

			parparam.vel[i].x = createpos.x + dvel[i].x;
			parparam.vel[i].y = createpos.y;
			parparam.vel[i].z = createpos.z;
			parparam.f[i] = true;

			break;
		}
		XMFLOAT3 pos = createpos;

		// 経過フレーム数をカウント
		parparam.Frame[i] += 0.1f;
		// 進行度を0～1の範囲に換算
		f[i] = 120.f / parparam.Frame[i];
		// カラーの線形補間
		parparam.ParColor[i].x = parparam.ParColor[i].x + (1.f - 0.1f) / f[i];
		parparam.ParColor[i].y = parparam.ParColor[i].y + (0.2f - 1.f) / f[i];
		parparam.ParColor[i].z = parparam.ParColor[i].z + (0.2f - 1.f) / f[i];
		parparam.scl[i].x -= 0.006f;
		parparam.scl[i].y -= 0.006f;
		parparam.vel[i].y += 0.02f; //360度に広がるようにする
		parparam.speed[i] += 0.002f; //徐々にスピードを速く
		parparam.alpha[i] -= 0.008f; // f + (0.2f - 1.f) / f[i];
		if (parparam.alpha[i] < 0.0f)
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
		parparam.partex[i]->Update(CameraControl::GetIns()->GetCamera());
	}
}

void Particle::Bleath()
{
	InitNormal(m_particles[FIRE], createpos);
	UpadaBleath(m_particles[FIRE]);
}

void Particle::Upda(float addspeed, float addalpha)
{
	if (Input::GetIns()->TriggerButton(Input::Y))
	{
		//	m_particles[ParType::NORMAL].phase = INIT;
	}
	InitNormal(m_particles[NORMAL], createpos);
	UpadaNormal_A(m_particles[NORMAL], addspeed, addalpha);
}

void Particle::Upda_B(bool loop)
{
	if (Input::GetIns()->TriggerButton(Input::Y))
	{
		//	m_particles[ParType::NORMAL].phase = INIT;
	}
	InitNormal(m_particles[NORMAL], createpos);
	UpadaNormal_B(m_particles[NORMAL], loop);
}

#include"imgui.h"

void Particle::Draw()
{
	Texture::PreDraw();
	for (int i = 0; i < m_particles[NORMAL].size; i++)
	{
		m_particles[NORMAL].partex[i]->Draw();
	}
	for (int i = 0; i < m_particles[FIRE].size; i++)
	{
		m_particles[FIRE].partex[i]->Draw();
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
		parparam.ParColor.resize(size);
		parparam.Frame.resize(size);
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

void Particle::UpadaNormal_A(ParParam& parparam, float addspeed, float addalpha)
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
		parparam.partex[i]->Update(CameraControl::GetIns()->GetCamera());
	}
	if (isAryEqual(parparam.alpha) == true)
	{
		parparam.phase = END;
	}
}


void Particle::UpadaNormal_B(ParParam& parparam, bool loop)
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
				parparam.vel[i] = {
					createpos.x + static_cast<float>(rand() % 5 - 2), createpos.y,
					createpos.z + static_cast<float>(rand() % 5 - 2)
				};
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
			//parparam.vel[i].x += parparam.speed[i] * cos(parparam.angle[i]); //360度に広がるようにする
			parparam.vel[i].y += 0.1f; // parparam.speed[i] * sin(parparam.angle[i]); //360度に広がるようにする
			parparam.speed[i] += 0.002f; //徐々にスピードを速く
			parparam.alpha[i] -= 0.01f;
			if (loop)
			{
				if (parparam.alpha[i] < 0.0f)
				{
					parparam.f[i] = false;
				}
			}
			parparam.partex[i]->SetPosition(parparam.vel[i]);
			parparam.partex[i]->SetScale({parparam.scl[i].x, parparam.scl[i].y, 1.0f});
			parparam.partex[i]->SetBillboard(TRUE);
			parparam.partex[i]->SetColor({BeginParColor.x, BeginParColor.y, BeginParColor.z, parparam.alpha[i]});
			parparam.partex[i]->Update(CameraControl::GetIns()->GetCamera());
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
			parparam.partex[i]->Update(CameraControl::GetIns()->GetCamera());
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
