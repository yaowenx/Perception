#include "kalman/ekfilter.hpp"

class TrackerEKF : public Kalman::EKFilter<float,1> {
public:
	TrackerEKF();

protected:

	void makeA();
	void makeH();
	void makeV();
	void makeR();
	void makeW();
	void makeQ();
	void makeProcess();
	void makeMeasure();

	float Period, Mass, Bfriction, Portance, Gravity;
};

typedef TrackerEKF::Vector Vector;
typedef TrackerEKF::Matrix Matrix;


TrackerEKF::TrackerEKF()
{
	setDim(4, 1, 2, 2, 2);
	Period = 0.1;
}

void TrackerEKF::makeA()
{
	A(1,1) = 1.0;
	A(1,2) = Period;
	A(1,3) = 0.0;
	A(1,4) = 0.0;

	A(2,1) = 0.0;
	A(2,2) = 1.0;
	A(2,3) = 0.0;
	A(2,4) = 0.0;

	A(3,1) = 0.0;
	A(3,2) = 0.0;
	A(3,3) = 1.0;
	A(3,4) = Period;

	A(4,1) = 0.0;
	A(4,2) = 0.0;
	A(4,3) = 0.0;
	A(4,4) = 1.0;
}

void TrackerEKF::makeW()
{
	W(1,1) = 0.0;
	W(1,2) = 0.0;
	W(2,1) = 1.0;
	W(2,2) = 0.0;
	W(3,1) = 0.0;
	W(3,2) = 0.0;
	W(4,1) = 0.0;
	W(4,2) = 1.0;
}

void TrackerEKF::makeQ()
{
	Q(1,1) = 0.01*0.01;
	Q(1,2) = 0.01*0.01/10.0;
	Q(2,1) = 0.01*0.01/10.0;
	Q(2,2) = 0.01*0.01;
}

void TrackerEKF::makeH()
{
	H(1,1) = 1.0;
	H(1,2) = 0.0;
	H(1,3) = 0.0;
	H(1,4) = 0.0;

	H(2,1) = 0.0;
	H(2,2) = 0.0;
	H(2,3) = 1.0;
	H(2,4) = 0.0;
}

void TrackerEKF::makeV()
{
	V(1,1) = 1.0;
        V(1,2) = 0.0;
        V(2,1) = 0.0;
	V(2,2) = 1.0;
}

void TrackerEKF::makeR()
{
	R(1,1) = 0.01*0.01;
        R(1,2) = 0.0;
        R(2,1) = 0.0;
	R(2,2) = 50*50;
}

void TrackerEKF::makeProcess()
{
	Vector x_(x.size());
	x_(1) = x(1) + x(2)*Period;
	x_(2) = x(2);
	x_(3) = x(3) + x(4)*Period;
	x_(4) = x(4);
	x.swap(x_);
}

void TrackerEKF::makeMeasure()
{
	z(1)=x(1);
	z(2)=x(3);
}
