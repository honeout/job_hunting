#include "Messenger.h"

void Messenger::Clear()
{
	receivers.clear();
}

void Messenger::SendData(const std::string& identifier, void* data)
{
	// �w��̃L�[�̊J�n�m�[�h�I���m�[�h���擾
	auto itRange = receivers.equal_range(identifier);
	for(decltype(itRange.first) it = itRange.first; it != itRange.second; ++it )
	{
		it->second.func(data);
	}
}



uint64_t Messenger::AddReceiver(const std::string& identifier, Receiver receiver)
{
	receivers.insert(std::make_pair(identifier, Data( incrementKey, receiver ) ));
	return incrementKey++;
}

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

