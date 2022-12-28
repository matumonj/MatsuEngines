#include "Particle.h"
#include"mHelper.h"

void Particle::Init(UINT num)
{
	Texture::LoadTexture(64, L"Resources/2d/BossAttackEffect/BomParticle.png");
	Texture::LoadTexture(65, L"Resources/2d/mainEffect/pipo-hiteffect034.png");
	Texture* l_tex0 = Texture::Create(num);
	parnum = num;

	BeginParColor = {1.f, 1.f, 1.f, 1.f};
	SetParType(20, m_particles[NORMAL], l_tex0);
}

#include"PlayerControl.h"

void Particle::Upda()
{
	if (Input::GetInstance()->TriggerButton(Input::Y))
	{
		//	m_particles[ParType::NORMAL].phase = INIT;
	}
	InitNormal(m_particles[NORMAL], createpos);
	UpadaNormal_A(m_particles[NORMAL]);
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

#include"CameraControl.h"

void Particle::UpadaNormal_A(ParParam& parparam)
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
		parparam.scl[i].x += 0.1f;
		parparam.scl[i].y += 0.1f;
		parparam.vel[i].x += parparam.speed[i] * cos(parparam.angle[i]); //360度に広がるようにする
		parparam.vel[i].y += parparam.speed[i] * sin(parparam.angle[i]); //360度に広がるようにする
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
