#include "ModelControll.h"

ModelControll::ModelControll()
{
}

ModelControll::~ModelControll()
{
}

void ModelControll::Start()
{
}

void ModelControll::Update(float elapsedTime)
{
}
#ifdef _DEBUG
void ModelControll::OnGUI()
{
}
#endif // _DEBUG
void ModelControll::LoadModel(const char* filename)
{
    model = std::make_unique<Model>(filename);
}
