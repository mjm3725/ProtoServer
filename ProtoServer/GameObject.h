#pragma once

class GameObject : public enable_shared_from_this<GameObject>
{
public: 
	GameObject(__int64 handle, string& name);

	void Update(float elapsedTime);


private:
	__int64 _handle;
	string _name;
};
