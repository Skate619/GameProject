#ifndef THPC_PLATFORM_H
#define THPC_PLATFORM_H


namespace thpc
{
	struct floatXY
	{
		float x,y;
	};
	
	struct RGBA
	{
		char r,g,b,a;
	};

	struct Draw
	{
		void reset();
		void clear(unsigned char r=0, unsigned char g=0, unsigned char b=0);
		void clear(RGBA rgba);
		void setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
		void setFillColor(RGBA rgba);
		void setLineColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
		void setLineColor(RGBA rgba);
		void setLineWidth(float thickness);
		void line(floatXY a, floatXY b);
		void rect(floatXY pos, floatXY size);
		void circle(floatXY center, float radius);
		void tri(floatXY a, floatXY b, floatXY c);
		void poly(floatXY *points, size_t count);

		floatXY winSize;
		RGBA fillColor,lineColor;
		float lineWidth;
	};

	struct Platform
	{
		Draw draw;
		void kill();
	};
};


#endif