#pragma once

#include<cstdint>
#include<vector>

/// <summary>
/// 接触記録
/// </summary>
class ContactRecord{
public:
	ContactRecord() = default;
	~ContactRecord() = default;

	/// <summary>
	/// 履歴の登録
	/// </summary>
	/// <param name="number"></param>
	void AddRecord(uint32_t number);

	/// <summary>
	/// 履歴のチェック
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	bool CheckRecord(uint32_t number);

	/// <summary>
	/// 履歴のClear
	/// </summary>
	/// <returns></returns>
	void Clear();

private:
	std::vector<uint32_t>record_;
};

