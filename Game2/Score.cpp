#include "Score.h"

Score::Score()
{
	score = 0;
	multiplier = 3.0f;
}
Score::~Score()
{

}

void Score::init()
{

}

void Score::draw()
{

}

void Score::addPoints(int points)
{
	score += points * multiplier;
}

void Score::subtractPoints(int points)
{
	score -= points;
}

string Score::getString()
{
	out.str("");
	out << "Player Score: " << score << "\nMultiplier: " << multiplier;
	return out.str();
}

int Score::getInt()
{
	return score;
}