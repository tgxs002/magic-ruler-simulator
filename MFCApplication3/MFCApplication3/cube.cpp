
#include "cube.h"
#include <math.h>
#include <iostream>


cube::cube(Vector3f &p0, Vector3f &p1, Vector3f &p2, Vector3f &p3, Vector3f &p4, Vector3f &p5, int number)
{	
	points[0] = p0;
	points[1] = p1;
	points[2] = p2;
	points[3] = p3;
	points[4] = p4;
	points[5] = p5;
	this->number = number;
}

Matrix4f &cube::getRotMat(float angle)const
{
	Matrix4f left, middle, right;

	angle *= float(3.1415926/180);
	float cosa = cosf(angle);
	float mico = 1 - cosf(angle);
	float sina = sinf(angle);

    left.m[0][0] = 1.0f;		 left.m[0][1] = 0.0f;		 left.m[0][2] = 0.0f;        left.m[0][3] = (points[3].x + points[5].x)/2;
    left.m[1][0] = 0.0f        ; left.m[1][1] = 1.0f;	     left.m[1][2] = 0.0f;        left.m[1][3] = (points[3].y + points[5].y)/2;
    left.m[2][0] = 0.0f        ; left.m[2][1] = 0.0f;        left.m[2][2] = 1.0f;		 left.m[2][3] = (points[3].z + points[5].z)/2;
    left.m[3][0] = 0.0f        ; left.m[3][1] = 0.0f;        left.m[3][2] = 0.0f;        left.m[3][3] = 1.0f;

	

	right.m[0][0] = 1.0f 		; right.m[0][1] = 0.0f;		 right.m[0][2] = 0.0f;        right.m[0][3] = -(points[3].x + points[5].x)/2;
    right.m[1][0] = 0.0f        ; right.m[1][1] = 1.0f;	     right.m[1][2] = 0.0f;        right.m[1][3] = -(points[3].y + points[5].y)/2;
    right.m[2][0] = 0.0f        ; right.m[2][1] = 0.0f;      right.m[2][2] = 1.0f;		  right.m[2][3] = -(points[3].z + points[5].z)/2;
    right.m[3][0] = 0.0f        ; right.m[3][1] = 0.0f;      right.m[3][2] = 0.0f;        right.m[3][3] = 1.0f;
	
	Vector3f v1 = points[2] - points[4];
	Vector3f v2 = points[3] - points[5];
	
	Vector3f v3 = v2.Cross(v1);
	v3.Normalize();
	float x = v3.x;
	float y = v3.y;
	float z = v3.z;

	

	middle.m[0][0] = cosa + mico * x * x	 ; middle.m[0][1] = -z * sina + mico * x * y; middle.m[0][2] = y * sina + mico * x * z	; middle.m[0][3] = 0.0f;
    middle.m[1][0] = z * sina + mico * x * y ; middle.m[1][1] = cosa + mico * y * y		; middle.m[1][2] = -x * sina + mico * y * z	; middle.m[1][3] = 0.0f;
    middle.m[2][0] = -y * sina + mico * x * z; middle.m[2][1] = x * sina + mico * y * z	; middle.m[2][2] = cosa + mico * z * z		; middle.m[2][3] = 0.0f;
    middle.m[3][0] = 0.0f					 ; middle.m[3][1] = 0.0f 					; middle.m[3][2] = 0.0f						; middle.m[3][3] = 1.0f;

	return  left * middle * right;
}

void cube::rot(Matrix4f matrix)
{
	for(int i = 0; i < 6; i++)
	{
		points[i] = (matrix * Vector4f(points[i].x, points[i].y, points[i].z, 1)).to3f();
	}
}

Vector3f* cube::getPoints()
{
	return &points[0];
}

ruler::ruler(int n_)
{
	this->n = n_;
	Cube = new cube[n];
	n_ = n_/2;
	for(int i=0; i<n_; i++)
	{
		Cube[2*i] = cube(
			Vector3f(0.0f, float(i), float(i)), 
			Vector3f(1.0f, float(i), float(i)), 
			Vector3f(1.0f, float(i), float(i)+1.0f), 
			Vector3f(0.0f, float(i), float(i)+1), 
			Vector3f(0.0f, float(i)+1.0f, float(i)+1.0f), 
			Vector3f(1.0f, float(i)+1.0f, float(i)+1.0f), 2 * i);

		Cube[2*i+1] = cube(
			Vector3f(1, float(i), float(i)+1), 
			Vector3f(0.0f, float(i), float(i)+1.0f), 
			Vector3f(0, float(i)+1.0f, float(i)+1.0f), 
			Vector3f(1.0f, float(i)+1.0f, float(i)+1.0f), 
			Vector3f(1, float(i)+1.0f, float(i)+2.0f), 
			Vector3f(0.0f, float(i)+1.0f, float(i)+2.0f), 2 * i + 1);
	}
}

ruler::~ruler()
{
	delete []Cube;
}

void ruler::rotate(int index, int angle)
{
	if(index < -n/2 || index > n-1 || index == 0)
		return;
	else
	{
		if(index < 0)
		{
			index = n + index;
		}

		Matrix4f mat = Cube[index - 1].getRotMat(angle);
		if(index <= n/2)
		{
			for(int i=0; i< index; i++)
			{
				Cube[i].rot(mat);
			}
		}
		else
		{
			for(int i=index; i<n; i++)
			{
				Cube[i].rot(mat);
			}
		}
	}
	return;
}

Vector3f ruler::getCore()const
{
	Vector3f result(0.0f, 0.0f, 0.0f);
	for (int i=0; i < n; i++)
	{
		result += Cube[i].getPoints()[0];
	}
	return Vector3f(result.x / n, result.y / n, result.z / n);
}

void ruler::rotate(Vector3f pos, Vector3f v3, float angle)
{
	Matrix4f left, middle, right;

	angle *= float(3.1415926/180);
	float cosa = cosf(angle);
	float mico = 1 - cosf(angle);
	float sina = sinf(angle);

    left.m[0][0] = 1.0f;		 left.m[0][1] = 0.0f;		 left.m[0][2] = 0.0f;        left.m[0][3] = pos.x;
    left.m[1][0] = 0.0f        ; left.m[1][1] = 1.0f;	     left.m[1][2] = 0.0f;        left.m[1][3] = pos.y;
    left.m[2][0] = 0.0f        ; left.m[2][1] = 0.0f;        left.m[2][2] = 1.0f;		 left.m[2][3] = pos.z;
    left.m[3][0] = 0.0f        ; left.m[3][1] = 0.0f;        left.m[3][2] = 0.0f;        left.m[3][3] = 1.0f;

	

	right.m[0][0] = 1.0f 		; right.m[0][1] = 0.0f;		 right.m[0][2] = 0.0f;        right.m[0][3] = -pos.x;
    right.m[1][0] = 0.0f        ; right.m[1][1] = 1.0f;	     right.m[1][2] = 0.0f;        right.m[1][3] = -pos.y;
    right.m[2][0] = 0.0f        ; right.m[2][1] = 0.0f;      right.m[2][2] = 1.0f;		  right.m[2][3] = -pos.z;
    right.m[3][0] = 0.0f        ; right.m[3][1] = 0.0f;      right.m[3][2] = 0.0f;        right.m[3][3] = 1.0f;
	
	v3.Normalize();
	float x = v3.x;
	float y = v3.y;
	float z = v3.z;

	

	middle.m[0][0] = cosa + mico * x * x	 ; middle.m[0][1] = -z * sina + mico * x * y; middle.m[0][2] = y * sina + mico * x * z	; middle.m[0][3] = 0.0f;
    middle.m[1][0] = z * sina + mico * x * y ; middle.m[1][1] = cosa + mico * y * y		; middle.m[1][2] = -x * sina + mico * y * z	; middle.m[1][3] = 0.0f;
    middle.m[2][0] = -y * sina + mico * x * z; middle.m[2][1] = x * sina + mico * y * z	; middle.m[2][2] = cosa + mico * z * z		; middle.m[2][3] = 0.0f;
    middle.m[3][0] = 0.0f					 ; middle.m[3][1] = 0.0f 					; middle.m[3][2] = 0.0f						; middle.m[3][3] = 1.0f;

	Matrix4f mat =  left * middle * right;

	for (int i=0; i < n; i++)
	{
		Cube[i].rot(mat);
	}
	return;
}
