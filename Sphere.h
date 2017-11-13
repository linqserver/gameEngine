#pragma once
#include "GameObject.h"
class Sphere :
	public GameObject
{
public:
	Sphere();
	~Sphere();
	virtual void Render() ;
};

