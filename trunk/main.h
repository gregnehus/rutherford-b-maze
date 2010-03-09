



typedef enum {
  west = 1,   // 0001
  north = 2,  // 0010
  east = 4,   // 0100
  south = 8   // 1000
} walls;

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
cell maze[MAZE_WIDTH][MAZE_HEIGHT];

void halt();
void turn_left(int duration);
void turn_right(int duration);
void look_left(int duration);
void look_right(int duration);
void dash(int duration);
int get_sonar();
void print_sonar();
void scan_cell();
void align_turret();
void get_neighbor_coordinate(int x, int y, walls dir, coord *n);
float get_distance(int x1, int y1, int x2, int y2);
walls choose_best_cell();
