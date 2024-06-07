#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Jump.h"

// �R���X�g���N�^
Jump::Jump()
{

}

// �f�X�g���N�^
Jump::~Jump()
{

}

// GUI�`��
void Jump::OnGUI()
{
	ImGui::InputInt("Jump max", &jumpCount);
	//ImGui::InputFloat("Turn Speed", &turnSpeed);
}

// ���[���h�ړ�
bool Jump::OnLanding()
{
	std::shared_ptr<Actor> actor = GetActor();
	// ���n�������烊�Z�b�g
	jumpCount = 0;
	velocity = actor->GetVelocity();
	//// �������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��  �\���ȑ��x�ŗ��Ƃ��d�͂̂T�{�Q�A�R�b��ɒ��n���[�V����������B
	if (velocity.y < gravity * 5.0f)
	{
		//if (state != State::Damage && state != State::Death)
		//{
		//	// ���n�X�e�[�g�֑J��
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
//// ���[�J���ړ�
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
//// ����
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
