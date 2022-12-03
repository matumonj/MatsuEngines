#include "Particle.h"
#include"mHelper.h"

void Particle::Init()
{
	Texture::LoadTexture(40, L"Resources/2d/particle/normal.png");

	Texture* l_tex0 = Texture::Create(40);

	SetParType(10, m_particles[ParType::NORMAL], l_tex0);
}

void Particle::Upda()
{
	//if (m_particles[ParType::NORMAL].CreateJudg) {
	
}

void Particle::Draw()
{
	Texture::PreDraw();
	for (int i = 0; i < m_particles[ParType::NORMAL].size; i++) {
		m_particles[ParType::NORMAL].partex[i]->Draw();
	}
	Texture::PostDraw;
}

void Particle::EndParticleUpda(ParParam& parparam)
{
	if (parparam.size != 0) {
		for (int i = 0; i < parparam.size; i++)
		{
			parparam.alpha[i] = 1.0f;

		}
	}
}
void Particle::SetParType(int size, ParParam& parparam,Texture*tex)
{
	parparam.size = size;
	if (parparam.size != 0) {
		parparam.angle.resize(size);	
		parparam.alpha.resize(size);
		parparam.scl.resize(size);
		parparam.vel.resize(size);
		parparam.speed.resize(size);
		parparam.partex.resize(size);
		for (int i = 0; i <size; i++) {
			parparam.partex[i].reset(tex);
			parparam.partex[i]->CreateTexture();

			parparam.partex[i]->SetAnchorPoint({ 0.f,0.f });
		}
	}
}

void Particle::UpadaNormal(ParParam& parparam)
{
	if (parparam.phase != Phase::UPDA)return;
	for (int i = 0; i < parparam.size; i++)
	{
		parparam.scl[i].x += 0.1f;
		parparam.scl[i].y += 0.1f;
		parparam.vel[i].x += parparam.speed[i] * cos(parparam.angle[i]); //360度に広がるようにする
		parparam.vel[i].y += parparam.speed[i] * sin(parparam.angle[i]); //360度に広がるようにする
		parparam.speed[i] += 0.002f; //徐々にスピードを速く
		parparam.alpha[i] -= 0.01f;

		parparam.partex[i]->SetPosition(parparam.vel[i]);
		parparam.partex[i]->SetScale({ parparam.scl[i].x,parparam.scl[i].y,0.0f });
		parparam.partex[i]->SetBillboard(TRUE);
		parparam.partex[i]->SetColor({ 1.f,1.f,1.f,parparam.alpha[i] });
	}
	
}
