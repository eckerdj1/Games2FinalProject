#pragma once

#include "d3dUtil.h"
#include "constants.h"
#include <locale>
#include <codecvt>
#include <string>
#include <sstream>
using std::string;
using std::wstring;
using std::wostringstream;
using std::ostringstream;



class Text
{
public:
	Text();
	~Text();

	static ID3D10Device* device;
	
	void init(wstring fontName, int size);

	void draw(string text, Vector2 position, Color color = Black);

	static wstring toWString(string str);
	static const wchar_t* toWCString(string str);
	static string toString(int n);
	static void setDevice(ID3D10Device* deviceP);

private:

	string text;
	Vector2 position;
	
	ID3DX10Font* font;

};