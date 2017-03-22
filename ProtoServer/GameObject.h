#pragma once

class GameObject
{
public: 
	void Update(float elapsedTime);


private:
	__int64 _handle;
	string _name;
};
