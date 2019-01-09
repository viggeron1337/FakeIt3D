#include "Camera.h"

void Camera::_InitViewMatrix()
{
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(const Camera & camera)
{
}

Camera & Camera::operator=(const Camera & camera)
{
	// TODO: insert return statement here
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
	// TODO: insert return statement here
}

const XMFLOAT4A Camera::GetLookAt()
{
	return XMFLOAT4A();
}

const XMFLOAT4X4A Camera::GetViewMatrix()
{
	return XMFLOAT4X4A();
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
	return XMFLOAT4X4A();
}

const XMFLOAT4X4A Camera::GetOrthoMatrix()
{
	return XMFLOAT4X4A();
}
