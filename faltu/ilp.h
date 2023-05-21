

void input_matrix(long A[][200], const char* file, int* n);

void input_constraint(long A[][200], long b[], long c[], const char* file, int* C_n, int* C_m);

void show_vector(long b[], int n, char* str);

void show_matrix(long A[][200], int n, char* str);

void show_ilp(long C[][200], long b[], long c[], int n, int m);

void mult_matrix(long A[][200], long B[][200], long C[][200], int n);

void mult_vector(long A[][200], long b[], long c[], int n);
void fmult_vector(long A[][200], double b[], double c[], int n);

int check1(long f[], long Constraint[][200], long b[], long c[], int C_n, int C_m);

int check2(long f[], long Constraint[][200], long b[], long c[], int C_n, int C_m);

int same(long a[], long b[], int n);
