#include "Camera.h"

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

	m_angle = camera.m_angle; 
	m_clientWidth = camera.m_clientWidth; 
	m_clientHeight = camera.m_clientHeight; 
	m_nearPlane = camera.m_nearPlane; 
	m_farPlane = camera.m_farPlane; 

	m_viewMatrix = camera.m_viewMatrix; 
	m_projMatrix = camera.m_projMatrix; 
	m_orthoMatrix = camera.m_orthoMatrix; 

	return *this; 
}

void Camera::_InitViewMatrix()
{
	XMStoreFloat4x4A(&m_viewMatrix, XMMatrixLookAtLH(XMLoadFloat4A(&m_position), XMLoadFloat4A(&m_target),
		XMLoadFloat4A(&m_up))); 

	//Update so that shaders work with the correct data
	XMMATRIX view = XMLoadFloat4x4A(&m_viewMatrix);
	XMMATRIX proj = XMLoadFloat4x4A(&m_projMatrix);
	XMMATRIX wvp = XMLoadFloat4x4A(&m_cBufferData.mvpMatrix);

	wvp = XMMatrixIdentity() * view * proj;
}

void Camera::InitProjMatrix(const float angle, const float client_width, const float client_height,
	const float nearPlane, const float farPlane)
{
	m_angle = angle; 
	m_clientWidth = client_width; 
	m_clientHeight = client_height; 
	m_nearPlane = nearPlane; 
	m_farPlane = farPlane; 

	XMStoreFloat4x4A(&m_projMatrix, XMMatrixPerspectiveFovLH(angle, client_width/client_height, nearPlane, farPlane));

	//Update so that shaders work with the correct data
	XMMATRIX view = XMLoadFloat4x4A(&m_viewMatrix); 
	XMMATRIX proj = XMLoadFloat4x4A(&m_projMatrix); 
	XMMATRIX wvp = XMLoadFloat4x4A(&m_cBufferData.mvpMatrix); 

	wvp = XMMatrixIdentity() * view * proj; 

	XMStoreFloat4x4A(&m_cBufferData.mvpMatrix, wvp); 
}

void Camera::InitOrthoMatrix(const float client_width, const float client_height,
	const float nearPlane, const float farPlane)
{
	m_clientWidth = client_width;
	m_clientHeight = client_height;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

	
	XMStoreFloat4x4(&m_orthoMatrix, XMMatrixOrthographicLH(client_width, client_height, nearPlane, farPlane));
}

void Camera::onResize(UINT width, UINT height)
{
	m_clientWidth = width; 
	m_clientHeight = height; 
	InitProjMatrix(m_angle, width, height, m_nearPlane, m_farPlane);
	InitOrthoMatrix(width, height, m_nearPlane, m_farPlane); 
}

void Camera::Move(XMFLOAT4 direction)
{
	XMVECTOR Mpos, Mtarget, Mup; 

	Mpos = XMLoadFloat4(&m_position); 
	Mtarget = XMLoadFloat4(&m_target);
	Mup = XMLoadFloat4(&m_up);

	Mpos = XMVector4Transform(Mpos, XMMatrixTranslation(direction.x, direction.y, direction.z)); 
	Mtarget = XMVector4Transform(Mtarget, XMMatrixTranslation(direction.x, direction.y, direction.z));
	Mup = XMVector4Transform(Mup, XMMatrixTranslation(direction.x, direction.y, direction.z));

	XMStoreFloat4A(&m_position, Mpos); 
	XMStoreFloat4A(&m_target, Mtarget);
	XMStoreFloat4A(&m_up, Mup);

	this->_InitViewMatrix(); 
}

void Camera::Rotate(XMFLOAT4A axis, float degrees)
{

	XMVECTOR Maxis, Mtarget, Mpos, MlookAtTarget, MlookAtUp, Mup; 

	Maxis = XMLoadFloat4A(&axis); 
	Mtarget = XMLoadFloat4A(&m_target); 
	Mpos = XMLoadFloat4A(&m_position); 
	Mup = XMLoadFloat4A(&m_up);

	if (XMVector4Equal(Maxis, XMVectorZero()) || degrees == 0.0f)
		return; 

	//Create LookAt and Up vector
	MlookAtTarget = Mtarget - Mpos;
	MlookAtUp = Mup - Mpos; 

	//Rotate them around "axis" by "degree".
	MlookAtTarget = DirectX::XMVector4Transform(MlookAtTarget, XMMatrixRotationAxis(Maxis, XMConvertToRadians(degrees))); 
	MlookAtUp = DirectX::XMVector4Transform(MlookAtUp, XMMatrixRotationAxis(Maxis, XMConvertToRadians(degrees)));

	//Update the member vectors according to the new rotation
	XMStoreFloat4A(&m_target, (Mpos + MlookAtTarget)); 
	XMStoreFloat4A(&m_up, (Mpos + MlookAtUp)); 

	//Initiate the new viewMatrix. 
	_InitViewMatrix(); 
}

void Camera::SetPosition(XMFLOAT4A & position)
{
	XMVECTOR MnewPos, Mpos; 
	XMFLOAT4A moveVector;

	Mpos = XMLoadFloat4A(&m_position);
	MnewPos = XMLoadFloat4A(&position); 

	XMStoreFloat4A(&moveVector, MnewPos - Mpos);
	this->Move(moveVector); 
	this->setTarget(m_target); 
}

const XMFLOAT4A & Camera::GetPosition() const
{
	return m_position; 
}

void Camera::setTarget(XMFLOAT4A target)
{
	XMVECTOR MnewTarget, Mtarget, Mpos, MoldLookAt, MnewLookAtTarget; 
	XMFLOAT4A axis; 

	Mtarget = XMLoadFloat4A(&m_target);
	Mpos = XMLoadFloat4A(&m_position); 
	MnewTarget = XMLoadFloat4A(&target); 

	//If new target is the same as pos or the old target, don't bother. 
	if (XMVector4Equal(MnewTarget, Mpos) || XMVector4Equal(MnewTarget, Mtarget))
		return; 

	//Old and new look at target.
	MoldLookAt = Mtarget - Mpos; 
	MnewLookAtTarget = MnewTarget - Mpos; 

	//Get the rotation angle between the old angle and the new one. 
	float angle = XMConvertToDegrees(XMVectorGetX(XMVector4AngleBetweenVectors(XMVector4Normalize(MoldLookAt),
		XMVector4Normalize(MnewLookAtTarget)))); 

	//Make sure the angle isn't the same or 180.
	if (angle != 0.0f && angle != 360.0f && angle != 180.0f)
	{
		//Get the axis the camera is rotating around. 
		XMVECTOR Maxis = XMVector3Cross(MoldLookAt, MnewLookAtTarget); 
		XMStoreFloat4A(&axis, Maxis); 

		//Rotate with angle around the axis
		Rotate(axis, angle); 
	}

	//Update old target. 
	XMStoreFloat4A(&m_target, MnewTarget); 

	//Initiate the new viewMatrix. 
	_InitViewMatrix(); 
}

const XMFLOAT4A & Camera::GetTarget() const
{
	return m_target; 
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
	m_angle = angle; 
	InitProjMatrix(m_angle, m_clientWidth, m_clientHeight, m_nearPlane, m_farPlane); 
}

const float & Camera::GetAngle() const
{
	return m_angle; 
}

void Camera::SetNearPlane(float nearest)
{
	m_nearPlane = nearest; 
	onResize(m_clientWidth, m_clientHeight); 
}

void Camera::setFarPlane(float farhest)
{
	m_farPlane = farhest;
	onResize(m_clientWidth, m_clientHeight);
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
