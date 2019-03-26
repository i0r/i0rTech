#pragma once

struct device_mouse_t {
	bool      LeftButton;
	bool      RightButton;
	bool      MiddleButton;
	bool      Extra1Button;
	bool      Extra2Button;

	bool      RelativeMouse;

	i32       PositionX;
	i32       PositionY;
	
	i32       PreviousX;
	i32       PreviousY;

	i32       WheelX;
	i32       WheelY;

	i32       PreviousWheelX;
	i32       PreviousWheelY;

	INLINE void SetRelative( const bool state ) {
		this->RelativeMouse = state;
	}
};
