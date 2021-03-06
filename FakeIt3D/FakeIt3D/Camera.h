#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Extern.h"
#include "Object.h"

using namespace DirectX; 


class Camera 
{

public:
	struct VS_CONSTANT_BUFFER
	{
		XMFLOAT4X4A wvpMatrix;
	};

private:

	//Intiliazie cameras View Matrix based upon m_position, m_target and m_Up.
	void _InitViewMatrix();

	ID3D11Buffer* m_CcameraBuffer; 

	VS_CONSTANT_BUFFER m_cBufferData; 
	void createConstantBuffer(); 

public:
	
	//Construct a default camera looking at {0,0,0}, placed at {0,0,-1}
	//with up-vector {0,1,0} (m_up is not a vector, but a point). 
	Camera(); 
	~Camera(); 

	void init(); 

	//Create camera from other camera (copy constructor). 
	Camera(const Camera& camera);

	//Operator to make Copy Constructor work properly
	Camera& operator=(const Camera& camera); 

	void InitProjMatrix(const float angle, const float client_width, const float client_height,
		const float nearPlane, const float farPlane); 

	//If sometime needed.
	void InitOrthoMatrix(const float client_width, const float client_height, 
	const float nearPlane, const float farPlane); 
	
	void onResize(UINT width, UINT height); 

	//////////////////////////////////////////////////////////////////////////////
	/********************View Matrix Transformation Interfaces******************/
	/////////////////////////////////////////////////////////////////////////////

	void Move(XMFLOAT4 direction); 

	//Rotates camera around "axis". by "degrees". Here, the cameras postion is the pivot point.
	void Rotate(XMFLOAT4A axis, float degrees); 

	void SetPosition(XMFLOAT4A& position); 

	const XMFLOAT4A& GetPosition() const;

	//Change camera target position
	void setTarget(XMFLOAT4A target); 

	const XMFLOAT4A& GetTarget() const; 

	//Get cameras up-vector
	const XMFLOAT4A& GetUp(); 

	//Get the look at vector of the camera. 
	const XMFLOAT4A GetLookAt();

	//Get transposed view matrix
	const XMFLOAT4X4A GetViewMatrix(); 
	
	//////////////////////////////////////////////////////////////////////////////
	/****************Projection Matrix Transformation Interfaces****************/
	/////////////////////////////////////////////////////////////////////////////

	void SetAngle(float angle); 

	const float& GetAngle() const; 

	// Set nearest culling plane distance from view frustum's projection plane
	void SetNearPlane(float nearest); 

	// Set farhest culling plane distance from view frustum's projection plane
	void setFarPlane(float farhest); 

	//Set current wvp when needed. 
	void setCurrentWVP(Object* obj); 

	//Returns transposed camera's Projection Matrix
	const XMFLOAT4X4A GetProjMatrix(); 

	//Returns transposed Orthogonal Camera Matrix.
	const XMFLOAT4X4A GetOrthoMatrix(); 

private:
	XMFLOAT4A m_position; 
	XMFLOAT4A m_target; 
	XMFLOAT4A m_up; 

		//Projection parameters
			
		float m_angle; 
		float m_clientWidth;    //Window width 
		float m_clientHeight;	//Window height
		float m_nearPlane; 
		float m_farPlane; 
		
		XMFLOAT4X4A m_viewMatrix; 
		XMFLOAT4X4A m_projMatrix; 
		XMFLOAT4X4A m_orthoMatrix; 

public:
	const VS_CONSTANT_BUFFER& getCBufferData() const; 
	ID3D11Buffer* getConstantBuffer(); 

	void updateMatrices();
}; 