///////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Camera.h"

Camera::Camera()
{
	m_position = vector3d(0.0f, 0.0f, 0.0f);
	m_rotation = vector3d(0.0f, 0.0f, 0.0f);
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position = vector3d(x, y, z);
}


void Camera::SetRotation(float x, float y, float z)
{
	m_rotation = vector3d(x, y, z);
}

void Camera::Render()
{
	VectorType up, position, lookAt;
	float yaw, pitch, roll;
	float rotationMatrix[9];


	// Setup the vector that points upwards.
	up.position.x = 0.0f;
	up.position.y = 1.0f;
	up.position.z = 0.0f;

	// Setup the position of the camera in the world.
	position.position = m_position;

	// Setup where the camera is looking by default.
	lookAt.position.x = 0.0f;
	lookAt.position.y = 0.0f;
	lookAt.position.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.x * RADIAN;
	yaw   = m_rotation.y * RADIAN;
	roll  = m_rotation.z * RADIAN;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	MatrixRotationYawPitchRoll(rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	TransformCoord(lookAt, rotationMatrix);
	TransformCoord(up, rotationMatrix);
	
	// Translate the rotated camera position to the location of the viewer.
	lookAt.position.x = position.position.x + lookAt.position.x;
	lookAt.position.y = position.position.y + lookAt.position.y;
	lookAt.position.z = position.position.z + lookAt.position.z;

	// Finally create the view matrix from the three updated vectors.
	BuildViewMatrix(position, lookAt, up);
}

void Camera::MatrixRotationYawPitchRoll(float* matrix, float yaw, float pitch, float roll)
{
	float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;


	// Get the cosine and sin of the yaw, pitch, and roll.
	cYaw = cosf(yaw);
	cPitch = cosf(pitch);
	cRoll = cosf(roll);

	sYaw = sinf(yaw);
	sPitch = sinf(pitch);
	sRoll = sinf(roll);

	// Calculate the yaw, pitch, roll rotation matrix.
	matrix[0] = (cRoll * cYaw) + (sRoll * sPitch * sYaw);
	matrix[1] = (sRoll * cPitch);
	matrix[2] = (cRoll * -sYaw) + (sRoll * sPitch * cYaw);
	
	matrix[3] = (-sRoll * cYaw) + (cRoll * sPitch * sYaw);
	matrix[4] = (cRoll * cPitch);
	matrix[5] = (sRoll * sYaw) + (cRoll * sPitch * cYaw);
	
	matrix[6] = (cPitch * sYaw);
	matrix[7] = -sPitch;
	matrix[8] = (cPitch * cYaw);
}

void Camera::TransformCoord(VectorType& vector, float* matrix)
{
	float x, y, z;


	// Transform the vector by the 3x3 matrix.
	x = (vector.position.x * matrix[0]) + (vector.position.y * matrix[3]) + (vector.position.z * matrix[6]);
	y = (vector.position.x * matrix[1]) + (vector.position.y * matrix[4]) + (vector.position.z * matrix[7]);
	z = (vector.position.x * matrix[2]) + (vector.position.y * matrix[5]) + (vector.position.z * matrix[8]);

	// Store the result in the reference.
	vector.position.x = x;
	vector.position.y = y;
	vector.position.z = z;
}

void Camera::BuildViewMatrix(VectorType position, VectorType lookAt, VectorType up)
{
	VectorType zAxis, xAxis, yAxis;
	float length, result1, result2, result3;


	// zAxis = normal(lookAt - position)
	zAxis.position.x = lookAt.position.x - position.position.x;
	zAxis.position.y = lookAt.position.y - position.position.y;
	zAxis.position.z = lookAt.position.z - position.position.z;
	length = sqrt((zAxis.position.x * zAxis.position.x) + (zAxis.position.y * zAxis.position.y) + (zAxis.position.z * zAxis.position.z));
	zAxis.position.x = zAxis.position.x / length;
	zAxis.position.y = zAxis.position.y / length;
	zAxis.position.z = zAxis.position.z / length;

	// xAxis = normal(cross(up, zAxis))
	xAxis.position.x = (up.position.y * zAxis.position.z) - (up.position.z * zAxis.position.y);
	xAxis.position.y = (up.position.z * zAxis.position.x) - (up.position.x * zAxis.position.z);
	xAxis.position.z = (up.position.x * zAxis.position.y) - (up.position.y * zAxis.position.x);
	length = sqrt((xAxis.position.x * xAxis.position.x) + (xAxis.position.y * xAxis.position.y) + (xAxis.position.z * xAxis.position.z));
	xAxis.position.x = xAxis.position.x / length;
	xAxis.position.y = xAxis.position.y / length;
	xAxis.position.z = xAxis.position.z / length;

	// yAxis = cross(zAxis, xAxis)
	yAxis.position.x = (zAxis.position.y * xAxis.position.z) - (zAxis.position.z * xAxis.position.y);
	yAxis.position.y = (zAxis.position.z * xAxis.position.x) - (zAxis.position.x * xAxis.position.z);
	yAxis.position.z = (zAxis.position.x * xAxis.position.y) - (zAxis.position.y * xAxis.position.x);

	// -dot(xAxis, position)
	result1 = ((xAxis.position.x * position.position.x) + (xAxis.position.y * position.position.y) + (xAxis.position.z * position.position.z)) * -1.0f;

	// -dot(yaxis, eye)
	result2 = ((yAxis.position.x * position.position.x) + (yAxis.position.y * position.position.y) + (yAxis.position.z * position.position.z)) * -1.0f;

	// -dot(zaxis, eye)
	result3 = ((zAxis.position.x * position.position.x) + (zAxis.position.y * position.position.y) + (zAxis.position.z * position.position.z)) * -1.0f;

	// Set the computed values in the view matrix.
	m_viewMatrix[0]  = xAxis.position.x;
	m_viewMatrix[1]  = yAxis.position.x;
	m_viewMatrix[2]  = zAxis.position.x;
	m_viewMatrix[3]  = 0.0f;

	m_viewMatrix[4]  = xAxis.position.y;
	m_viewMatrix[5]  = yAxis.position.y;
	m_viewMatrix[6]  = zAxis.position.y;
	m_viewMatrix[7]  = 0.0f;

	m_viewMatrix[8]  = xAxis.position.z;
	m_viewMatrix[9]  = yAxis.position.z;
	m_viewMatrix[10] = zAxis.position.z;
	m_viewMatrix[11] = 0.0f;

	m_viewMatrix[12] = result1;
	m_viewMatrix[13] = result2;
	m_viewMatrix[14] = result3;
	m_viewMatrix[15] = 1.0f;
}

void Camera::GetViewMatrix(float* matrix)
{
	matrix[0]  = m_viewMatrix[0];
	matrix[1]  = m_viewMatrix[1];
	matrix[2]  = m_viewMatrix[2];
	matrix[3]  = m_viewMatrix[3];

	matrix[4]  = m_viewMatrix[4];
	matrix[5]  = m_viewMatrix[5];
	matrix[6]  = m_viewMatrix[6];
	matrix[7]  = m_viewMatrix[7];

	matrix[8]  = m_viewMatrix[8];
	matrix[9]  = m_viewMatrix[9];
	matrix[10] = m_viewMatrix[10];
	matrix[11] = m_viewMatrix[11];

	matrix[12] = m_viewMatrix[12];
	matrix[13] = m_viewMatrix[13];
	matrix[14] = m_viewMatrix[14];
	matrix[15] = m_viewMatrix[15];
}
