#pragma once
#include <vector>
#include <memory>
class Actor;
class Player;

class State
{
public:
	// コンストラクタ
	State(std::weak_ptr<Actor> actor) :owner(actor) {}
	virtual ~State() {}
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	virtual void Enter() = 0;
	// ステートで実行するメソッド
	virtual void Execute(float elapsedTime) = 0;
	// ステートから出ていくときのメソッド
	virtual void Exit() = 0;
protected:
	std::weak_ptr<Actor> owner;
};

//class PlayerState
//{
//public:
//	// コンストラクタ
//	PlayerState(std::weak_ptr<Actor> actor) :owner(actor) {}
//	virtual ~PlayerState() {}
//	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
//	// ステートに入った時のメソッド
//	virtual void Enter() = 0;
//	// ステートで実行するメソッド
//	virtual void Execute(float elapsedTime) = 0;
//	// ステートから出ていくときのメソッド
//	virtual void Exit() = 0;
//
//protected:
//	std::shared_ptr<Player> GetPlayerId()
//	{
//		auto ptr = owner.lock();
//		std::shared_ptr<Player> ptrP = nullptr;
//		if (ptr)
//			ptrP = ptr->GetComponent<Player>();
//		//std::shared_ptr<Player> ptrP = ptr ? ptr->GetComponent<Player>() : nullptr;
//		return ptrP;
//	}
//
//protected:
//	std::weak_ptr<Actor> owner;
//};
