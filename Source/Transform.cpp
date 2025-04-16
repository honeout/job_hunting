#include <imgui.h>
#include "Transform.h"
#include "Mathf.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

// modelをワールド位置に
void Transform::UpdateTransform()
{
	// スケールだけ行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列作成
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
	// 位置行列だけを作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// 3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;// 行列は計算順番変えると結果が変わる
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

void Transform::UpdateTransformProjectile()
{
	// 何か
	DirectX::XMVECTOR Front, Up, Right;
	// 前ベクトルを算出　まず向きを手に入れる
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);
	// 仮の上ベクトルを算出　少しおかしい値を入れておく真上に打った時に
	// ここと同じ値だとバグるから
	Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
	Up = DirectX::XMVector3Normalize(Up);
	// 右ベクトル算出　上二つを使い一つの新しい軸を作る
	Right = DirectX::XMVector3Cross(Up, Front);// ２つで１つの奴を出す
	Right = DirectX::XMVector3Normalize(Right);
	// 上ベクトル算出 さらに作った二つでさらに新しいひとつを作る。
	Up = DirectX::XMVector3Cross(Front, Right);// 斜め上の奴出来る
	// 計算結果を取り出し
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);
	// 算出した軸ベクトルから行列を作成 回転あり
	transform._11 = right.x * scale.x; // 大きさだけやった奴０．１倍ならそうなる
	transform._12 = right.y * scale.x;// 角度
	transform._13 = right.z * scale.x;
	transform._14 = 0.0f;
	transform._21 = up.x * scale.y;
	transform._22 = up.y * scale.y;// 裏返る
	transform._23 = up.z * scale.y;
	transform._24 = 0.0f;
	transform._31 = front.x * scale.z;
	transform._32 = front.y * scale.z;
	transform._33 = front.z * scale.z;// 向き
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;
	this->direction = front;// 向きをこっちに入れるこれでむきの計算
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
	//// Maxポジション
	//maxPos.x += position.x;
	//maxPos.y += position.y;
	//maxPos.z += position.z;

	//// Minポジション
	//minPos.x += position.x;
	//minPos.y += position.y;
	//minPos.z += position.z;

	position.x += Mathf::RandomRange(minPos.x, maxPos.x);
	position.y += Mathf::RandomRange(minPos.y, maxPos.y);
	position.z += Mathf::RandomRange(minPos.z, maxPos.z);
}

