/*******************************************************************
 *							MaxGCD								   *
 * Simple object used to find GCD between two numbers. Created for *
 * practice building max externals with inlets and outlets.		   *
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ext.h"
#include "ext_obex.h"
#include "ext_strings.h"

static t_class *t_gcd_class = NULL;

typedef struct _gcd {
	t_object g_obj;
	long storedVal;
	void *m_outlet1;
} t_GCD;
	

void *GCD_new();
void GCD_free(t_GCD *x);
void GCD_in1(t_GCD *x, long v);
void GCD_int(t_GCD *x, long i);
int GCD_findGCD(long *x, long *y);
void GCD_bang(t_GCD *x);
void GCD_assist(t_GCD *x, void *b, long m, long a, char *s);
void stdinletinfo();

int main () {
	t_class *c = NULL;
	
	c = class_new("MaxGCD", (method)GCD_new, (method)GCD_free, sizeof(t_GCD), 0L, 0);

	class_addmethod(c, (method)stdinletinfo, "inletinfo", A_CANT, 1); 
	class_addmethod(c, (method)GCD_in1, "in1", A_LONG, 0);
	class_addmethod(c, (method)GCD_int, "int", A_LONG, 0);
	class_addmethod(c, (method)GCD_bang, "bang", 0);
	class_addmethod(c, (method)GCD_assist,"assist", A_CANT, 0);
	
	class_register(CLASS_BOX, c);
	
	t_gcd_class = c;
	
	return EXIT_SUCCESS;
}

void *GCD_new() {
	t_GCD *x = (t_GCD *)object_alloc(t_gcd_class);
	x->storedVal = 1;
	intin(x, 1);
	x->m_outlet1 = intout((t_object *)x);
	return x;
}

void GCD_free(t_GCD *x) {
	if (x->storedVal)
		sysmem_freeptr(&x->storedVal);
}

void GCD_in1(t_GCD *x, long v) {
	if (v == 0) 
		object_post((t_object *)x,"Value cannot be zero");
	else 
		x->storedVal = v;
}

void GCD_int(t_GCD *x, long i) {
	if (i == 0)
		object_post((t_object *)x,"Value cannot be zero");
	else 
		outlet_int(x->m_outlet1, GCD_findGCD(&i, &x->storedVal));
}

void GCD_bang(t_GCD *x) {
	object_post((t_object *)x, "StoredVal = %ld", x->storedVal);
}

void GCD_assist(t_GCD *x, void *b, long m, long a, char *s) {
	if (m == ASSIST_OUTLET)
		strncpy_zero(s, "GCD of both values", 512);
	else {
		switch (a) {
			case 0:
				strncpy_zero(s, "Calculate GCD between this and stored value", 512);
				break;
			case 1:
				strncpy_zero(s, "Value to be stored and calculated against", 512);
				break;
		}
	}
}


int GCD_findGCD(long *x, long *y) {
	int temp = 1, bigger, smaller;
	if (*x > *y) {
		bigger = *x;
		smaller = *y;
	}
	else {
		bigger = *y;
		smaller = *x;
	}

	while (temp != 0) {
		if (bigger % smaller == 0) {
			temp = smaller;
			break;
		}
		temp = bigger % smaller;
		bigger = smaller;
		smaller = temp;
	}
	return temp;
}

