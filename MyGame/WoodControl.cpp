#include "WoodControl.h"


WoodControl* WoodControl::GetInstance()
{
	static WoodControl instance;

	return &instance;
}
void WoodControl::LoadWood(DebugCamera* camera)
{
	//file2.open("EnemyParam_CSV/position.csv");
		//file.open("EnemyParam_CSV/open.csv");
	file.open("EnemyParam_CSV/wood.csv");

	//popcom << file.rdbuf();

	popcom << file.rdbuf();

	//file.close();
	file.close();
	//return oi;
	//fopen_s(&fp, "posxx.json", "r");
	/*流れとしては
	敵の数読み込み->
	読み込んだ敵の数分エネミーのパラメータ配列の要素数増やす->
	敵の数分作ったら配列の中身をロードしたものに->
	敵の番号が1だったらα,2だったらβでインスタンス生成、初期化
	*/
	//fread(&Enemy_Quantity, sizeof(int), 1, fp);
	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("Wood_Quantity") == 0) {
			std::getline(line_stream, word, ',');
			int quantity = (int)std::atof(word.c_str());
			Wood_Quantity = quantity;
			break;
		}
	}
	Wood_Num.resize(Wood_Quantity);
	woodpos.resize(Wood_Quantity);
	for (int i = 0; i < Wood_Quantity; i++) {
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

				woodpos[i] = { x,y,z };
				break;
			}
		}
	}
	woods.resize(Wood_Quantity);

	Load_WoodPosition.resize(Wood_Quantity);

	for (int i = 0; i < Wood_Quantity; i++) {

		woods[i] = std::make_unique<Wood>();

		woods[i]->Initialize(camera);
		woods[i]->SetPosition(woodpos[i]);
	}

}

void WoodControl::Initialize(DebugCamera* camera)
{

}

void WoodControl::Update(DebugCamera* camera)
{
	Player_OldPos= Player::GetInstance()->GetPosition();
	for (int i = 0; i < Wood_Quantity; i++) {
		if (woods[i] != nullptr) {

			woods[i]->Update(camera);
		}
	}
	for (int i = 0; i < Wood_Quantity; i++) {
		if (woods[i]->CollideWood() == true) {
			Player::GetInstance()->SetPosition(Player_OldPos);
			Player::GetInstance()->SetGround(true);
			break;
		}
	}
}

void WoodControl::Draw()
{
	for (int i = 0; i < Wood_Quantity; i++) {
		if (woods[i] != nullptr) {
			woods[i]->Draw();
		}
	}
}

void WoodControl::ImGuiDraw()
{

}