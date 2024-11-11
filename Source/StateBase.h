#pragma once
#include <vector>
#include <memory>
//class EnemySlime;
//class Player;
class Actor;


class State
{
public:
	// �R���X�g���N�^
	State(Actor* actor) :owner(actor) {}
	virtual ~State() {}
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Enter() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Execute(float elapsedTime) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void Exit() = 0;


protected:
	Actor* owner;
};
