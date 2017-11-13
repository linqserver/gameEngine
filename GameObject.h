#pragma once
class GameObject
{
public:
	GameObject();
	~GameObject();
	void move_obj(); //the movements are the same for all the objects (nextpos = pos + vel)
	void rotate_obj(); //rotations are the same for every objects too
	
};

