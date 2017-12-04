#include "thpc_platform.h"
using namespace thpc;


void Draw::reset()
{
	this->setFillColor(0,0,0);
	this->setLineColor(255,255,255);
	this->lineWidth = 0.f;
}

void Draw::clear(RGBA rgba)
{
	this->clear(rgba.r,rgba.g,rgba.b);
}

void Draw::setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->fillColor.r = r;
	this->fillColor.g = g;
	this->fillColor.b = b;
	this->fillColor.a = a;
}

void Draw::setFillColor(RGBA rgba)
{
	this->setFillColor(rgba.r,rgba.g,rgba.b,rgba.a);
}

void Draw::setLineColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->lineColor.r = r;
	this->lineColor.g = g;
	this->lineColor.b = b;
	this->lineColor.a = a;
}

void Draw::setLineColor(RGBA rgba)
{
	this->setLineColor(rgba.r,rgba.g,rgba.b,rgba.a);
}

void Draw::setLineWidth(float thickness)
{
	this->lineWidth = thickness;
}
