#pragma once

#define PI 3.14159
enum
{
	USESMOOTHERSTEP = 0,
	USELINEAR,
	USELINEARINCREAMENT,
	USEBAZIERSPLINE,
	USEBAZIERCURVE
};



typedef struct Angle
{
	enum Unit
	{
		Degrees,
		Radians
	};
	
	double value;
	Unit unit;

	Angle(double value, Unit unit = Degrees) : value(value), unit(unit) {}

	double getValue() const
	{
		return(value);
	}

	Unit getUnit() const
	{
		return(unit);
	}

	double toDegrees() const
	{
		return(unit == Degrees ? value : value * 180.0 / PI);
	}

	double toRadians() const
	{
		return(unit == Radians ? value : value * PI / 180.0);
	}

}Angle;




typedef struct Vec3f
{
	float x;
	float y;
	float z;

	//Constructor: to default initialize the 
	Vec3f() : x(0.0f), y(0.0f), z(0.0f) {}

	//parameterized constructor
	Vec3f(float x_in, float y_in, float z_in) : x(x_in), y(y_in), z(z_in)
	{

	}

	float magnitudeVec3f(void)//Represends the length
	{
		return(sqrt(x * x + y * y + z * z));
	}

	void NormalizeVec3f(void)
	{
		float magnitude = magnitudeVec3f();
		if (magnitude != 0)
		{
			x = x / magnitude;
			y = y / magnitude;
			z = z / magnitude;
		}
	}

	//Distance
	float distance(Vec3f* relVec)
	{
		return(sqrt((x - relVec->x) * (x - relVec->x) + (y - relVec->y) * (y - relVec->y) + (z - relVec->z) * (z - relVec->z)));
	}
	
	//Dot
	float dotProduct(Vec3f* relVec, float theata)
	{
		return(sqrt(x * x + y * y + z * z) * sqrt(relVec->x * relVec->x + relVec->y * relVec->y + relVec->z * relVec->z) * cos(theata));
	}

	//Cross
	Vec3f Cross(Vec3f inputVector)
	{
		Vec3f result;

		result.x = (y * inputVector.z) - (z * inputVector.y);

		result.y = (z * inputVector.x) - (x * inputVector.z);

		result.z = (x * inputVector.y) - (y * inputVector.x);

		return(result);
	}


}Vec3f;




typedef struct Vec2f
{
	float x;
	float y;
	float magnitude = 0.0f;

	//Constructor: to default initialize the 
	Vec2f() : x(0.0f), y(0.0f) {}

	//parameterized constructor
	Vec2f(float x_in, float y_in, float z_in) : x(x_in), y(y_in)
	{

	}

	float magnitudeVec2f(void)
	{
		return(sqrt(x * x + y * y ));
	}

	void NormalizeVec2f(void)
	{
		float magnitude = magnitudeVec2f();
		x = x / magnitude;
		y = y / magnitude;
	}

	//Distance
	float distance(Vec2f* relVec)
	{
		return(sqrt((x - relVec->x) * (x - relVec->x) + (y - relVec->y) * (y - relVec->y)));
	}

	//Dot
	float dotProduct(Vec2f* relVec, float theata)
	{
		return(sqrt(x * x + y * y) * sqrt(relVec->x * relVec->x + relVec->y * relVec->y) * cos(theata));
	}
}Vec2f;



typedef struct Vec4f
{
	float x;
	float y;
	float z;
	float w;
	float magnitude = 0.0f;

	//Constructor: to default initialize the 
	Vec4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

	//parameterized constructor
	Vec4f(float x_in, float y_in, float z_in, float w_in) : x(x_in), y(y_in), z(z_in), w(w_in)
	{

	}

	float magnitudeVec4f(void)
	{
		return(sqrt(x * x + y * y + z * z + w * w));
	}

	void NormalizeVec4f(void)
	{
		float magnitude = magnitudeVec4f();
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
		w = w / magnitude;

	}

	//Distance
	float distance(Vec4f* relVec)
	{
		return(sqrt((x - relVec->x) * (x - relVec->x) + (y - relVec->y) * (y - relVec->y) + (z - relVec->z) * (z - relVec->z) + (w - relVec->w) * (w - relVec->w)));
	}

	//Dot
	float dotProduct(Vec4f* relVec, float theata)
	{
		return(sqrt(x * x + y * y + z * z + w * w) * sqrt(relVec->x * relVec->x + relVec->y * relVec->y + relVec->z * relVec->z + relVec->w * relVec->w) * cos(theata));
	}
}Vec4f;



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

