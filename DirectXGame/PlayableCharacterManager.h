#pragma once

#include"Player.h"

#include<vector>

/// <summary>
/// 操作キャラ管理クラス
/// </summary>
class PlayableCharacterManager{
public:
	PlayableCharacterManager() = default;
	~PlayableCharacterManager() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 操作キャラクターの追加
	/// </summary>
	/// <param name="player"></param>
	void AddCharacters(Player* player);

	/// <summary>
	/// 現在の操作キャラクターの取得
	/// </summary>
	/// <returns></returns>
	Player* GetCurrentCharacter();

	/// <summary>
	/// 操作キャラクターの変更
	/// </summary>
	/// <param name="newIndex"></param>
	void SwitchCharacter(int newCharacterIndex);

private:
	//操作可能キャラクター
	std::vector<Player*> characters_;
	//現在操作しているキャラクター番号
	int currentCharacterIndex_;
};

