#include "TitleScene.h"
#include <Xinput.h>


using json = nlohmann::json;

struct RankEntry {
	std::string name;
	int score;
};

// 比較関数（降順）
bool CompareScore(const RankEntry& a, const RankEntry& b) {
	return a.score > b.score;
}


TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize() {


    // 状態変数
    combo = 1;
	success = false;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

}

void TitleScene::Update() {


	if (input_->PressedKey(DIK_SPACE)) {

		if (reset == false) {
			int chance = rand() % 100;
			if (chance < SUCCESS_RATE) {
				combo++;
				success = true;
			}
			else {
				success = false;
				reset = true;
			}
		}
	}
	if (reset == true && input_->PressedKey(DIK_Q)) {
		// 成功処理のあと
		if (success && reset) {
			// スコアがランキングに入るなら登録
			TryInsertToRanking("Player", combo);
			PostRankingToServer(CreateRankingJson());
		}
		combo = 1;
		reset = false;
	}

	// UI描画
	ImGui::Begin("ST Continuation Challenge ");//エヴァ15ST継続チャレンジ

	if (ImGui::Button("Success Determination (can be done with buttons)")) {//成功判定（ボタンでもOK）
		if (reset == false) {
			int chance = rand() % 100;
			if (chance < SUCCESS_RATE) {
				combo++;
				success = true;
			}
			else {
				success = false;
				reset = true;
			}
		}
	}
	
	if (reset == true) {
		if (ImGui::Button("Reset (can also be done with Q key)")) {
			// 成功処理のあと
			if (success && reset) {
				// スコアがランキングに入るなら登録
				TryInsertToRanking("Player", combo);
				PostRankingToServer(CreateRankingJson());
			}
			combo = 1;
			reset = false;
		}
	}


	ImGui::Text("Space key or button to challenge");//スペースキー or ボタンで挑戦可能
	ImGui::Text("Success rate: %d%%", SUCCESS_RATE);//成功率
	ImGui::Text("Number of consecutive successes: %d", combo); //連続成功数
	ImGui::Text("%s", success ? "Success！" : "Failure…");//成功or失敗

	ImGui::End();
	
	ImGui::Begin("Top 5 Combo Ranking");


	

	for (size_t i = 0; i < ranking.size(); ++i) {
		ImGui::Text("%d位: %s - %d combo", (int)i + 1, ranking[i].name.c_str(), ranking[i].score);
	}

	ImGui::End();

}


// 成功回数が5位以内より高ければランキング登録
void TitleScene::TryInsertToRanking(const std::string& playerName, int combo) {
	ranking.push_back((playerName, combo));
	std::sort(ranking.begin(), ranking.end(), CompareScore);
	if (ranking.size() > 5) {
		ranking.resize(5); // 上位5件に制限
	}
}

//ランキングデータの読み込み
void TitleScene::LoadRankingFromJson(const std::string& jsonText) {
	json j = json::parse(jsonText);
	ranking.clear();
	for (auto& item : j["ranking"]) {
		ranking.push_back(( item["name"], item["score"] ));
	}
}

//ランキングデータの保存
std::string TitleScene::CreateRankingJson() {
	json j;
	for (const auto& entry : ranking) {
		j["ranking"].push_back({ {"name", entry.name}, {"score", entry.score} });
	}
	return j.dump();
}

std::string GetRankingFromServer() {
	// curlでGETリクエストしてJSON文字列を受け取る
}

void PostRankingToServer(const std::string& jsonData) {
	// curlでPOSTする
}


void TitleScene::Draw(){

}

