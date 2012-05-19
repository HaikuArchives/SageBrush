/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */
#ifndef RULER_H
#define RULER_H

#include <SupportDefs.h>
#include <GraphicsDefs.h>
#include <InterfaceDefs.h>
#include <View.h>
#include "Units.h"

class Ruler: public BView
{
	orientation fOrientation;
	float ratio;
	rgb_color markcolor;
	Units::Type units;
	float rangeMin, rangeMax, mark; // in pixels

  public:
	Ruler(orientation direction, int32 align, 
		float border_start = 0.0, float border_end = 0.0);
	~Ruler();

	virtual void AttachedToWindow(void);
	virtual void Draw(BRect updateRect);
	virtual void WindowActivated(bool state);
	
	orientation Orientation(void) const;
	
	void SetRatio(float ratio);
	inline float Ratio(void) const;
	
	void SetUnits(Units::Type u);
	
	void SetMark(float new_mark);
	void HideMark(void);
	inline float Mark(void) const;
	void SetMarkColor(rgb_color color);
	inline void SetMarkColor(uint8 r, uint8 g, uint8 b, uint8 alpha = 255);
	
	inline rgb_color MarkColor(void) const;
	
	int32 Align(void) const;
	
	void SetRange(float a, float b, Units::Type u = Units::PIXELS);
	inline void GetRangeInPixels(float &a, float &b) const;
  public:
	static void set_v_width(float width);
	static void set_h_height(float height);
	static float v_width(void);
	static float h_height(void);
};

inline void Ruler::SetMarkColor(uint8 r, uint8 g, uint8 b, uint8 alpha = 255)
{
	rgb_color color = {r, g, b, alpha};
	SetMarkColor(color);
}
	
inline rgb_color Ruler::MarkColor(void) const
{
	return markcolor;
}

inline float Ruler::Ratio(void) const
{
	return ratio;
}

inline void Ruler::GetRangeInPixels(float &a, float &b) const
{
	a = rangeMin;
	b = rangeMax;
}

inline float Ruler::Mark(void) const
{
	return mark;
}

#endif /* RULER_H */