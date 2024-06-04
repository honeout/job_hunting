#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"

// コンストラクタ
Movement::Movement()
{

}

// デストラクタ
Movement::~Movement()
{

}

// GUI描画
void Movement::OnGUI()
{
	ImGui::InputFloat("Move Speed", &moveSpeed);
	ImGui::InputFloat("Turn Speed", &turnSpeed);
}

// ワールド移動
void Movement::Move(const DirectX::XMFLOAT3& direction, float elapsedTime)
{
	std::shared_ptr<Actor> actor = GetActor();
	float speed = moveSpeed * elapsedTime;
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);
	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&actor->GetPosition());

	Position = DirectX::XMVectorAdd(Position, Velocity);

	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, Position);
	actor->SetPosition(position);
}

// ローカル移動
void Movement::MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime)
{
	std::shared_ptr<Actor> actor = GetActor();
	float speed = moveSpeed * elapsedTime;
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);
	DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&actor->GetRotation());
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
	DirectX::XMVECTOR Move = DirectX::XMVector3TransformNormal(Velocity, Transform);
	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&actor->GetPosition());

	Position = DirectX::XMVectorAdd(Position, Move);

	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, Position);
	actor->SetPosition(position);
}

// 旋回
void Movement::Turn(const DirectX::XMFLOAT3& direction, float elapsedTime)
{
	std::shared_ptr<Actor> actor = GetActor();
	float speed = turnSpeed * elapsedTime;
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&actor->GetRotation());
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
	DirectX::XMVECTOR OneZ = DirectX::XMVectorSet(0, 0, 1, 0);
	DirectX::XMVECTOR Front = DirectX::XMVector3TransformNormal(OneZ, Transform);

	Direction = DirectX::XMVector3Normalize(Direction);
	DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Front, Direction);
	if (DirectX::XMVector3Equal(Axis, DirectX::XMVectorZero()))
	{
		return;
	}

	DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Front, Direction);

	float dot;
	DirectX::XMStoreFloat(&dot, Dot);
	speed = (std::min)(1.0f - dot, speed);

	DirectX::XMVECTOR Turn = DirectX::XMQuaternionRotationAxis(Axis, speed);
	Rotation = DirectX::XMQuaternionMultiply(Rotation, Turn);

	DirectX::XMFLOAT4 rotation;
	DirectX::XMStoreFloat4(&rotation, Rotation);
	actor->SetRotation(rotation);
}
