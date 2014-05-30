#ifndef EASYGL_CONSTANTS_H
#define EASYGL_CONSTANTS_H

/* NO_COLOR can be used as a flag to indicate not drawn. WHITE is the first color. */
enum color_types {NO_COLOR = -1, WHITE = 0, BLACK, DARKGREY, LIGHTGREY, BLUE, GREEN, YELLOW,
CYAN, RED, PINK, LIGHTPINK, DARKGREEN, MAGENTA, BISQUE, LIGHTSKYBLUE, THISTLE, PLUM, KHAKI, CORAL,
TURQUOISE, MEDIUMPURPLE, DARKSLATEBLUE, DARKKHAKI, LIGHTMEDIUMBLUE, SADDLEBROWN, FIREBRICK, LIMEGREEN, NUM_COLOR};

enum line_types {SOLID, DASHED};

#define MAXPTS 100    /* Maximum number of points drawable by fillpoly */

typedef struct s_point {
	float x; 
	float y;

	#ifdef __cplusplus

	struct s_point operator+ (const struct s_point& rhs) const {
		struct s_point result = *this;
		result.x += rhs.x;
		result.y += rhs.y;
		return result;
	}

	struct s_point& operator+= (const struct s_point& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	s_point() { }
	s_point(float _x, float _y) : x(_x), y(_y) { }

	struct s_point& offset(float _x, float _y) {
		x += _x;
		y += _y;
		return *this;
	}

	#endif
} t_point; /* Used in calls to fillpoly, and elsewhere */

typedef struct {
	bool shift_pressed;  /* indicates whether a Shift key was pressed when a mouse button is pressed */
	bool ctrl_pressed;	/* indicates whether a Ctrl key was pressed when a mouse button is pressed */
	unsigned int button; /* indicates what button is pressed: left click is 1; right click is 3; */
						 /* scroll wheel click is 2; scroll wheel forward rotate is 4; */
						 /* scroll wheel backward is 5. */
} t_event_buttonPressed;   /* Used to pass information from event_loop when a mouse button or modifier key is pressed */

#endif // EASYGL_CONSTANTS_H
