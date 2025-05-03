#include "Messenger.h"

// �S�폜
void Messenger::Clear()
{
	receivers.clear();
}
// �o�^
void Messenger::SendData(const std::string& identifier, void* data)
{
	// �w��̃L�[�̊J�n�m�[�h�I���m�[�h���擾
	auto itRange = receivers.equal_range(identifier);
	for(decltype(itRange.first) it = itRange.first; it != itRange.second; ++it )
	{
		it->second.func(data);
	}
}
// �֐��o�^(����֐��𕡐��o�^�ł���̂Œ���)
uint64_t Messenger::AddReceiver(const std::string& identifier, Receiver receiver)
{
	receivers.insert(std::make_pair(identifier, Data( incrementKey, receiver ) ));
	return incrementKey++;
}
// �֐��o�^����(�o�^���̃L�[���K�v)
void Messenger::RemoveReceiver(uint64_t key)
{
	auto it = receivers.begin();
	auto itE = receivers.end();
	while(it != itE)
	{
		if( it->second.key == key )
		{
			receivers.erase(it);
			break;
		}
		it++;
	}
}

