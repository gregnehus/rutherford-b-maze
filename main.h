



typedef enum {
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

}directions;

typedef struct{

  int x;
  int y;
}coord;




typedef struct {
  walls cell_walls;
  bool visited;
} cell;

typedef struct{
  int x;
  int y;

  int angle;
} position;



position curr_position;


void halt();
void set_turret_angle(directions angle);
void set_base_angle(directions angle);
void turn_base(int angle, int direction);
void turn_turret(int angle, int direction);
void dash();
void initialize_maze();
int get_sonar();
void scan_cell();
void scan_wall(walls w);
void align_turret();
bool get_neighbor_coordinate(int x, int y, walls dir, coord *n);
float get_distance(int x1, int y1, int x2, int y2);
walls choose_best_cell();
void get_cell_pixel_origin(int x, int y, coord *n);
void get_cell_pixel_center(int x, int y, coord *n);
void draw_cell_wall(int x, int y, walls dir);