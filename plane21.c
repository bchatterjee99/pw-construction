#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "field21.h"
#include "keyboard.h"
#include "grid.h"
#include "plane21.h"

#define M_order 2097152
#define L_order 128
#define J_order 8
#define M_mult_order 2097151
#define L_mult_order 127
#define J_mult_order 7
#define Num_Points 16513 // M_mult_order / L_mult_order
#define M_char 2 // M characteristic
#define M_char_power 21 // M prime power
#define M_L_power 3 // M L extension power
unsigned long L_mult[L_mult_order];
unsigned long J_mult[J_mult_order];
unsigned long Points[Num_Points]; 
int Orbits[Num_Points]; // stores Orbit-id for each Point
int Orbit_count;
// rearrange points to group orbits together
int new_Pos[Num_Points]; 

int min(int a, int b)
{
    if(a <= b) return a;
    return b;
}

int find_order(unsigned long a)
{
    if(a == 0) return 0;
    int i = 1, min_good_power = M_mult_order;
    // a^p = 1 => p good power
    while(1)
    {
	if(M_mult_order % i == 0)
	{
	    unsigned long tmp;

	    tmp = field_exponent(a, i);
	    if(tmp == 1)
		min_good_power = min(min_good_power, i);

	    tmp = field_exponent(a, M_mult_order / i);
	    if(tmp == 1)
		min_good_power = min(min_good_power, M_mult_order / i);

	}
	i++;
	if(i * i > M_mult_order) break;
    }
    return min_good_power;
}

void find_LJ_mult()
{
    int L_count = 0, J_count = 0;
    unsigned long primitive = 0x2; // x
    unsigned long L_generator = field_exponent(primitive, M_mult_order / L_mult_order);
    unsigned long J_generator = field_exponent(primitive, M_mult_order / J_mult_order);

    unsigned long tmp;
    for(int i=0; i<L_mult_order; i++)
    {
	tmp = field_exponent(L_generator, i);
	L_mult[L_count++] = tmp;
    }
    for(int i=0; i<J_mult_order; i++)
    {
	tmp = field_exponent(J_generator, i);
	J_mult[J_count++] = tmp;
    }

    // check
    L_count = 0;
    for(int i=0; i<L_mult_order; i++)
    {
	int repeat = 0;
	for(int j=0; j<i; j++)
	    if(L_mult[i] == L_mult[j])
		repeat = 1;
	if(repeat == 0) L_count++;
    }
    J_count = 0;
    for(int i=0; i<J_mult_order; i++)
    {
	int repeat = 0;
	for(int j=0; j<i; j++)
	    if(J_mult[i] == J_mult[j])
		repeat = 1;
	if(repeat == 0) J_count++;
    }
    printf("L-count = %d  J-count = %d\n\n", L_count, J_count);
}

// merge a[0...n] and b[0...m] into c[]
void merge(unsigned long a[], int n, unsigned long b[], int m, unsigned long c[])
{
    unsigned long* tmp = (unsigned long *)malloc((m + n) * sizeof(unsigned long));
    int i = 0, j = 0;
    int idx = 0;
    while(i < n || j < m)
    {
	if(i >= n) { tmp[idx++] = b[j++]; }
	else if(j >= m) { tmp[idx++] = a[i++]; }
	else if(a[i] <= b[j]) { tmp[idx++] = a[i++]; }
	else { tmp[idx++] = b[j++]; }
    }
    for(int i=0; i<m+n; i++)
	c[i] = tmp[i];
    free(tmp);
}
void merge_sort_Points(unsigned long arr[], int l, int r)
{
    if(r == l) return;
    int mid = (l + r)/2;
    merge_sort_Points(arr, l, mid);
    merge_sort_Points(arr, mid+1, r);
    merge(arr + l, mid - l + 1, arr + mid + 1, r - mid, arr + l);
}
void find_Points()
{
    unsigned long primitive = 0x2; // x
    for(int i=0; i<Num_Points; i++)
	Points[i] = field_exponent(primitive, i);

    // sort check
    merge_sort_Points(Points, 0, Num_Points - 1);
    /* printf("Points:\n"); */
    /* for(int i=0; i<Num_Points; i++) */
    /* 	printf("%d: %ld\n", i, Points[i]); */
    /* printf("\n\n"); */
    int count = 0; if(Num_Points > 0) count++;
    for(int i=1; i<Num_Points; i++)
	if(Points[i] != Points[i-1]) count++;
    printf("Point count: %d\n\n", count);
}

int exp1(int a, int b)
{
    int ans = 1;
    while(b > 0)
    {
	if(b & 1)
	    ans = ans * a;
	a = a * a;
	b = b >> 1;
    }
    return ans;
}
// binary search for point
int bsearch_point(unsigned long x)
{
    int l = 0, r = Num_Points;
    while(l <= r)
    {
	int mid = (l + r)/2;
	if(Points[mid] == x) return mid;
	if(Points[mid] < x)
	    l = mid + 1;
	if(Points[mid] > x)
	    r = mid - 1;
    }
    return -1;
}
// return index of coset leader
int find_coset_leader(unsigned long a)
{
    for(int i=0; i<L_mult_order; i++)
    {
	unsigned long b = field_multiplication(a, L_mult[i]);
	int x = bsearch_point(b);
	if(x != -1) return x;
    }
    return -1;
}
void find_Orbits()
{
    unsigned long point, a, b;
    Orbit_count = 0; int point_count = 0;
    for(int i=0; i<Num_Points; i++) Orbits[i] = -1;
    for(int i=0; i<Num_Points; i++) 
    {
	printf("find_Orbits(): curr point -- %d/%d\n", i, Num_Points);
	if(Orbits[i] != -1) continue;
	point = Points[i];
	Orbit_count++;
	for(int j=0; j<J_mult_order; j++)
	{
	    // phi(J*)
	    a = field_multiplication(point, J_mult[j]);
	    for(int p = 0; p < M_char_power; p++) // includes identity
	    {
		// forbenius
		b = field_exponent(a, exp1(M_char, p));

                // coset leader for Lb (point identifier)
		int c = find_coset_leader(b);
		if(c == -1)
		{
		    printf("ERROR: cannot find coset leader\n\n");
		    destroy_plane();
		    exit(1);
		}

		// Now: Group-Action(i'th point) -> c'th point
		if(Orbits[c] == -1) // new point
		{
		    Orbits[c] = Orbit_count - 1; // place in current orbit
		    new_Pos[c] = point_count++; // place orbit points together
		}
	    }
	}
    }
    printf("Orbit Count: %d point_count: %d\n\n", Orbit_count, point_count);
}
void show_Orbit_sizes()
{
    int *count = (int*)malloc(Orbit_count*sizeof(int));
    for(int i=0; i<Orbit_count; i++) count[i] = 0;

    for(int i=0; i<Num_Points; i++)
	count[Orbits[i]]++;

    printf("Orbit sizes:\n");
    for(int i=0; i<Orbit_count; i++)
	printf("Orbit %d size: %d\n", i, count[i]);
    printf("\n\n");
    free(count);
}

int* orbit_color;
void plane_draw_Orbits()
{
    orbit_color = (int*)malloc(Orbit_count * sizeof(int));
    grid_draw(Num_Points);
    for(int i=0; i<Orbit_count; i++) orbit_color[i] = -1;
    for(int i=0; i<Num_Points; i++)
    {
	int o = Orbits[i];
	if(orbit_color[o] == -1)
	    orbit_color[o] = rand() & 0xffffff;
	grid_fill_cell(i, orbit_color[o]);
    }
}
void plane_draw_Orbits_grouped()
{
    orbit_color = (int*)malloc(Orbit_count * sizeof(int));
    grid_draw(Num_Points);
    for(int i=0; i<Orbit_count; i++) orbit_color[i] = -1;
    for(int i=0; i<Num_Points; i++)
    {
	int o = Orbits[i];
	if(orbit_color[o] == -1)
	    orbit_color[o] = rand() & 0xffffff;
	grid_fill_cell(new_Pos[i], orbit_color[o]);
    }
}

void plane_redraw_Orbits_grouped()
{
    for(int i=0; i<Num_Points; i++)
    {
	int o = Orbits[i];
	grid_fill_cell(new_Pos[i], orbit_color[o]);
    }
}

unsigned long trace_M_L(unsigned long a)
{
    unsigned long ans = 0;
    for(int i=0; i<M_L_power; i++)
    {
	unsigned long tmp = field_exponent(a, exp1(L_order, i));
	ans = field_addition(ans, tmp);
    }
    return ans;
}

void plane_draw_line(unsigned long alpha)
{
    // alpha = Points[10];
    int count = 0;
    for(int i=0; i<Num_Points; i++)
    {
	unsigned long point = Points[i];
	unsigned long t = trace_M_L(field_multiplication(alpha, point));
	if(t == 0)
	{
	    grid_mark_cell(new_Pos[i]);
	    count++;
	}
	/* grid_mark_cell(i * 5); */
	/* if(i >= counter) break; */
    }
    // printf("points on line: %d\n\n", count);
}

void create_plane()
{
    find_LJ_mult();
    find_Points();
    find_Orbits();
    show_Orbit_sizes();
}

void destroy_plane()
{
    free(orbit_color);
}

void plane_animate_random_lines()
{
    plane_draw_Orbits_grouped();
    int counter = 30;
    while(counter--)
    {
	unsigned long alpha = rand() % M_order;
	plane_redraw_Orbits_grouped();
	plane_draw_line(alpha);
	grid_update();
	if(key_press()) break;
	sleep(2);
    }
}

void plane_animate_Orbit_lines(int orbit)
{
    plane_draw_Orbits_grouped();
    int counter = 100; int idx = 0;
    while(counter--)
    {
	// pick next line from orbit
	while(Orbits[idx] != orbit) idx++;
	unsigned long alpha = Points[idx]; // Point represntative => line representative
	idx++; // skip point
	idx = idx % Num_Points; // restart 

	printf("line: 0x%02x\n\n", alpha);
	plane_redraw_Orbits_grouped();
	plane_draw_line(alpha);
	grid_update();
	if(key_press()) break;
	sleep(2);
    }
}

void construct_ILP()
{
    // num of incident points in each orbit
    int* Orbit_Points = (int*)malloc(Orbit_count * sizeof(int));

    printf("ILP:\n");
    for(int i=0; i<Orbit_count; i++)
    {
	// clear num incident points in each orbit
	for(int j=0; j<Orbit_count; j++) Orbit_Points[j] = 0;

	// find orbit representative
	int j;
	for(j=0; j<Num_Points; j++)
	    if(Orbits[j] == i) break;
	unsigned long alpha = Points[j];

	for(int j=0; j<Num_Points; j++)
	{
	    unsigned long point = Points[j];
	    unsigned long t = trace_M_L(field_multiplication(alpha, point));
	    if(t == 0)
		Orbit_Points[Orbits[j]]++;
	}

	for(int j=0; j<Orbit_count; j++)
	    printf("%d ", Orbit_Points[j]);
	printf("\n");
    }
    free(Orbit_Points);
}
