/*#pragma once

#include"vmath.h"
#include "MyMath.h"

Vec2f lastMouse, delta;

float toRadians(float degree)
{
	return(degree * (PI / 180.0f));
}



typedef struct Camera
{
	Vec3f position;
	Vec3f front;
	Vec3f up;
	Vec3f right;

	float pitch;
	float yaw;
	float roll;

	float sensitivity = 0.01f;

	void MoveForward()//TODO: Add a parametric value for delta time 
	{
		position.x += front.x;
		position.y += front.y;
		position.z += front.z;
	}

	void MoveBackward()//TODO: Add a parametric value for delta time 
	{
		position.x -= front.x;
		position.y -= front.y;
		position.z -= front.z;
	}

	void MoveLeft()//TODO: Add a parametric value for delta time 
	{
		right = front.Cross(up);
		position.x -= right.x;
		position.y -= right.y;
		position.z -= right.z;
	}

	void MoveRight()//TODO: Add a parametric value for delta time 
	{
		right = front.Cross(up);
		position.x += right.x;
		position.y += right.y;
		position.z += right.z;
	}

	void MoveUpward()//TODO: Elaborate inline functions
	{
		position.y += 0.1f;
	}

	void MoveDownward()
	{
		position.y -= 0.1f;
	}

	void OnMouseMove(LPARAM lParam)
	{
		delta.x = GET_X_LPARAM(lParam) - lastMouse.x;
		delta.y = GET_Y_LPARAM(lParam) - lastMouse.y;

		yaw   += delta.x  * 0.01f;
		pitch -= delta.y  * 0.01f;

		//Caping the values of pitch to -90 and 90
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		front.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
		front.y = sin(toRadians(pitch));
		front.z = sin(toRadians(yaw)) * cos(toRadians(pitch));

		front.NormalizeVec3f();

		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;
	}

	void getViewMatrix(void)
	{
		//vmath::lookat(position, position + front, up);
	}

}Camera;*/




///Try
#pragma once

#include"vmath.h"
#include "MyMath.h"

vmath::vec2 lastMouse; 
vmath::vec2 delta; 

float toRadians(float degree)
{
	return(degree * (PI / 180.0f));
}



class Camera
{
public:
		vmath::vec3 position;
		vmath::vec3 front;
		vmath::vec3 up;
		vmath::vec3 right;

		float pitch = 0.0f;
		float yaw = 0.0f;
		float roll = 0.0f;

		float sensitivity = 0.005f;

		void MoveForward()//TODO: Add a parametric value for delta time 
		{
			position += sensitivity * front;
		}

		void MoveBackward()//TODO: Add a parametric value for delta time 
		{
			position -= sensitivity * front;
		}

		void MoveLeft()//TODO: Add a parametric value for delta time 
		{
			right = Cross();

			position -= sensitivity * right;
		}

		void MoveRight()//TODO: Add a parametric value for delta time 
		{
			right = Cross();

			position += sensitivity * right;
		}

		void MoveUpward()//TODO: Elaborate inline functions
		{
			position[1] += sensitivity;
		}

		void MoveDownward()
		{
			position[1] -= sensitivity;
		}

		void OnMouseMove(LPARAM lParam)
		{
			delta[0] = GET_X_LPARAM(lParam) - lastMouse[0];
			delta[1] = GET_Y_LPARAM(lParam) - lastMouse[1];

			yaw += delta[0] * sensitivity;
			pitch -= delta[1] * sensitivity;

			//Caping the values of pitch to -90 and 90
			if (pitch > 89.0f)
			{
				pitch = 89.0f;
			}
			if (pitch < -89.0f)
			{
				pitch = -89.0f;
			}

			front[0] = cos(toRadians(yaw)) * cos(toRadians(pitch));
			front[1] = sin(toRadians(pitch));
			front[2] = sin(toRadians(yaw)) * cos(toRadians(pitch));

			NormalizeVec3f();

			up[0] = 0.0f;
			up[1] = 1.0f;
			up[2] = 0.0f;
		}

		void getViewMatrix(void)
		{
			vmath::lookat(position, position + front, up);
		}

		vmath::vec3 Cross()
		{
			vmath::vec3 result;

			result[0] = (front[1] * up[2]) - (front[2] * up[1]);

			result[1] = (front[2] * up[0]) - (front[0] * up[2]);

			result[2] = (front[0] * up[1]) - (front[1] * up[0]);

			return(result);
		}


		void NormalizeVec3f()
		{
			float magnitude = magnitudeVec3f();
			if (magnitude != 0)
			{
				front[0] = front[0] / magnitude;
				front[1] = front[1] / magnitude;
				front[2] = front[2] / magnitude;
			}
		}

		float magnitudeVec3f()
		{
			return(sqrt(front[0] * front[0] + front[1] * front[1] + front[2] * front[2]));
		}


		double toRadians(float ang) const
		{
			return(ang * (PI / 180.0));
		}

};



