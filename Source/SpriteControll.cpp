#include "SpriteControll.h"

SpriteControll::SpriteControll()
{
}

SpriteControll::~SpriteControll()
{
}

void SpriteControll::OnGUI()
{
}

void SpriteControll::LoadSprite(const char* filename)
{
    sprite = std::make_unique<Sprite>(filename);
}

