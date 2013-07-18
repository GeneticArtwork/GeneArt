////////////////////////////////////////////////////////////////////////////////
// Filename: camera.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_


//////////////
// INCLUDES //
//////////////
#include "Global.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera
{
private:
	struct VectorType 
	{ 
		vector3d position;
	};

public:
	Camera();
	~Camera();

	void SetPosition(float x, float y , float z);
	void SetRotation(float x, float y, float z);

	void Render();
	void GetViewMatrix(float* matrixOut);

private:
	void MatrixRotationYawPitchRoll(float*, float, float, float);
	void TransformCoord(VectorType&, float*);
	void BuildViewMatrix(VectorType, VectorType, VectorType);

private:
	vector3d m_position;											// Position of camera.
	vector3d m_rotation;											// Rotation of camera.
	float m_viewMatrix[16];											// The view matrix (position & rotation) of camera.
};

#endif