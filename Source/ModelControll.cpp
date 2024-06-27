#include "ModelControll.h"

ModelControll::ModelControll()
{
}

ModelControll::~ModelControll()
{
    //if (model != nullptr)
    //{
    //   delete model;
    //   model = nullptr;
    //}
}

void ModelControll::Start()
{

}

void ModelControll::Update(float elapsedTime)
{
}

void ModelControll::OnGUI()
{

}

void ModelControll::LoadModel(const char* filename)
{
    model = std::make_unique<Model>(filename);
}
