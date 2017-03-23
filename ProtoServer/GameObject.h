#pragma once

class GameObject : public enable_shared_from_this<GameObject>
{
public: 
	void Update(float elapsedTime);


private:
	__int64 _handle;
	string _name;
};
