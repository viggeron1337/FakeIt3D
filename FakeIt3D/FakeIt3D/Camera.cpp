#include "Camera.h"

void Camera::_InitViewMatrix()
{
}

Camera::Camera()
{
	m_position = XMFLOAT4A(0.0f, 0.0f, -1.0f, 0.0f);
	m_target = XMFLOAT4A(0.0f, 0.0f, 0.0f, 0.0f); 
	
	XMVECTOR Mpos; 
	Mpos = XMLoadFloat4A(&m_position); 
	Mpos = XMVectorAdd(Mpos, XMVECTOR{ 0,1,0,0});
	XMStoreFloat4A(&m_up, Mpos); 

	m_angle = 0.0f; 
	m_clientWidth = 0.0f; 
	m_clientHeight = 0.0f; 
	m_nearPlane = 0.0f; 
	m_farPlane = 0.0f; 

	XMStoreFloat4x4A(&m_viewMatrix, XMMatrixIdentity()); 
	XMStoreFloat4x4A(&m_projMatrix, XMMatrixIdentity());
	XMStoreFloat4x4A(&m_orthoMatrix, XMMatrixIdentity());
}

Camera::~Camera()
{
}

Camera::Camera(const Camera & camera)
{
	*this = camera; 
}

Camera & Camera::operator=(const Camera & camera)
{
	m_position = camera.m_position; 
	m_target = camera.m_target;  
	m_up = camera.m_up; 

	//.......
}

void Camera::InitProjMatrix(const float angle, const float client_width, const float client_height,
	const float nearPlane, const float farPlane)
{

}

void Camera::InitOrthoMatrix(const float client_width, const float client_height,
	const float nearPlane, const float farPlane)
{

}

void Camera::onResize(UINT width, UINT height)
{
}

void Camera::Move(XMFLOAT4 direction)
{
}

void Camera::Rotate(XMFLOAT4A axis, float degrees)
{
}

void Camera::SetPosition(XMFLOAT4A & position)
{
}

const XMFLOAT4A & Camera::GetPosition() const
{
	// TODO: insert return statement here
}

void Camera::setTarget(XMFLOAT4A target)
{
}

const XMFLOAT4A & Camera::GetTarget() const
{
	// TODO: insert return statement here
}

const XMFLOAT4A & Camera::GetUp()
{
	XMVECTOR pos;
	XMVECTOR up;
	XMVECTOR Mresult;
	XMFLOAT4A result;

	pos = XMLoadFloat4A(&m_position);
	up = XMLoadFloat4A(&m_up);

	Mresult = XMVectorSubtract(up, pos);

	XMStoreFloat4A(&result, Mresult);

	return result;
}

const XMFLOAT4A Camera::GetLookAt()
{
	XMVECTOR pos; 
	XMVECTOR target; 
	XMVECTOR Mresult; 
	XMFLOAT4A result; 

	pos = XMLoadFloat4A(&m_position); 
	target = XMLoadFloat4A(&m_target);

	Mresult = XMVectorSubtract(target, pos); 

	XMStoreFloat4A(&result, Mresult);

	return result; 
}

const XMFLOAT4X4A Camera::GetViewMatrix()
{
	XMMATRIX view; 
	view = XMLoadFloat4x4A(&m_viewMatrix); 
	view = XMMatrixTranspose(view); 
	XMStoreFloat4x4A(&m_viewMatrix, view);

	return m_viewMatrix; 
}

void Camera::SetAngle(float angle)
{
}

const float & Camera::GetAngle() const
{
	// TODO: insert return statement here
}

void Camera::SetNearPlane()
{
}

void Camera::setFarPlane()
{
}

const XMFLOAT4X4A Camera::GetProjMatrix()
{
	XMMATRIX proj; 
	proj = XMLoadFloat4x4A(&m_projMatrix); 
	proj = XMMatrixTranspose(proj);
	XMStoreFloat4x4A(&m_projMatrix, proj); 
	
	return m_projMatrix; 
}

const XMFLOAT4X4A Camera::GetOrthoMatrix()
{
	XMMATRIX ortho;
	ortho = XMLoadFloat4x4A(&m_orthoMatrix);
	ortho = XMMatrixTranspose(ortho);
	XMStoreFloat4x4A(&m_orthoMatrix, ortho);

	return m_orthoMatrix;
}
