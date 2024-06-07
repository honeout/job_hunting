#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Jump.h"

// コンストラクタ
Jump::Jump()
{

}

// デストラクタ
Jump::~Jump()
{

}

// GUI描画
void Jump::OnGUI()
{
	ImGui::InputInt("Jump max", &jumpCount);
	//ImGui::InputFloat("Turn Speed", &turnSpeed);
}

// ワールド移動
bool Jump::OnLanding()
{
	std::shared_ptr<Actor> actor = GetActor();
	// 着地したからリセット
	jumpCount = 0;
	velocity = actor->GetVelocity();
	//// 下方向の速力が一定以上なら着地ステートへ  十分な速度で落とす重力の５倍２、３秒後に着地モーションをする。
	if (velocity.y < gravity * 5.0f)
	{
		//if (state != State::Damage && state != State::Death)
		//{
		//	// 着地ステートへ遷移
		//	TransitionLandState();
		//}
		return true;
	}
}

void Jump::JumpVelocity(float speed)
{
	velocity.y += speed;
	//position.y = (std::min)(position.y, speed);
}

//
//// ローカル移動
//void Jump::MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime)
//{
//	std::shared_ptr<Actor> actor = GetActor();
//	float speed = moveSpeed * elapsedTime;
//	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
//	DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);
//	DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&actor->GetRotation());
//	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
//	DirectX::XMVECTOR Move = DirectX::XMVector3TransformNormal(Velocity, Transform);
//	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&actor->GetPosition());
//
//	Position = DirectX::XMVectorAdd(Position, Move);
//
//	DirectX::XMFLOAT3 position;
//	DirectX::XMStoreFloat3(&position, Position);
//	actor->SetPosition(position);
//}
//
//// 旋回
//void Jump::Turn(const DirectX::XMFLOAT3& direction, float elapsedTime)
//{
//	std::shared_ptr<Actor> actor = GetActor();
//	float speed = turnSpeed * elapsedTime;
//	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
//	DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&actor->GetRotation());
//	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
//	DirectX::XMVECTOR OneZ = DirectX::XMVectorSet(0, 0, 1, 0);
//	DirectX::XMVECTOR Front = DirectX::XMVector3TransformNormal(OneZ, Transform);
//
//	Direction = DirectX::XMVector3Normalize(Direction);
//	DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Front, Direction);
//	if (DirectX::XMVector3Equal(Axis, DirectX::XMVectorZero()))
//	{
//		return;
//	}
//
//	DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Front, Direction);
//
//	float dot;
//	DirectX::XMStoreFloat(&dot, Dot);
//	speed = (std::min)(1.0f - dot, speed);
//
//	DirectX::XMVECTOR Turn = DirectX::XMQuaternionRotationAxis(Axis, speed);
//	Rotation = DirectX::XMQuaternionMultiply(Rotation, Turn);
//
//	DirectX::XMFLOAT4 rotation;
//	DirectX::XMStoreFloat4(&rotation, Rotation);
//	actor->SetRotation(rotation);
//}
