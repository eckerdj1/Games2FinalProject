
#include "GameObject.h"

GameObject::GameObject()
{
	radii = Vector3(0,0,0);
	size = Vector3(0,0,0);
	direction = Vector3(0,0,0);
	speed = 0;
	active = true;
	Identity(&world);
}

GameObject::~GameObject()
{
	//delete box;
	box = NULL;
}

void GameObject::draw()
{
	if (!active)
		return;
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
        box->draw();
    }
		/*box->draw();*/
}

void GameObject::init(Box *b, Vector3 pos, Vector3 dir, Vector3 size, float speed)
{
	box = b;
	radii = size / 2.0f;
	position = pos;
	direction = dir;
	this->speed = speed;
	this->size = size;

	corners.push_back(Vector3(radii.x, size.y, radii.z));
	corners.push_back(Vector3(-radii.x, size.y, radii.z));
	corners.push_back(Vector3(radii.x, 0, radii.z));
	corners.push_back(Vector3(-radii.x, 0, radii.z));
	corners.push_back(Vector3(radii.x, size.y, -radii.z));
	corners.push_back(Vector3(-radii.x, size.y, -radii.z));
	corners.push_back(Vector3(radii.x, 0, -radii.z));
	corners.push_back(Vector3(-radii.x, 0, -radii.z));
}

void GameObject::update(float dt)
{
	if (!active)
		return;
	position += direction * speed * dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);


}


float GameObject::getBoxBottom()
{
	return position.y - 1.0f;
}



bool GameObject::onTopOf(GameObject *gameObject)
{
	if (!active || gameObject->isNotActive())
		return false;
	if (isAbove(gameObject))
		if (position.y - gameObject->position.y > 1.95f && position.y - gameObject->position.y < 2.05f)
		{
			return true;
		}
	return false;
}

bool GameObject::isAbove(GameObject* gameObject)
{
	if (!active)// || gameObject->isNotActive())
		return false;
	if (position.y - gameObject->position.y >= 1.95f)
	{
		if (fabs(position.x - gameObject->position.x) <= 0.1f && fabs(position.z - gameObject->position.z) <= 0.1f)
			return true;
		else 
			return false;
	}
	return false;
}

void GameObject::normlizeDirection()
{
	//float distSquared = velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z;
	//Vector3 vSquared(velocity.x * velocity.x, velocity.y * velocity.y, velocity.z * velocity.z);
	//velocity = vSquared / distSquared;
	if (direction.x != 0)
		direction.x /= fabs(direction.x);
	if (direction.y != 0)
		direction.y /= fabs(direction.y);
	if (direction.z != 0)
		direction.z /= fabs(direction.z);
}

void GameObject::deleteBox()
{
	box = 0;
}

bool GameObject::contains(Vector3 point)
{
	if (point.z >= position.z - zRadius() && point.z <= position.z + zRadius())
	{
		if (point.y >= position.y - yRadius() && point.y <= position.y + yRadius())
		{
			if (point.x >= position.x - xRadius() && point.x <= position.x + xRadius())
			{
				return true;
			}
		}
	}
	return false;
}

DXColor GameObject::colorAtPoint(float zPos)
{
	DXColor c1, c2, c3;
	c1 = box->getColor1();
	c2 = box->getColor2();
	if (c1 == c2)
		return c1;
	c3 = c1;
	float colorPos = (zPos - position.z - zRadius()) / size.z;
	if (c1.r != c2.r)
		c3.r += ((c1.r - c2.r) * colorPos);
	if (c1.g != c2.g)
		c3.g += ((c1.g - c2.g) * colorPos);
	if (c1.b != c2.b)
		c3.b += ((c1.b - c2.b) * colorPos);

	return c3;		
}

float GameObject::xRadius()
{
	return size.x / 2.0f;
}
float GameObject::yRadius()
{
	return size.y / 2.0f;
}
float GameObject::zRadius()
{
	return size.z / 2.0f;
}

Vector3 GameObject::cornerAt(int i)
{
	return corners[i];
}