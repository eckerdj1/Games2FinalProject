
#include "BodyPart.h"

BodyPart::BodyPart()
{
	GameObject();
	scaleX = scaleY = scaleZ = 1.0f;
	rotX = rotY = rotZ = 0.0f;
	transX = transY = transZ = 0.0f;
	root = 0;

}

void BodyPart::init(string _name, Box* b, Vector3 pos, Vector3 dir, Vector3 size, float speed)
{
	name = _name;
	GameObject::init(b, pos, dir, size, speed);
}

void BodyPart::addChild(BodyPart* part)
{
	children.push_back(part);
}

void BodyPart::setBody(Body* p)
{
	body = p;
}
void BodyPart::setRoot(BodyPart* part)
{
	root = part;
}

void BodyPart::update(float dt)
{
	Identity(&world);
	transX = position.x;
	transY = position.y;
	transZ = position.z;
	rotY = atan2f(direction.x, direction.z);
	if (name == "torso" || name == "base" || name == "Body")
	{
		//position += direction * getSpeed() * dt;
		traverse(world);
	}
}

void BodyPart::traverse(Matrix m)
{
	Matrix currentWorld, scale, trans, ry, rx, rz;
	Identity(&currentWorld);
	Identity(&scale);
	Identity(&trans);
	Identity(&ry);
	Identity(&rx);
	Identity(&rz);
	
	Scale(&scale, scaleX, scaleY, scaleZ);
	Translate(&trans, transX, transY, transZ);
	RotateX(&rx, rotX);
	RotateY(&ry, rotY);
	RotateZ(&rz, rotZ);
	currentWorld = scale * rz * rx * ry * trans * m;
	world = currentWorld;
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->traverse(currentWorld);
	}
}

void BodyPart::draw(Matrix mVP)
{
	body->mfxWorldVar->SetMatrix(world);
	body->mfxWVPVar->SetMatrix(world * mVP);
	body->mTech->GetPassByIndex(0)->Apply(0);
	if (name == "head")
	{
		box->rescale(getSize().x, getSize().y, getSize().z);
		box->draw();
	}
	else
	{
		box->rescale(getSize().x, getSize().y, getSize().z);
		box->draw();
	}
	for (int i=0; i<children.size(); ++i)
	{
		children[i]->draw(mVP);
	}
}

Matrix BodyPart::getWorld()
{
	return world;
}

void BodyPart::reduceScaleByFactor(float r)
{
	scaleX /= r;
	scaleY /= r;
	scaleZ /= r;
}