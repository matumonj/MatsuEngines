#include "Particle.h"
#include"mHelper.h"

void Particle::Init()
{
	Texture* l_tex0 = Texture::Create(34);
	Texture::LoadTexture(64, L"Resources/2d/BossAttackEffect/BomParticle.png");

	SetParType(1, m_particles[ParType::NORMAL], l_tex0);
}
#include"PlayerControl.h"
void Particle::Upda()
{
	if (Input::GetInstance()->TriggerButton(Input::Y)) {
		m_particles[ParType::NORMAL].phase = INIT;
	}
	InitNormal(m_particles[ParType::NORMAL],createpos);
		UpadaNormal_A(m_particles[ParType::NORMAL]);
}
#include"imgui.h"
void Particle::Draw()
{
	ImGui::Begin("phase");
	ImGui::Text("%d", m_particles[ParType::NORMAL].phase);
	ImGui::Text("x%f y%f z%f", m_particles[ParType::NORMAL].vel[0].x, m_particles[ParType::NORMAL].vel[0].y, m_particles[ParType::NORMAL].vel[0].z);
	ImGui::Text("x%f y%f z%f", PlayerControl::GetInstance()->GetPlayer()->GetPosition().x, PlayerControl::GetInstance()->GetPlayer()->GetPosition().y, PlayerControl::GetInstance()->GetPlayer()->GetPosition().z);
	ImGui::Text("rx%f ", m_particles[ParType::NORMAL].alpha[0]);
	
	ImGui::End();
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
		parparam.f.resize(size);

		std::vector<Texture*> l_tex0;
		l_tex0.resize(size);
	
		for (int i = 0; i <size; i++) {
			l_tex0[i] = Texture::Create(64);

			parparam.partex[i].reset(l_tex0[i]);
			parparam.partex[i]->CreateTexture();

			parparam.partex[i]->SetAnchorPoint({ 0.5f,0.5f });
		}
	}
}

void Particle::InitNormal(ParParam& parparam, XMFLOAT3 pos)
{
	if (parparam.phase != Phase::INIT)return;
	for (int i = 0; i < parparam.size; i++)
	{
		parparam.scl[i] = { 2.0f,2.0f };
		parparam.vel[i] = { pos };
		parparam.speed[i] = 0.0f; //���X�ɃX�s�[�h�𑬂�
		parparam.alpha[i] = 0.0f;
		parparam.angle[i] = rand() % 360;
	}
	parparam.phase = Phase::UPDA;
}
#include"CameraControl.h"
void Particle::UpadaNormal_A(ParParam& parparam)
{
	if (parparam.phase != Phase::UPDA)return;
	
	for (int i = 0; i < parparam.size; i++)
	{
		if (!parparam.f[i]) {
			parparam.vel[i] = createpos;
			parparam.scl[i] = { 2.0f,2.0f };
			parparam.speed[i] = 0;
			parparam.alpha[i] =1;
			parparam.f[i] = true;
			
			break;
		}
	}
	for (int i = 0; i < parparam.size; i++)
	{
		if (!parparam.f[i])continue;
		parparam.scl[i].x += 0.1f;
		parparam.scl[i].y += 0.1f;
		parparam.vel[i].x += parparam.speed[i] * cos(parparam.angle[i]); //360�x�ɍL����悤�ɂ���
		parparam.vel[i].y += parparam.speed[i] * sin(parparam.angle[i]); //360�x�ɍL����悤�ɂ���
		parparam.speed[i] += 0.002f; //���X�ɃX�s�[�h�𑬂�
		parparam.alpha[i] -= 0.05f;
		if (parparam.alpha[i] < 0.0f) {
			parparam.f[i] = false;
		}
		parparam.partex[i]->SetPosition(parparam.vel[i]);
		parparam.partex[i]->SetScale({ parparam.scl[i].x,parparam.scl[i].y,1.0f });
		parparam.partex[i]->SetBillboard(TRUE);
		parparam.partex[i]->SetColor({ 1.f,1.f,1.f,parparam.alpha[i] });
		parparam.partex[i]->Update(CameraControl::GetInstance()->GetCamera());

	}
	//if (isAryEqual(parparam.alpha) == true) {
	//	parparam.phase = Phase::END;
	//}
}


bool Particle::isAryEqual(std::vector<float>num)
{
	if (num.size() == 0)return false;
	if (num.size() == 1)return false;
	if (num[0] > 0.0f)return false;
	for (int i = 0; i < num.size(); i++) {
		if (num[0] != num[i])return false;
	}
	return true;
}
