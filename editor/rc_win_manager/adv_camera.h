#ifndef _CAMERA_H
#define _CAMERA_H

#include "utMath.h"
#include <string>
using namespace std;
using namespace Horde3D;
class Camera
{
public:
	enum CameraBehavior
	{
		CAMERA_BEHAVIOR_FIRST_PERSON,
		CAMERA_BEHAVIOR_SPECTATOR,
		CAMERA_BEHAVIOR_FLIGHT,
		CAMERA_BEHAVIOR_ORBIT
	};
	enum ProjectionModePlane
	{
		D3,
		X,
		Y,
		Z
	};
private:
	int _cam;

	static const Vec3f WORLD_XAXIS;
	static const Vec3f WORLD_YAXIS;
	static const Vec3f WORLD_ZAXIS;
	static const float DEFAULT_ORBIT_MIN_ZOOM;
	static const float DEFAULT_ORBIT_MAX_ZOOM;
	static const float DEFAULT_ORBIT_OFFSET_DISTANCE;
	static const float DEFAULT_ROTATION_SPEED;

	CameraBehavior	m_behavior;
	bool			m_preferTargetYAxisOrbiting;
	float			m_accumPitchDegrees;
	float			m_savedAccumPitchDegrees;
	float			m_rotationSpeed;
	float			m_orbitMinZoom;
	float			m_orbitMaxZoom;
	float			m_orbitOffsetDistance;
	float			m_firstPersonYOffset;


	Vec3f			m_eye;
	Vec3f			m_savedEye;
	Vec3f			m_target;
	Vec3f			m_targetYAxis;

	Vec3f			m_xAxis;
	Vec3f			m_yAxis;
	Vec3f			m_zAxis;
	Vec3f			m_viewDir;
	Vec3f			m_acceleration;
	Vec3f			m_currentVelocity;
	Vec3f			m_velocity;
	Quaternion		m_orientation;
	Quaternion		m_savedOrientation;

	Matrix4f		m_viewMatrix;
	Matrix4f		m_projMatrix;
	Matrix4f		m_viewProjMatrix;
	Vec3f			direction;


	//ѕеременные дл€ интерпол€ции камеры
	bool			m_interpolation;
	Vec3f			m_startInterp;
	Vec3f			m_endInterp;
	float			m_currentTime;
	float			m_timeForInterpolite;

public:
	Vec3f			CAMERA_DIRRECTION;
	~Camera(void);
	Camera(void);

	void setNodeCamera(int cam);
	int  GetCamera(void);
	
	void lookAt(const Vec3f &target);
	void lookAt(const Vec3f &eye, const Vec3f &target, const Vec3f &up);
	void move(float dx, float dy, float dz);
	void move(const Vec3f &direction, const Vec3f &amount);
	void moveByMouseOrtho(const Vec3f &pos);
	void setCameraProjection(ProjectionModePlane);
	void rotate(float headingDegrees, float pitchDegrees, float rollDegrees);
	void ResetRotate(void);

	void rotateSmoothly(float headingDegrees, float pitchDegrees, float rollDegrees);
	void undoRoll();
	void zoom(float zoom, float minZoom, float maxZoom);

	void updatePosition(const Vec3f &direction, float elapsedTimeSec);
	Vec3f updatePositionInterpolite(const Vec3f &start,const Vec3f &end,float time,float currentTime);
	Matrix4f convertToCurrent(void);


	//void UpdateCamera(void);
	

	void setBehavior(CameraBehavior newBehavior);
	void setPosition(float x, float y, float z);
	void setPosition(const Vec3f &position);
	//void setPositionOrto(const Vec3f &position);
	void setAcceleration(float x, float y, float z);
	void setAcceleration(const Vec3f &acceleration);
	void setVelocity(float x, float y, float z);
	void setVelocity(const Vec3f &velocity);
	void setCurrentVelocity(float x, float y, float z);
	void setCurrentVelocity(const Vec3f &currentVelocity);
	void setOrientation(const Quaternion &orientation);

	void setOrbitMaxZoom(float orbitMaxZoom);
	void setOrbitMinZoom(float orbitMinZoom);
	void setOrbitOffsetDistance(float orbitOffsetDistance);
	void setOrbitPitchMaxDegrees(float orbitPitchMaxDegrees);
	void setOrbitPitchMinDegrees(float orbitPitchMinDegrees);
	void setPreferTargetYAxisOrbiting(bool preferTargetYAxisOrbiting);
	void setRotationSpeed(float rotationSpeed);



	const Vec3f		&getAcceleration() const;
	CameraBehavior	getBehavior() const;
	const Vec3f		&getCurrentVelocity() const;
	const Vec3f		&getPosition() const;
	Matrix4f		&getProjectionMatrix();

	float			getOrbitMinZoom() const;
	float			getOrbitMaxZoom() const;
	float			getOrbitOffsetDistance() const;
	float			getOrbitPitchMaxDegrees() const;
	float			getOrbitPitchMinDegrees() const;
	const Quaternion &getOrientation() const;
	float			getRotationSpeed() const;
	bool			preferTargetYAxisOrbiting() const;
	Matrix4f		&getViewProjectionMatrix();
	
	const Vec3f		&getVelocity() const;
	const Vec3f		&getViewDirection() const;
	Matrix4f		&getViewMatrix() ;
	const Vec3f		&getXAxis() const;
	const Vec3f		&getYAxis() const;
	const Vec3f		&getZAxis() const;
	void updateViewMatrix(void);
private:
	void rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees);
	void rotateFirstPerson(float headingDegrees, float pitchDegrees);
	
	void rotateOrbit(float headingDegrees, float pitchDegrees, float rollDegrees);
	void updateVelocity(const Vec3f &direction, float elapsedTimeSec);
	
	

};


inline const Vec3f &Camera::getAcceleration() const
{ return m_acceleration; }

inline Camera::CameraBehavior Camera::getBehavior() const
{ return m_behavior; }

inline const Vec3f &Camera::getCurrentVelocity() const
{ return m_currentVelocity; }

inline const Vec3f &Camera::getPosition() const
{ return m_eye; }
inline float Camera::getOrbitMinZoom() const
{ return m_orbitMinZoom; }

inline float Camera::getOrbitMaxZoom() const
{ return m_orbitMaxZoom; }

inline float Camera::getOrbitOffsetDistance() const
{ return m_orbitOffsetDistance; }

inline const Quaternion &Camera::getOrientation() const
{ return m_orientation; }

inline float Camera::getRotationSpeed() const
{ return m_rotationSpeed; }

inline Matrix4f &Camera::getProjectionMatrix() 
{ return m_projMatrix; }

inline const Vec3f &Camera::getVelocity() const
{ return m_velocity; }

inline const Vec3f &Camera::getViewDirection() const
{ return m_viewDir; }

inline Matrix4f &Camera::getViewMatrix()
{ return m_viewMatrix; }

inline Matrix4f &Camera::getViewProjectionMatrix()
{ return m_viewProjMatrix; }

inline const Vec3f &Camera::getXAxis() const
{ return m_xAxis; }

inline const Vec3f &Camera::getYAxis() const
{ return m_yAxis; }

inline const Vec3f &Camera::getZAxis() const
{ return m_zAxis; }

inline bool Camera::preferTargetYAxisOrbiting() const
{ return m_preferTargetYAxisOrbiting; }


#endif