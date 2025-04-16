#include <imgui.h>
#include "Transform.h"
#include "Mathf.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

// model�����[���h�ʒu��
void Transform::UpdateTransform()
{
	// �X�P�[�������s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s��쐬
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
	// �ʒu�s�񂾂����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// 3�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;// �s��͌v�Z���ԕς���ƌ��ʂ��ς��
	// �v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

void Transform::UpdateTransformProjectile()
{
	// ����
	DirectX::XMVECTOR Front, Up, Right;
	// �O�x�N�g�����Z�o�@�܂���������ɓ����
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);
	// ���̏�x�N�g�����Z�o�@�������������l�����Ă����^��ɑł�������
	// �����Ɠ����l���ƃo�O�邩��
	Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
	Up = DirectX::XMVector3Normalize(Up);
	// �E�x�N�g���Z�o�@�����g����̐V�����������
	Right = DirectX::XMVector3Cross(Up, Front);// �Q�łP�̓z���o��
	Right = DirectX::XMVector3Normalize(Right);
	// ��x�N�g���Z�o ����ɍ������ł���ɐV�����ЂƂ����B
	Up = DirectX::XMVector3Cross(Front, Right);// �΂ߏ�̓z�o����
	// �v�Z���ʂ����o��
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);
	// �Z�o�������x�N�g������s����쐬 ��]����
	transform._11 = right.x * scale.x; // �傫������������z�O�D�P�{�Ȃ炻���Ȃ�
	transform._12 = right.y * scale.x;// �p�x
	transform._13 = right.z * scale.x;
	transform._14 = 0.0f;
	transform._21 = up.x * scale.y;
	transform._22 = up.y * scale.y;// ���Ԃ�
	transform._23 = up.z * scale.y;
	transform._24 = 0.0f;
	transform._31 = front.x * scale.z;
	transform._32 = front.y * scale.z;
	transform._33 = front.z * scale.z;// ����
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;
	this->direction = front;// �������������ɓ���邱��łނ��̌v�Z
}

#ifdef _DEBUG
void Transform::OnGUI()
{
    ImGui::InputFloat3("Position", &position.x);
    ImGui::InputFloat3("Angle", &angle.x);
    ImGui::InputFloat3("Scale", &scale.x);
}
#endif // _DEBUG
void Transform::SpawnRandomInArea(DirectX::XMFLOAT3 maxPos, DirectX::XMFLOAT3 minPos)
{
	//// Max�|�W�V����
	//maxPos.x += position.x;
	//maxPos.y += position.y;
	//maxPos.z += position.z;

	//// Min�|�W�V����
	//minPos.x += position.x;
	//minPos.y += position.y;
	//minPos.z += position.z;

	position.x += Mathf::RandomRange(minPos.x, maxPos.x);
	position.y += Mathf::RandomRange(minPos.y, maxPos.y);
	position.z += Mathf::RandomRange(minPos.z, maxPos.z);
}

