#include "Text.h"


ID3D10Device* Text::device;

Text::Text()
{
	text = "";
	position = Vector2();
	font = 0;
}
Text::~Text()
{
	ReleaseCOM(font);
}

//static class functions
wstring Text::toWString(string str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	static wstring newStr = converter.from_bytes(str);
	newStr = converter.from_bytes(str);
	return newStr;
}
const wchar_t* Text::toWCString(string str)
{	
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	static wstring newStr = converter.from_bytes(str);
	newStr = converter.from_bytes(str);
	return newStr.c_str();
}
string Text::toString(int n)
{
	static ostringstream out;
	out.str("");
	out.clear();
	out << n;
	string newStr = out.str();
	return newStr;
}
void Text::setDevice(ID3D10Device* deviceP)
{
	device = deviceP;
}

void Text::init(wstring fontName, int size)
{
	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = size;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy(fontDesc.FaceName, fontName.c_str());// toWString(fontName).c_str());

	D3DX10CreateFontIndirect(device, &fontDesc, &font);
}

void Text::draw(string text, Vector2 position, DXColor color)
{
	RECT R = {position.x, position.y, 0, 0};
	LPCWSTR str = toWCString(text);
	font->DrawText(0, str, -1, &R, DT_NOCLIP, color);
}

