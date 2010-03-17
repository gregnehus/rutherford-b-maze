/**********************************************************************************
* File:         main.h
* Authors:      Greg Nehus, Matt Odille
*********************************************************************************/

/**********************************************************************************
* Typedefs
*********************************************************************************/
typedef enum{
	  west = 1,   // 0001
	  north = 2,  // 0010
	  east = 4,   // 0100
	  south = 8   // 1000
} walls;

typedef enum{
    dWest = 0,
    dNorth = 90,
    dEast = 180,
    dSouth = 270
} directions;

typedef struct{
	  int x;
	  int y;
} coord;

typedef struct{
	  walls cell_walls;
	  bool visited;
} cell;

typedef struct{
	  int x;
	  int y;
	  int angle;
} position;


/**********************************************************************************
* Global Variables
*********************************************************************************/
position curr_position;


/**********************************************************************************
* Function prototypes
*********************************************************************************/
void halt();
void set_turret_angle(directions angle);
void set_base_angle(directions angle);
void turn_base(int angle, int direction);
void turn_turret(int angle, int direction, directions iAngle);
void dash();
void scan_cell();
void scan_wall(walls w, walls c);
void align_turret();
bool get_neighbor_coordinate(int x, int y, walls dir, coord *n);
float get_distance(int x1, int y1, int x2, int y2);
walls choose_best_cell();
void display_map();
void draw_cell(int x, int y);
void draw_cell_wall(int x, int y, walls dir);
void get_cell_pixel_origin(int x, int y, coord *n);
void get_cell_pixel_center(int x, int y, coord *n);
void initialize_maze();
task we_are_the_champions();
void navigate_to_cell(int dest_x, int dest_y);
void adjust(float distance);
void dash_wallfollow();
void draw_destination(int x, int y);
