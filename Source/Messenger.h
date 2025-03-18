#pragma once

#include <functional>
#include <string>
#include <map>


class Messenger
{
public:
	// ���b�Z�[�W��M�֐�
	using Function = void(void*);
	using Receiver = std::function<Function>;

	static constexpr const uint64_t InvalidKey = 0xFFFFFFFFFFFFFFFF;

private:
	Messenger() {}
	~Messenger() {}

public:
	// �B��̃C���X�^���X�擾
	static Messenger& Instance()
	{
		static Messenger instance;
		return instance;
	}

	// ��M�֐��o�^����
	void Clear();

	// �f�[�^���M
	void SendData(const std::string& identifier, void* data);

	// �֐��o�^(����֐��𕡐��o�^�ł���̂Œ���)
	uint64_t AddReceiver(const std::string& identifier, Receiver receiver);

	// �֐��o�^����(�o�^���̃L�[���K�v)
	void RemoveReceiver(uint64_t key);
private:
	struct Data
	{
		uint64_t	key;
		Receiver	func;
		Data( uint64_t key, Receiver func ) : key( key ), func( func ){}
		bool operator==( Data& r )
		{
			return key == r.key;
		}
	};
	std::multimap<std::string, Data> receivers;
	uint64_t incrementKey = 0;
};
