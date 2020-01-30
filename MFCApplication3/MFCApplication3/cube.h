

#if !defined CUBE
#define CUBE

#include "ogldev_math_3d.h"
#include <vector>
class cube
{
public:
	cube(Vector3f &p0, Vector3f &p1, Vector3f &p2, Vector3f &p3, Vector3f &p4, Vector3f &p5, int number);
	cube(){};
	~cube(){}
	Matrix4f &getRotMat(float angle)const;
	void rot(Matrix4f matrix);
	Vector3f* getPoints();
private:
	Vector3f points[6];
	int number;
};

class ruler
{
public:
	ruler(int n);
	~ruler();
	void rotate(int index, int angle);
	void rotate(Vector3f pos, Vector3f dir, float angle);
	Vector3f getCore()const;

	int n;
	cube* Cube;
};

struct Sequence
{
	int totalNumber;
	int dimension;
	int present;
	vector<int> place, movement;
};

#endif