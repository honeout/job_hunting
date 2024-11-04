#include "SpriteControll.h"

SpriteControll::SpriteControll()
{
}

SpriteControll::~SpriteControll()
{
}

#ifdef _DEBUG
void SpriteControll::OnGUI()
{
}
#endif // _DEBUG
void SpriteControll::LoadSprite(const char* filename)
{
    sprite = std::make_unique<Sprite>(filename);
}

