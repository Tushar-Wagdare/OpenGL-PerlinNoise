#include<windows.h>
#define PI 3.141526
enum
{
	USESMOOTHERSTEP = 0,
	USELINEAR,
	USELINEARINCREAMENT,
	USEBAZIERSPLINE,
	USEBAZIERCURVE
};

typedef struct
{
	float x;
	float y;
}Vec2f;

typedef struct
{
	float x;
	float y;
	float z;
}Vec3f;

typedef struct
{
	float x;
	float y;
	float z;
	float w;
}Vec4f;

typedef struct Vec3b
{
	BOOL Trans = FALSE;
	BOOL Scale = FALSE;
	BOOL Rotate = FALSE;
}Vec3b;



float toRadians(float degree)
{
	return(degree * (PI / 180.0f));
}

Vec3f cross(Vec3f a, Vec3f b)
{
	Vec3f c;
	c.x = (a.y * b.z) - (a.z * b.y);
	c.y = (a.z * b.x) - (a.x * b.z);
	c.z = (a.x * b.y) - (a.y * b.x);

	return(c);
}

void NormalizeVec3f(Vec3f* inputVector)
{
	float magnitude = sqrt((inputVector->x * inputVector->x) + (inputVector->y * inputVector->y) + (inputVector->z * inputVector->z));

	if (magnitude != 0)
	{
		inputVector->x = inputVector->x / magnitude;
		inputVector->y = inputVector->y / magnitude;
		inputVector->z = inputVector->z / magnitude;
	}
}

typedef struct
{
	Vec3f contolPoints[6];
	Vec3f splineVertices[60];
	int resolution;
}BezierSpline;

//generateSpline(BezierSpline* bazierSpline)
//{
//	int index = 0;
//	float tStep = 1.0f / bazierSpline->resolution;
//	for (float = 0; i < 1.0f; i += tStep)
//	{
//		bazierSpline->splineVertices[index] = bazierSpline->contolPoints;
//		index++;
//	}
//}

typedef struct Camera
{
	Vec3f position;
	Vec3f front;
	Vec3f up;
	Vec3f right;
	float yaw;
	float roll;
	float pitch;

}Camera;

float lerpf(float point1, float point2, float weight, BOOL bflag)
{
	if (bflag == TRUE)
	{
		return(point2 - point1) * ((weight * (weight * 6.0 - 15.0) + 10.0) * weight * weight * weight) + point1;
	}
	else
	{
		return(point1 + (point2 - point1) * weight);
	}
}

float smootherstep(float w)
{
	float a = w * w;
	float b = 1.0f - ((1.0f - w) * (1.0f - w));

	return(lerpf(a, b, w, FALSE));
}

Vec3f lerp3v(Vec3f* p1, Vec3f* p2, float weight, BOOL ipFlag)
{
	Vec3f result;
	if (ipFlag == USESMOOTHERSTEP)
	{
		float w = smootherstep(weight);
		result.x = p1->x + (p2->x - p1->x) * w;
		result.y = p1->y + (p2->y - p1->y) * w;
		result.z = p1->z + (p2->z - p1->z) * w;
	}
	
	if (ipFlag == USELINEAR)
	{
		result.x = p1->x + (p2->x - p1->x) * weight;
		result.y = p1->y + (p2->y - p1->y) * weight;
		result.z = p1->z + (p2->z - p1->z) * weight;
	}

	if (ipFlag == USELINEARINCREAMENT)
	{
		result.x = (1 - weight) * p1->x + weight * p2->x;
		result.y = (1 - weight) * p1->y + weight * p2->y;
		result.z = (1 - weight) * p1->z + weight * p2->z;
	}	

	/*result.x = (p2->x - p1->x) * ((weight * (weight * 6.0 - 15.0) + 10.0) * weight * weight * weight) + p1->x;
	result.y = (p2->y - p1->y) * ((weight * (weight * 6.0 - 15.0) + 10.0) * weight * weight * weight) + p1->y;
	result.z = (p2->z - p1->z) * ((weight * (weight * 6.0 - 15.0) + 10.0) * weight * weight * weight) + p1->z;*/

	return(result);
}

void drawSpline(Vec3f* spline, Vec3f splineData[], GLuint size, float w, BOOL bFlag, BOOL ipFlag)
{
	Vec3f a,a1,a2,b,c,d,e;
	GLuint splineIndex = 0;

	if (bFlag == USEBAZIERCURVE)
	{
		GLuint numPoints = size / sizeof(spline[0]) - 1.0f;
		for (int i = 0; i < numPoints; i += 3)
		{
			for (float j = 0.0f; j < 1.0f; j += w)
			{
				a = lerp3v(&spline[i], &spline[i + 1], j, ipFlag);
				b = lerp3v(&spline[i + 1], &spline[i + 2], j, ipFlag);
				c = lerp3v(&spline[i + 2], &spline[i + 3], j, ipFlag);

				d = lerp3v(&a, &b, j, ipFlag);
				e = lerp3v(&b, &c, j, ipFlag);

				splineData[splineIndex] = lerp3v(&d, &e, j, ipFlag);
				splineIndex++;
			}
		}
	}
	
	if (bFlag == USEBAZIERSPLINE)
	{
		GLuint numPoints = size / sizeof(spline[0]);
		for (int i = 0; i < numPoints; i++)
		{
			a1.x = spline[i].x + (spline[i + 1].x - spline[i].x) * (1 / 3);
			a1.y = spline[i].y + (spline[i + 1].y - spline[i].y) * (1 / 3);
			a1.z = spline[i].z + (spline[i + 1].z - spline[i].z) * (1 / 3);

			a2.x = spline[i].x + (spline[i + 1].x - spline[i].x) * (2 / 3);
			a2.y = spline[i].y + (spline[i + 1].y - spline[i].y) * (2 / 3);
			a2.z = spline[i].z + (spline[i + 1].z - spline[i].z) * (2 / 3);

			for (float j = 0.0f; j < 1.0f; j += w)
			{
				a = lerp3v(&spline[i], &a1, j, ipFlag);
				b = lerp3v(&a1, &a2, j, ipFlag);
				c = lerp3v(&a2, &spline[i + 1], j, ipFlag);

				d = lerp3v(&a, &b, j, ipFlag);
				e = lerp3v(&b, &c, j, ipFlag);

				splineData[splineIndex] = lerp3v(&d, &e, j, ipFlag);
				splineIndex++;
			}
		}
	}
}

Camera createCamera(Vec3f initialPosition, Vec3f initialFront, Vec3f initialUp, float initialYaw, float initialRoll, float initialPitch)
{
	Camera camera;

	camera.position = initialPosition;
	camera.front = initialFront;
	camera.up = initialUp;
	camera.yaw = initialYaw;
	camera.roll = initialRoll;
	camera.pitch = initialPitch;

	return(camera);
}

typedef struct Transformation
{
	float xTrans;
	float yTrans;
	float zTrans;

	float xRotate;
	float yRotate;
	float zRotate;

	float xScale;
	float yScale;
	float zScale;

	float eyeMovement;

	float headMove;
	BOOL bHeadMove = TRUE;
	float rotAntena = 0.0f;
	BOOL bRotAntena = TRUE;

	Vec3f rLegs;

	BOOL bBeeWings = TRUE;

	BOOL bEye = TRUE;

	BOOL bBeeFly = TRUE;

	Vec3f wingRotate = { -90.0f,0.0f,0.0f };

	Transformation() : xTrans(0.0f), yTrans(0.0f), zTrans(0.0f), xRotate(0.0f), yRotate(0.0f), zRotate(0.0f), xScale(0.0f), yScale(0.0f), zScale(0.0f) {}

	Transformation(float x1,float y1,float z1,float x2,float y2,float z2,float x3,float y3,float z3) : xTrans(x1), yTrans(y1), zTrans(z1), xScale(x2), yScale(y2), zScale(z2), xRotate(x3), yRotate(y3), zRotate(z3) {}

}Transformation;

void cube(void)
{
	//*********************************** Cube *************************************
	glBegin(GL_QUADS);
	//Front face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0f, -1.0f, 1.0f);
	//Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0f, -1.0f, -1.0f);
	//Back face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	//Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	//Top Face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);
	//Bottom Face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();
}

typedef struct Terrain
{
	Vec3f posRightTop;
	Vec3f posLeftTop;
	Vec3f posLeftBottom;
	Vec3f posRightBottom;

	Vec3f colRightTop;
	Vec3f colLeftTop;
	Vec3f colLeftBottom;
	Vec3f colRightBottom;
}Terrain;

typedef struct Light
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[4];

	void LightIntensity(float factor)
	{
		ambient[0] = ambient[0] * factor;
		ambient[1] = ambient[1] * factor;
		ambient[2] = ambient[2] * factor;

		diffuse[0] = diffuse[0] * factor;
		diffuse[1] = diffuse[1] * factor;
		diffuse[2] = diffuse[2] * factor;

		specular[0] = specular[0] * factor;
		specular[1] = specular[1] * factor;
		specular[2] = specular[2] * factor;
	}

}Light;



