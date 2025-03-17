#include "GlobalVariables.h"

GlobalVariables* GlobalVariables::GatInstance()
{
    static GlobalVariables instance;
    return &instance;
}

void GlobalVariables::CreateGroup(const std::string& groupName)
{
    //指定名のオブジェクトがなければ追加する
    datas_[groupName];
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value){
    //グループの参照を取得
    Group& group = datas_[groupName];
    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;
    //設定した項目をstd::mapに追加
    group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value){
    //グループの参照を取得
    Group& group = datas_[groupName];

    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;
    //設定した項目をstd::mapに追加
    group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value){
    //グループの参照を取得
    Group& group = datas_[groupName];
    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;
    //設定した項目をstd::mapに追加
    group.items[key] = newItem;

}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector4& value){
    //グループの参照を取得
    Group& group = datas_[groupName];
    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;
    //設定した項目をstd::mapに追加
    group.items[key] = newItem;
}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const{
    // 指定グループが存在するか確認
    auto itGroup = datas_.find(groupName);
    assert(itGroup != datas_.end());

    // グループの参照を取得
    const Group& group = itGroup->second;

    // 指定キーが存在するか確認
    auto itItem = group.items.find(key);
    assert(itItem != group.items.end());

    // 項目がint32_t型か確認
    assert(std::holds_alternative<int32_t>(itItem->second.value));

    // 値を取得して返す
    return std::get<int32_t>(itItem->second.value);

}

float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const{
    // 指定グループが存在するか確認
    auto itGroup = datas_.find(groupName);
    assert(itGroup != datas_.end());

    // グループの参照を取得
    const Group& group = itGroup->second;

    // 指定キーが存在するか確認
    auto itItem = group.items.find(key);
    assert(itItem != group.items.end());

    // 項目がfloat型か確認
    assert(std::holds_alternative<float>(itItem->second.value));

    // 値を取得して返す
    return std::get<float>(itItem->second.value);
}

Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const{
       
    // 指定グループが存在するか確認
    auto itGroup = datas_.find(groupName);
    assert(itGroup != datas_.end());

    // グループの参照を取得
    const Group& group = itGroup->second;

    // 指定キーが存在するか確認
    auto itItem = group.items.find(key);
    assert(itItem != group.items.end());

    // 項目がfloat型か確認
    assert(std::holds_alternative<Vector3>(itItem->second.value));

    // 値を取得して返す
    return std::get<Vector3>(itItem->second.value);
}

Vector4 GlobalVariables::GetVector4Value(const std::string& groupName, const std::string& key) const{
    // 指定グループが存在するか確認
    auto itGroup = datas_.find(groupName);
    assert(itGroup != datas_.end());

    // グループの参照を取得
    const Group& group = itGroup->second;

    // 指定キーが存在するか確認
    auto itItem = group.items.find(key);
    assert(itItem != group.items.end());

    // 項目がfloat型か確認
    assert(std::holds_alternative<Vector4>(itItem->second.value));

    // 値を取得して返す
    return std::get<Vector4>(itItem->second.value);
}

void GlobalVariables::Update(){
    if (!ImGui::Begin("Global Variables",nullptr , ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }
 

    // メニューバーの開始
    if (ImGui::BeginMenuBar()) {
        // 各グループのループ
        for (std::map<std::string, Group>::iterator itGroup = datas_.begin();
            itGroup != datas_.end(); ++itGroup) {

            // グループ名を取得
            const std::string& groupName = itGroup->first;
            // グループの参照を取得
            Group& group = itGroup->second;

            // メニューの開始
            if (ImGui::BeginMenu(groupName.c_str())) {
                // 各項目のループ
                for (std::map<std::string, Item>::iterator itItem = group.items.begin();
                    itItem != group.items.end(); ++itItem) {
                    // 項目名を取得
                    const std::string& itemName = itItem->first;
                    // 項目の参照を取得
                    Item& item = itItem->second;
                    if (ImGui::TreeNode(itemName.c_str())) {

                        // int32_t 型の値を保持していれば
                        if (std::holds_alternative<int32_t>(item.value)) {
                            int32_t* ptr = std::get_if<int32_t>(&item.value);
                            ImGui::DragInt(itemName.c_str(), ptr, 0, 100);
                        }
                        // float 型の値を保持していれば
                        else if (std::holds_alternative<float>(item.value)) {
                            float* ptr = std::get_if<float>(&item.value);
                            ImGui::DragFloat(itemName.c_str(), ptr, 0, 100);
                        }
                        // Vector3 型の値を保持していれば
                        else if (std::holds_alternative<Vector3>(item.value)) {
                            Vector3* ptr = std::get_if<Vector3>(&item.value);
                            ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f, -10.0f, 10.0f);
                        }
                        // Vector4 型の値を保持していれば
                        else if (std::holds_alternative<Vector4>(item.value)) {
                            Vector4* ptr = std::get_if<Vector4>(&item.value);
                            ImGui::DragFloat4(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f, -10.0f, 10.0f);
                        }
                        //改行
                        ImGui::Text("\n");

                        if (ImGui::Button("Save")) {
                            SaveFile(groupName);
                            std::string message = std::format("{}.json saved.", groupName);
                            MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::EndMenu(); // メニューを閉じる
            }
        }
        ImGui::EndMenuBar(); // メニューバーを閉じる
    }

    ImGui::End(); // ウィンドウを閉じる

}

void GlobalVariables::SaveFile(const std::string& groupName) {
    //グループを検索
    std::map < std::string, Group>::iterator itGroup = datas_.find(groupName);

    //未登録テェック
    assert(itGroup != datas_.end());

    json root;

    root = json::object();

    //jsonオブジェクト登録
    root[groupName] = json::object();

    //各項目について
    for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
        itItem != itGroup->second.items.end(); ++itItem) {

        //各項目を取得
        const std::string& itemName = itItem->first;
        //項目の参照を取得
        Item& item = itItem->second;

        //int32_t型の値を保持していれば
        if (std::holds_alternative<int32_t>(item.value)) {
            //int32_t型の値を登録
            root[groupName][itemName] = std::get<int32_t>(item.value);
        }
        //float型の値を保持していれば
        else if (std::holds_alternative<float>(item.value)) {
            //float型の値を登録
            root[groupName][itemName] = std::get<float>(item.value);
        }
        //Vector3型の値を保持していれば
        else if (std::holds_alternative<Vector3>(item.value)) {
            //float型のjson配列登録
            Vector3 value = std::get<Vector3>(item.value);
            root[groupName][itemName] = json::array({ value.x,value.y,value.z });
        }
        //Vector4型の値を保持していれば
        else if (std::holds_alternative<Vector4>(item.value)) {
            //float型のjson配列登録
            Vector4 value = std::get<Vector4>(item.value);
            root[groupName][itemName] = json::array({ value.x,value.y,value.z,value.w });
        }
    }

    //ディレクトリがなければ作成する
    std::filesystem::path dir(kDirectoryPath);
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directory(dir);
    }

    //書き込むJSONファイルを合成する
    std::string filePath = kDirectoryPath + groupName + ".json";
    //書き込み用ファイルストリーム
    std::ofstream ofs;
    //ファイルを書き込み用に開く
    ofs.open(filePath);

    //ファイルオープン失敗？
    if (ofs.fail()) {
        std::string message = "Failed open data file for write.";
        MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
        assert(0);
        return;
    }
    //ファイルにjson文字列を書き込む(インデント幅4)
    ofs << std::setw(4) << root << std::endl;
    //ファイルを閉じる
    ofs.close();


}

void GlobalVariables::LoadFiles(){
    // 保存先ディレクトリのパスをローカル変数で宣言
    std::filesystem::path saveDir(kDirectoryPath);

    // ディレクトリが存在しなければスキップ
    if (!std::filesystem::exists(saveDir)) {
        std::cerr << "Error: 保存先ディレクトリが存在しません → " << saveDir << std::endl;
        return;
    }
    //各ファイルの処理
    std::filesystem::directory_iterator dir_it(saveDir);
    for (const std::filesystem::directory_entry& entry : dir_it) {
        //ファイルパスを取得
        const std::filesystem::path& filePath = entry.path();

        //ファイル拡張子を取得
        std::string extension = filePath.extension().string();

        //.jsonファイル以外はスキップ
        if (extension.compare(".json") != 0) {
            continue;
        }

        //ファイル読み込み
        LoadFile(filePath.stem().string());

    }

}

void GlobalVariables::LoadFile(const std::string& groupName){
    //読み込みJSONファイルのフルパスを合成する
    std::string filePath = kDirectoryPath + groupName + ".json";
    // 読み込み用ファイルストリーム
    std::ifstream ifs(filePath);  

    // ファイルオープン失敗？
    if (ifs.fail()) {
        std::string message = "Failed to open data file for read: " + filePath;
        MessageBoxA(nullptr, message.c_str(), "GlobalVariables", MB_ICONERROR);
        assert(0);
        return;
    }

    json root;

    //json文字列からjsonのデータ構造に展開
    ifs >> root;
    //ファイルを閉じる
    ifs.close();


    //グループを検索
    json::iterator itGroup = root.find(groupName);

    //未登録チェック
    assert(itGroup != root.end());

    //各アイテムについて
    for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
        //アイテム名を取得
        const std::string& itemName = itItem.key();

        //int32_t型の値を保持していれば
        if (itItem->is_number_integer()) {
            //int型の値を登録
            int32_t value = itItem->get<int32_t>();
            SetValue(groupName, itemName, value);
        }
        //float型の値を保持していれば
        else if (itItem->is_number_float()) {
            //float型の値を登録
            double value = itItem->get<double>();
            SetValue(groupName, itemName, static_cast<float>(value));
        }
        //要素数3の配列があれば
        else if (itItem->is_array() && itItem->size() == 3) {
            //float型のjson配列登録
            Vector3 value = { itItem->at(0),itItem->at(1),itItem->at(2) };
            SetValue(groupName, itemName, value);
        }
        //要素数4の配列があれば
        else if (itItem->is_array() && itItem->size() == 4) {
            //float型のjson配列登録
            Vector4 value = { itItem->at(0),itItem->at(1),itItem->at(2),itItem->at(3) };
            SetValue(groupName, itemName, value);
        }
    }
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {
    // グループを検索
    auto itGroup = datas_.find(groupName);

    Group& group = itGroup->second;

    // 項目が未登録なら
    if (group.items.find(key) == group.items.end()) {
        SetValue(groupName, key, value);
    }
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value){
    // グループを検索
    auto itGroup = datas_.find(groupName);

    Group& group = itGroup->second;

    // 項目が未登録なら
    if (group.items.find(key) == group.items.end()) {
        SetValue(groupName, key, value);
    }
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value){
     // グループを検索
    auto itGroup = datas_.find(groupName);

    Group& group = itGroup->second;

    // 項目が未登録なら
    if (group.items.find(key) == group.items.end()) {
        SetValue(groupName, key, value);
    }
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector4& value){
    // グループを検索
    auto itGroup = datas_.find(groupName);

    Group& group = itGroup->second;

    // 項目が未登録なら
    if (group.items.find(key) == group.items.end()) {
        SetValue(groupName, key, value);
    }
}
