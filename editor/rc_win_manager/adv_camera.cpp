#include "adv_camera.h"
#include <iostream>
#include <algorithm>
//#include "math.h"
//#define _USE_MATH_DEFINES
//#define M_PI       3.14159265358979323846
using namespace std;
#define stop __asm nop

const float Camera::DEFAULT_ROTATION_SPEED = 0.3f;
// const float Camera::DEFAULT_FOVX = 90.0f;
// const float Camera::DEFAULT_ZNEAR = 0.1f;
// const float Camera::DEFAULT_ZFAR = 1000.0f;

const float Camera::DEFAULT_ORBIT_MIN_ZOOM = 1.0f;
const float Camera::DEFAULT_ORBIT_MAX_ZOOM = 100.0f;

const float Camera::DEFAULT_ORBIT_OFFSET_DISTANCE = DEFAULT_ORBIT_MIN_ZOOM +
(DEFAULT_ORBIT_MAX_ZOOM - DEFAULT_ORBIT_MIN_ZOOM) * 0.25f;

const Vec3f Camera::WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const Vec3f Camera::WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const Vec3f Camera::WORLD_ZAXIS(0.0f, 0.0f, 1.0f);


Camera::Camera(void)
{
	_cam = 0x0;
	m_interpolation = false;
	m_currentTime = 0.0;
	m_startInterp= Vec3f(0,0,0);
	m_endInterp =Vec3f(0,0,0);
	m_timeForInterpolite=0.0;
}

Camera::~Camera(void)
{
}
void Camera::lookAt( const Vec3f &target )
{
	m_xAxis= Vec3f(1.0f, 0.0f, 0.0f);
	m_yAxis= Vec3f(0.0f, 1.0f, 0.0f);
	m_zAxis= Vec3f(0.0f, 0.0f, 1.0f);

	m_targetYAxis= Vec3f(0.0f, 1.0f, 0.0f);
	lookAt(m_eye, target, m_yAxis);
	updateViewMatrix();
}

void Camera::lookAt( const Vec3f &eye, const Vec3f &target, const Vec3f &up )
{
	m_eye = eye;

	m_zAxis = eye - target;
	m_zAxis.normalise();

	m_viewDir = -m_zAxis;

	m_xAxis = up.cross(m_zAxis);
	m_xAxis.normalise();

	m_yAxis = m_zAxis.cross(m_xAxis);
	m_yAxis.normalise();
	m_xAxis.normalise();

	m_viewMatrix.c[0][0] = m_xAxis.x;
	m_viewMatrix.c[1][0] = m_xAxis.y;
	m_viewMatrix.c[2][0] = m_xAxis.z;
	m_viewMatrix.c[3][0] = -(m_xAxis.dot(eye) );

	m_viewMatrix.c[0][1] = m_yAxis.x;
	m_viewMatrix.c[1][1] = m_yAxis.y;
	m_viewMatrix.c[2][1] = m_yAxis.z;
	m_viewMatrix.c[3][1] = -(m_yAxis.dot(eye));

	m_viewMatrix.c[0][2] = m_zAxis.x;
	m_viewMatrix.c[1][2] = m_zAxis.y;
	m_viewMatrix.c[2][2] = m_zAxis.z;    
	m_viewMatrix.c[3][2] = -(m_zAxis.dot(eye));

	// Extract the pitch angle from the view matrix.
	m_accumPitchDegrees = radToDeg(-asinf(m_viewMatrix.c[1][2]));

	m_orientation.fromRotationMatrixView(m_viewMatrix.c);
	m_target=target;

}

void Camera::move( float dx, float dy, float dz )
{
	
	
	if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
	{
		// Orbiting camera is always positioned relative to the
		// target position. See updateViewMatrix().
		return;
	}
	Vec3f eye = m_eye;
	Vec3f forwards;

	if (m_behavior == CAMERA_BEHAVIOR_FIRST_PERSON)
	{
		// Calculate the forwards direction. Can't just use the camera's local
		// z axis as doing so will cause the camera to move more slowly as the
		// camera's view approaches 90 degrees straight up and down.

		forwards = WORLD_YAXIS.cross(m_xAxis);
		forwards.normalise();
	}
// 	if (m_behavior == CAMERA_BEHAVIOR_SPECTATOR)
// 	{
// 		// Calculate the forwards direction. Can't just use the camera's local
// 		// z axis as doing so will cause the camera to move more slowly as the
// 		// camera's view approaches 90 degrees straight up and down.
// 
// 		forwards = WORLD_YAXIS.crossProduct(m_xAxis);
// 		forwards.normalise();
// 	}
	else
	{
		forwards = m_viewDir;
	}

	eye += m_xAxis * dx;
	eye += WORLD_YAXIS * dy;
	eye += forwards * dz;

	setPosition(eye);
}

void Camera::move( const Vec3f &direction, const Vec3f &amount )
{
	if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
	{
		// Orbiting camera is always positioned relative to the
		// target position. See updateViewMatrix().
		return;
	}
	m_eye.x += direction.x * amount.x;
	m_eye.y += direction.y * amount.y;
	m_eye.z += direction.z * amount.z;

	updateViewMatrix();
}

void Camera::moveByMouseOrtho( const Vec3f &pos )
{
	//m_target+=pos;

	m_orientation.ToRotationMatrixView(m_viewMatrix.c);

	m_xAxis=Vec3f(m_viewMatrix.c[0][0], m_viewMatrix.c[1][0], m_viewMatrix.c[2][0]);
	////m_xAxis*=pos;
	//Vec3f tmpX=m_xAxis;
	//tmpX.x+=pos.x*10;
	//m_target+=tmpX;
	m_yAxis=Vec3f(m_viewMatrix.c[0][1], m_viewMatrix.c[1][1], m_viewMatrix.c[2][1]);
	////m_yAxis*=pos;
	m_zAxis=Vec3f(m_viewMatrix.c[0][2], m_viewMatrix.c[1][2], m_viewMatrix.c[2][2]);
	////m_zAxis*=pos;
	m_viewDir = -m_zAxis;

	//m_eye+=pos;

	m_eye += m_xAxis * pos.x;
	m_eye += m_yAxis * pos.y;
	m_eye += m_zAxis * pos.z;


	Vec3f forwards = WORLD_YAXIS.cross(m_xAxis);

	if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
	{
		//m_eye = m_target + m_zAxis * m_orbitOffsetDistance;
		m_target=m_eye-m_zAxis * m_orbitOffsetDistance;
	}
	m_viewMatrix.c[3][0] = -(m_xAxis.dot(m_eye));
	m_viewMatrix.c[3][1] = -(m_yAxis.dot(m_eye));
	m_viewMatrix.c[3][2] = -(m_zAxis.dot(m_eye));
	//m_viewMatrix.translate(pos.x,pos.y,pos.z);
}

void Camera::rotate( float headingDegrees, float pitchDegrees, float rollDegrees )
{
	switch (m_behavior)
	{
	case CAMERA_BEHAVIOR_FIRST_PERSON:
	case CAMERA_BEHAVIOR_SPECTATOR:
		rotateFirstPerson(headingDegrees, pitchDegrees);
		break;

	case CAMERA_BEHAVIOR_FLIGHT:
		rotateFlight(headingDegrees, pitchDegrees, rollDegrees);
		break;
	case CAMERA_BEHAVIOR_ORBIT:
		rotateOrbit(headingDegrees, pitchDegrees, rollDegrees);
		break;
	}

	updateViewMatrix();
}

void Camera::rotateSmoothly( float headingDegrees, float pitchDegrees, float rollDegrees )
{
	headingDegrees *= m_rotationSpeed;
	pitchDegrees *= m_rotationSpeed;
	rollDegrees *= m_rotationSpeed;

	rotate(headingDegrees, pitchDegrees, rollDegrees);

}
void Camera::undoRoll()
{
	if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
		lookAt(m_eye, m_target, m_targetYAxis);
	else
		lookAt(m_eye, m_eye + m_viewDir, WORLD_YAXIS);
}
void Camera::updatePosition( const Vec3f &direction, float elapsedTimeSec )
{
	if(!m_interpolation)
	{
		if (m_currentVelocity.magnitudeSq() != 0.0f)
		{
			// Only move the camera if the velocity vector is not of zero length.
			// Doing this guards against the camera slowly creeping around due to
			// floating point rounding errors.

			Vec3f displacement = (m_currentVelocity * elapsedTimeSec) +
				( m_acceleration * elapsedTimeSec * elapsedTimeSec * 0.5f);

			// Floating point rounding errors will slowly accumulate and cause the
			// camera to move along each axis. To prevent any unintended movement
			// the displacement vector is clamped to zero for each direction that
			// the camera isn't moving in. Note that the updateVelocity() method
			// will slowly decelerate the camera's velocity back to a stationary
			// state when the camera is no longer moving along that direction. To
			// account for this the camera's current velocity is also checked.

			if (direction.x == 0.0f && closeEnough(m_currentVelocity.x, 0.0f))
				displacement.x = 0.0f;

			if (direction.y == 0.0f && closeEnough(m_currentVelocity.y, 0.0f))
				displacement.y = 0.0f;

			if (direction.z == 0.0f && closeEnough(m_currentVelocity.z, 0.0f))
				displacement.z = 0.0f;

			move(displacement.x, displacement.y, displacement.z);
		}

		// Continuously update the camera's velocity vector even if the camera
		// hasn't moved during this call. When the camera is no longer being moved
		// the camera is decelerating back to its stationary state.

		updateVelocity(direction, elapsedTimeSec);
	}else
	{
		Vec3f IntPos = updatePositionInterpolite(m_startInterp,m_endInterp,m_timeForInterpolite,elapsedTimeSec);
		//move(IntPos.x, IntPos.y, IntPos.z);
// 		eye += m_xAxis * dx;
// 		eye += WORLD_YAXIS * dy;
// 		eye += forwards * dz;
		setPosition(IntPos);
	}
}

void Camera::zoom( float zoom, float minZoom, float maxZoom )
{
	if (m_behavior == CAMERA_BEHAVIOR_ORBIT )
	{
		// Moves the camera closer to or further away from the orbit
		// target. The zoom amounts are in world units.
		//m_orbitOffsetDistance + elapsedTimeSec*(zoom-m_orbitOffsetDistance);



		m_orbitMaxZoom = maxZoom;
		m_orbitMinZoom = minZoom;

		Vec3f offset = m_eye - m_target;

		m_orbitOffsetDistance = offset.length();
		offset.normalise();
		m_orbitOffsetDistance += zoom;
		m_orbitOffsetDistance = min(max(m_orbitOffsetDistance, minZoom), maxZoom);

		offset *= m_orbitOffsetDistance;
		m_eye = offset + m_target;

		updateViewMatrix();
	}
	//else
	//{
	//	// For the other behaviors zoom is interpreted as changing the
	//	// horizontal field of view. The zoom amounts refer to the horizontal
	//	// field of view in degrees.

	//	zoom = std::min(std::max(zoom, minZoom), maxZoom);
	//	perspective(zoom, m_aspectRatio, m_znear, m_zfar);
	//}
}
void Camera::setAcceleration( float x, float y, float z )
{
	m_acceleration = Vec3f(x, y, z);
}

void Camera::setAcceleration( const Vec3f &acceleration )
{
	m_acceleration = acceleration;
}

void Camera::setBehavior( CameraBehavior newBehavior )
{
	CameraBehavior prevBehavior = m_behavior;

	if (prevBehavior == newBehavior)
		return;

	m_behavior = newBehavior;

	switch (newBehavior)
	{
	case CAMERA_BEHAVIOR_FIRST_PERSON:
		switch (prevBehavior)
		{
		default:
			break;

		case CAMERA_BEHAVIOR_FLIGHT:
			m_eye.y = m_firstPersonYOffset;
			updateViewMatrix();
			break;

		case CAMERA_BEHAVIOR_SPECTATOR:
			m_eye.y = m_firstPersonYOffset;
			updateViewMatrix();
			break;

		case CAMERA_BEHAVIOR_ORBIT:
			m_eye.x = m_savedEye.x;
			m_eye.z = m_savedEye.z;
			m_eye.y = m_firstPersonYOffset;
			m_orientation = m_savedOrientation;
			m_accumPitchDegrees = m_savedAccumPitchDegrees;
			updateViewMatrix();
			break;
		}

		undoRoll();
		break;

	case CAMERA_BEHAVIOR_SPECTATOR:
		switch (prevBehavior)
		{
		default:
			break;

		case CAMERA_BEHAVIOR_FLIGHT:
			updateViewMatrix();
			break;

		case CAMERA_BEHAVIOR_ORBIT:
			m_eye = m_savedEye;
			m_orientation = m_savedOrientation;
			m_accumPitchDegrees = m_savedAccumPitchDegrees;
			updateViewMatrix();
			break;
		}

		undoRoll();
		break;

	case CAMERA_BEHAVIOR_FLIGHT:
		if (prevBehavior == CAMERA_BEHAVIOR_ORBIT)
		{
			m_eye = m_savedEye;
			m_orientation = m_savedOrientation;
			m_accumPitchDegrees = m_savedAccumPitchDegrees;
			updateViewMatrix();
		}
		else
		{
			m_savedEye = m_eye;
			updateViewMatrix();
		}
		break;

	case CAMERA_BEHAVIOR_ORBIT:
		if (prevBehavior == CAMERA_BEHAVIOR_FIRST_PERSON)
			m_firstPersonYOffset = m_eye.y;

		m_savedEye = m_eye;
		m_savedOrientation = m_orientation;
		m_savedAccumPitchDegrees = m_accumPitchDegrees;

		m_targetYAxis = m_yAxis;

		Vec3f newEye = m_eye + m_zAxis * m_orbitOffsetDistance;
		Vec3f newTarget = m_eye;

		lookAt(newEye, newTarget, m_targetYAxis);
		break;
	}
}

void Camera::setCurrentVelocity( float x, float y, float z )
{
	m_currentVelocity=Vec3f(x, y, z);
}

void Camera::setCurrentVelocity( const Vec3f &currentVelocity )
{
	m_currentVelocity = currentVelocity;
}

void Camera::setOrbitMaxZoom( float orbitMaxZoom )
{
	m_orbitMaxZoom = orbitMaxZoom;
}

void Camera::setOrbitMinZoom( float orbitMinZoom )
{
	m_orbitMinZoom = orbitMinZoom;
}

void Camera::setOrbitOffsetDistance( float orbitOffsetDistance )
{
	m_orbitOffsetDistance = orbitOffsetDistance;
}

void Camera::setOrientation( const Quaternion &newOrientation )
{
	Matrix4f m;
	newOrientation.ToRotationMatrixView(m.c);
	m_accumPitchDegrees = radToDeg(asinf(m.c[1][2]));


	m_orientation = newOrientation;

	//if (m_behavior == CAMERA_BEHAVIOR_FIRST_PERSON || m_behavior == CAMERA_BEHAVIOR_SPECTATOR)
		//lookAt(m_eye, m_eye + m_viewDir, WORLD_YAXIS);

	updateViewMatrix();
}

void Camera::setPosition( float x, float y, float z )
{
	m_eye= Vec3f(x, y, z);
	updateViewMatrix();
}

void Camera::setPosition( const Vec3f &position )
{
	m_eye = position;
	updateViewMatrix();
}

void Camera::setPreferTargetYAxisOrbiting( bool preferTargetYAxisOrbiting )
{
	m_preferTargetYAxisOrbiting = preferTargetYAxisOrbiting;

	if (m_preferTargetYAxisOrbiting)
		undoRoll();
}

void Camera::setRotationSpeed( float rotationSpeed )
{
	m_rotationSpeed = rotationSpeed;
}
void Camera::setVelocity( float x, float y, float z )
{
	m_velocity= Vec3f(x, y, z);

}

void Camera::setVelocity( const Vec3f &velocity )
{
	m_velocity = velocity;
}

void Camera::rotateFirstPerson( float headingDegrees, float pitchDegrees )
{
	// Implements the rotation logic for the first person style and
	// spectator style camera behaviors. Roll is ignored.


	/*m_accumPitchDegrees += pitchDegrees;

	if (m_accumPitchDegrees > 90.0f)
	{
		pitchDegrees = 90.0f - (m_accumPitchDegrees - pitchDegrees);
		m_accumPitchDegrees = 90.0f;
	}

	if (m_accumPitchDegrees < -90.0f)
	{
		pitchDegrees = -90.0f - (m_accumPitchDegrees - pitchDegrees);
		m_accumPitchDegrees = -90.0f;
	}*/

	Quaternion rot;

	// Rotate camera about the world y axis.
	// Note the order the quaternions are multiplied. That is important!
	if (headingDegrees != 0.0f)
	{
		rot.fromAxisAngle(WORLD_YAXIS, headingDegrees);
		m_orientation = rot.mult(m_orientation) ;
	}

	// Rotate camera about its local x axis.
	// Note the order the quaternions are multiplied. That is important!
	if (pitchDegrees != 0.0f)
	{
		rot.fromAxisAngle(WORLD_XAXIS,pitchDegrees);
		m_orientation = m_orientation.mult(rot) ;
	}
}

void Camera::ResetRotate( void )
{
//	m_accumPitchDegrees = pitchDegrees;

	Quaternion rot;

	// Rotate camera about the world y axis.
	// Note the order the quaternions are multiplied. That is important!
		m_orientation = rot ;
		updateViewMatrix();


	// Rotate camera about its local x axis.
	// Note the order the quaternions are multiplied. That is important!

}
void Camera::rotateFlight( float headingDegrees, float pitchDegrees, float rollDegrees )
{

	m_accumPitchDegrees += pitchDegrees;

	if (m_accumPitchDegrees > 360.0f)
		m_accumPitchDegrees -= 360.0f;

	if (m_accumPitchDegrees < -360.0f)
		m_accumPitchDegrees += 360.0f;

	Quaternion rot;
	Matrix4f m;
	m.fromHeadPitchRoll(headingDegrees, pitchDegrees, rollDegrees);

	rot.fromRotationMatrixView(m.c);
	m_orientation =m_orientation.mult(rot);
}

void Camera::rotateOrbit( float headingDegrees, float pitchDegrees, float rollDegrees )
{
	Quaternion rot;

	float Rottmp[16];
	m_orientation.toMatrix(Rottmp);
	Matrix4f RotMatrix(Rottmp);
	Vec3f Rot;
	Vec3f TMP;

	RotMatrix.decompose(TMP,Rot,TMP);
	//cout<<Rot.z<<endl;
 	/*if (abs(Rot.z) >1.4 && pitchDegrees>0)
 	{
 		pitchDegrees=0;
 	}*/
	if (m_preferTargetYAxisOrbiting)
	{
		if (headingDegrees != 0.0f)
		{
				rot.fromAxisAngle(m_targetYAxis, headingDegrees);
				m_orientation = rot.mult(m_orientation);
		}

		if (pitchDegrees != 0.0f)
		{
			rot.fromAxisAngle(WORLD_XAXIS, pitchDegrees);
			m_orientation = m_orientation.mult(rot);
		}
	}
	else
	{
		Matrix4f m;
		m.fromHeadPitchRoll(headingDegrees, pitchDegrees, rollDegrees);

		rot.fromRotationMatrixView(m.c);
		m_orientation =m_orientation.mult(rot) ;
	}
}
void Camera::updateVelocity( const Vec3f &direction, float elapsedTimeSec )
{
	// Updates the camera's velocity based on the supplied movement direction
	// and the elapsed time (since this method was last called). The movement
	// direction is in the range [-1,1].

	if (direction.x != 0.0f)
	{
		// Camera is moving along the x axis.
		// Linearly accelerate up to the camera's max speed.

		m_currentVelocity.x += direction.x * m_acceleration.x * elapsedTimeSec;

		if (m_currentVelocity.x > m_velocity.x)
			m_currentVelocity.x = m_velocity.x;
		else if (m_currentVelocity.x < -m_velocity.x)
			m_currentVelocity.x = -m_velocity.x;
	}
	else
	{
		// Camera is no longer moving along the x axis.
		// Linearly decelerate back to stationary state.

		if (m_currentVelocity.x > 0.0f)
		{
			if ((m_currentVelocity.x -= m_acceleration.x * elapsedTimeSec) < 0.0f)
				m_currentVelocity.x = 0.0f;
		}
		else
		{
			if ((m_currentVelocity.x += m_acceleration.x * elapsedTimeSec) > 0.0f)
				m_currentVelocity.x = 0.0f;
		}
	}

	if (direction.y != 0.0f)
	{
		// Camera is moving along the y axis.
		// Linearly accelerate up to the camera's max speed.

		m_currentVelocity.y += direction.y * m_acceleration.y * elapsedTimeSec;

		if (m_currentVelocity.y > m_velocity.y)
			m_currentVelocity.y = m_velocity.y;
		else if (m_currentVelocity.y < -m_velocity.y)
			m_currentVelocity.y = -m_velocity.y;
	}
	else
	{
		// Camera is no longer moving along the y axis.
		// Linearly decelerate back to stationary state.

		if (m_currentVelocity.y > 0.0f)
		{
			if ((m_currentVelocity.y -= m_acceleration.y * elapsedTimeSec) < 0.0f)
				m_currentVelocity.y = 0.0f;
		}
		else
		{
			if ((m_currentVelocity.y += m_acceleration.y * elapsedTimeSec) > 0.0f)
				m_currentVelocity.y = 0.0f;
		}
	}

	if (direction.z != 0.0f)
	{
		// Camera is moving along the z axis.
		// Linearly accelerate up to the camera's max speed.

		m_currentVelocity.z += direction.z * m_acceleration.z * elapsedTimeSec;

		if (m_currentVelocity.z > m_velocity.z)
			m_currentVelocity.z = m_velocity.z;
		else if (m_currentVelocity.z < -m_velocity.z)
			m_currentVelocity.z = -m_velocity.z;
	}
	else
	{
		// Camera is no longer moving along the z axis.
		// Linearly decelerate back to stationary state.

		if (m_currentVelocity.z > 0.0f)
		{
			if ((m_currentVelocity.z -= m_acceleration.z * elapsedTimeSec) < 0.0f)
				m_currentVelocity.z = 0.0f;
		}
		else
		{
			if ((m_currentVelocity.z += m_acceleration.z * elapsedTimeSec) > 0.0f)
				m_currentVelocity.z = 0.0f;
		}
	}
}

void Camera::updateViewMatrix(void )
{
	m_orientation.ToRotationMatrixView(m_viewMatrix.c);

	m_xAxis= Vec3f(m_viewMatrix.c[0][0], m_viewMatrix.c[1][0], m_viewMatrix.c[2][0]);
	m_yAxis= Vec3f(m_viewMatrix.c[0][1], m_viewMatrix.c[1][1], m_viewMatrix.c[2][1]);
	m_zAxis= Vec3f(m_viewMatrix.c[0][2], m_viewMatrix.c[1][2], m_viewMatrix.c[2][2]);
	m_viewDir = -m_zAxis;
	if (m_behavior == CAMERA_BEHAVIOR_ORBIT)
	{
		// Calculate the new camera position based on the current
		// orientation. The camera must always maintain the same
		// distance from the target. Use the current offset vector
		// to determine the correct distance from the target.

		m_eye = m_target + m_zAxis * m_orbitOffsetDistance;
	}
	m_viewMatrix.c[3][0] = -(m_xAxis.dot(m_eye));
	m_viewMatrix.c[3][1] = -(m_yAxis.dot(m_eye));
	m_viewMatrix.c[3][2] = -(m_zAxis.dot(m_eye));
}

void Camera::setNodeCamera( int cam )
{
	_cam = cam;
	m_behavior = CAMERA_BEHAVIOR_FIRST_PERSON;
	m_preferTargetYAxisOrbiting = true;

	m_accumPitchDegrees = 0.0f;
	m_savedAccumPitchDegrees = 0.0f;

	m_rotationSpeed = DEFAULT_ROTATION_SPEED;
	m_orbitMinZoom = DEFAULT_ORBIT_MIN_ZOOM;
	m_orbitMaxZoom = DEFAULT_ORBIT_MAX_ZOOM;
	m_orbitOffsetDistance = DEFAULT_ORBIT_OFFSET_DISTANCE;


	m_eye= Vec3f(0.0f, 0.0f, 0.0f);
	m_savedEye= Vec3f(0.0f, 0.0f, 0.0f);
	m_target= Vec3f(0.0f, 0.0f, 0.0f);

	m_xAxis= Vec3f(1.0f, 0.0f, 0.0f);
	m_yAxis= Vec3f(0.0f, 1.0f, 0.0f);
	m_zAxis= Vec3f(0.0f, 0.0f, 1.0f);

	m_targetYAxis= Vec3f(0.0f, 1.0f, 0.0f);
	m_viewDir= Vec3f(0.0f, 0.0f, -1.0f);

	m_acceleration= Vec3f(0.0f, 0.0f, 0.0f);
	m_currentVelocity= Vec3f(0.0f, 0.0f, 0.0f);
	m_velocity= Vec3f(0.0f, 0.0f, 0.0f);

//	m_viewMatrix.identity();
//	m_projMatrix.identity();
}
void Camera::setOrbitPitchMaxDegrees( float orbitPitchMaxDegrees )
{

}

void Camera::setOrbitPitchMinDegrees( float orbitPitchMinDegrees )
{

}

Matrix4f Camera::convertToCurrent( void )
{
	Matrix4f GlobMatr;
	GlobMatr=m_viewMatrix.inverted();
	//GlobMatr.
	//updateViewMatrix(true);
	//Creame2::setNodeTransformMatrix(_cam,&GlobMatr.x[0]);
	return GlobMatr;
}

Vec3f Camera::updatePositionInterpolite( const Vec3f &start,const Vec3f &end,float time,float currentTime )
{
	if (!m_interpolation)
		m_interpolation = true;

	m_startInterp = start;
	m_endInterp	=	  end;

	m_currentTime +=currentTime;
	m_timeForInterpolite = time;

	float f = m_currentTime / time;
	// обрежем f по диапазону 0-1
	if (f < 0) f = 0;
	if (f > 1)
	{
		 f = 1;
		 m_interpolation = false;
		 m_currentTime =0;
		 m_timeForInterpolite = 0;
	}
	// сама интерпол€ци€
	stop
	//f = f * f * (3 - 2 * f);

	return start * (1-f) + end * f;
}

int Camera::GetCamera( void )
{
	return _cam;
}

void Camera::setCameraProjection( ProjectionModePlane projection )
{
	
	if (projection == Camera::X)
	{
		Matrix4f RotMatrix;
		RotMatrix.RotateY(90);
		
		m_orientation.fromRotationMatrixView(RotMatrix.c);
		updateViewMatrix();
	}
	if (projection == Camera::Y)
	{
		Matrix4f RotMatrix;
		RotMatrix.RotateX(90);

		m_orientation.fromRotationMatrixView(RotMatrix.c);
		updateViewMatrix();
	}
	if (projection == Camera::Z)
	{
		Matrix4f RotMatrix;
		m_orientation.fromRotationMatrixView(RotMatrix.c);
		updateViewMatrix();
	}
}