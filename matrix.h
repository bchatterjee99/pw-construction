
double** create_matrix(int n, int m);


void destroy_matrix(double** A, int num_rows);


// copy column b[y] into a[x]
void copy_column(double **a, int x, double** b, int y, int num_rows);


// swap rows a[x] and a[y]
double swap_row(double** a, int n, int x, int y);


// store a inverse in b
void inverse(double** a, int n, double** b);

