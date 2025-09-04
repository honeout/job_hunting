#include "Messenger.h"

// ‘Síœ
void Messenger::Clear()
{
	receivers.clear();
}
// “o˜^
void Messenger::SendData(const std::string& identifier, void* data)
{
	// w’è‚ÌƒL[‚ÌŠJnƒm[ƒhI—¹ƒm[ƒh‚ğæ“¾
	auto itRange = receivers.equal_range(identifier);
	for(decltype(itRange.first) it = itRange.first; it != itRange.second; ++it )
	{
		it->second.func(data);
	}
}
// ŠÖ”“o˜^(“¯ˆêŠÖ”‚ğ•¡”“o˜^‚Å‚«‚é‚Ì‚Å’ˆÓ)
uint64_t Messenger::AddReceiver(const std::string& identifier, Receiver receiver)
{
	receivers.insert(std::make_pair(identifier, Data( incrementKey, receiver ) ));
	return incrementKey++;
}
// ŠÖ”“o˜^‰ğœ(“o˜^‚ÌƒL[‚ª•K—v)
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

