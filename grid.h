
extern int cell_size;
extern int num_row; // number of rows
extern int num_col; // number of columns


void grid_draw(int num_cell);

void grid_fill_cell(int pos, int color);

void grid_mark_cell(int pos);

void grid_update();
