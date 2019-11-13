/*
 *  Triangle-Triangle Overlap Test Routines
 *  July, 2002
 *  Updated December 2003
 *
 *  This file contains C implementation of algorithms for
 *  performing two and three-dimensional triangle-triangle intersection test
 *  The algorithms and underlying theory are described in
 *
 * "Fast and Robust Triangle-Triangle Overlap Test
 *  Using Orientation Predicates"  P. Guigue - O. Devillers
 *
 *  Journal of Graphics Tools, 8(1), 2003
 *
 *  Several geometric predicates are defined.  Their parameters are all
 *  points.  Each point is an array of two or three real precision
 *  floating point numbers. The geometric predicates implemented in
 *  this file are:
 *
 *    int tri_tri_overlap_test_3d(p1,q1,r1,p2,q2,r2)
 *    int tri_tri_overlap_test_2d(p1,q1,r1,p2,q2,r2)
 *
 *    int tri_tri_intersection_test_3d(p1,q1,r1,p2,q2,r2,
 *                                     coplanar,source,target)
 *
 *       is a version that computes the segment of intersection when
 *       the triangles overlap (and are not coplanar)
 *
 *    each function returns 1 if the triangles (including their
 *    boundary) intersect, otherwise 0
 *
 *
 *  Other information are available from the Web page
 *  http:<i>//www.acm.org/jgt/papers/GuigueDevillers03/
 *
 */

 // modified by Aaron to better detect coplanarity
/*
typedef float real;

#define ZERO_TEST(x)  (x == 0)
//#define ZERO_TEST(x)  ((x) > -0.001 && (x) < .001)

#include "stdio.h"
#include "IntersectionTests.h"


/* coplanar returns whether the triangles are coplanar
 *  source and target are the endpoints of the segment of
 *  intersection if it exists)
 */


 /* some 3D macros */
/*
#define CROSS(dest,v1,v2)                       \
dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) dest[0]=v1[0]-v2[0]; \
dest[1]=v1[1]-v2[1]; \
dest[2]=v1[2]-v2[2]; 

#define SCALAR(dest,alpha,v) dest[0] = alpha * v[0]; \
dest[1] = alpha * v[1]; \
dest[2] = alpha * v[2];

#define CHECK_MIN_MAX(p1,q1,r1,p2,q2,r2) {\
SUB(v1,p2,q1)\
SUB(v2,p1,q1)\
CROSS(N1,v1,v2)\
SUB(v1,q2,q1)\
if (DOT(v1,N1) > 0.0f) return 0;\
SUB(v1,p2,p1)\
SUB(v2,r1,p1)\
CROSS(N1,v1,v2)\
SUB(v1,r2,p1) \
if (DOT(v1,N1) > 0.0f) return 0;\
else return 1; }



/* Permutation in a canonical form of T2's vertices */
/*
#define TRI_TRI_3D(p1,q1,r1,p2,q2,r2,dp2,dq2,dr2) { \
if (dp2 > 0.0f) { \
if (dq2 > 0.0f) CHECK_MIN_MAX(p1,r1,q1,r2,p2,q2) \
else if (dr2 > 0.0f) CHECK_MIN_MAX(p1,r1,q1,q2,r2,p2)\
else CHECK_MIN_MAX(p1,q1,r1,p2,q2,r2) }\
else if (dp2 < 0.0f) { \
if (dq2 < 0.0f) CHECK_MIN_MAX(p1,q1,r1,r2,p2,q2)\
else if (dr2 < 0.0f) CHECK_MIN_MAX(p1,q1,r1,q2,r2,p2)\
else CHECK_MIN_MAX(p1,r1,q1,p2,q2,r2)\
} else { \
if (dq2 < 0.0f) { \
if (dr2 >= 0.0f)  CHECK_MIN_MAX(p1,r1,q1,q2,r2,p2)\
else CHECK_MIN_MAX(p1,q1,r1,p2,q2,r2)\
} \
else if (dq2 > 0.0f) { \
if (dr2 > 0.0f) CHECK_MIN_MAX(p1,r1,q1,p2,q2,r2)\
else  CHECK_MIN_MAX(p1,q1,r1,q2,r2,p2)\
} \
else  { \
if (dr2 > 0.0f) CHECK_MIN_MAX(p1,q1,r1,r2,p2,q2)\
else if (dr2 < 0.0f) CHECK_MIN_MAX(p1,r1,q1,r2,p2,q2)\
else return coplanar_tri_tri3d(p1,q1,r1,p2,q2,r2,N1,N2);\
}}}



/*
 *
 *  Three-dimensional Triangle-Triangle Overlap Test
 *
 */

/*
int IntersectionTests::tri_tri_overlap_test_3d(real p1[3], real q1[3], real r1[3],

	real p2[3], real q2[3], real r2[3])
{
	real dp1, dq1, dr1, dp2, dq2, dr2;
	real v1[3], v2[3];
	real N1[3], N2[3];

	/* Compute distance signs  of p1, q1 and r1 to the plane of
	 triangle(p2,q2,r2) */

/*
	SUB(v1, p2, r2)
		SUB(v2, q2, r2)
		CROSS(N2, v1, v2)

		SUB(v1, p1, r2)
		dp1 = DOT(v1, N2);
	SUB(v1, q1, r2)
		dq1 = DOT(v1, N2);
	SUB(v1, r1, r2)
		dr1 = DOT(v1, N2);

	if (((dp1 * dq1) > 0.0f) && ((dp1 * dr1) > 0.0f))  return 0;

	/* Compute distance signs  of p2, q2 and r2 to the plane of
	 triangle(p1,q1,r1) */
/*

	SUB(v1, q1, p1)
		SUB(v2, r1, p1)
		CROSS(N1, v1, v2)

		SUB(v1, p2, r1)
		dp2 = DOT(v1, N1);
	SUB(v1, q2, r1)
		dq2 = DOT(v1, N1);
	SUB(v1, r2, r1)
		dr2 = DOT(v1, N1);

	if (((dp2 * dq2) > 0.0f) && ((dp2 * dr2) > 0.0f)) return 0;

	/* Permutation in a canonical form of T1's vertices */



/*
	if (dp1 > 0.0f) {
		if (dq1 > 0.0f) TRI_TRI_3D(r1, p1, q1, p2, r2, q2, dp2, dr2, dq2)
		else if (dr1 > 0.0f) TRI_TRI_3D(q1, r1, p1, p2, r2, q2, dp2, dr2, dq2)
		else TRI_TRI_3D(p1, q1, r1, p2, q2, r2, dp2, dq2, dr2)
	}
	else if (dp1 < 0.0f) {
		if (dq1 < 0.0f) TRI_TRI_3D(r1, p1, q1, p2, q2, r2, dp2, dq2, dr2)
		else if (dr1 < 0.0f) TRI_TRI_3D(q1, r1, p1, p2, q2, r2, dp2, dq2, dr2)
		else TRI_TRI_3D(p1, q1, r1, p2, r2, q2, dp2, dr2, dq2)
	}
	else {
		if (dq1 < 0.0f) {
			if (dr1 >= 0.0f) TRI_TRI_3D(q1, r1, p1, p2, r2, q2, dp2, dr2, dq2)
			else TRI_TRI_3D(p1, q1, r1, p2, q2, r2, dp2, dq2, dr2)
		}
		else if (dq1 > 0.0f) {
			if (dr1 > 0.0f) TRI_TRI_3D(p1, q1, r1, p2, r2, q2, dp2, dr2, dq2)
			else TRI_TRI_3D(q1, r1, p1, p2, q2, r2, dp2, dq2, dr2)
		}
		else {
			if (dr1 > 0.0f) TRI_TRI_3D(r1, p1, q1, p2, q2, r2, dp2, dq2, dr2)
			else if (dr1 < 0.0f) TRI_TRI_3D(r1, p1, q1, p2, r2, q2, dp2, dr2, dq2)
			else return coplanar_tri_tri3d(p1, q1, r1, p2, q2, r2, N1, N2);
		}
	}
};



int IntersectionTests::coplanar_tri_tri3d(real p1[3], real q1[3], real r1[3],
	real p2[3], real q2[3], real r2[3],
	real normal_1[3], real normal_2[3]) {

	real P1[2], Q1[2], R1[2];
	real P2[2], Q2[2], R2[2];

	real n_x, n_y, n_z;

	n_x = ((normal_1[0] < 0) ? -normal_1[0] : normal_1[0]);
	n_y = ((normal_1[1] < 0) ? -normal_1[1] : normal_1[1]);
	n_z = ((normal_1[2] < 0) ? -normal_1[2] : normal_1[2]);


	/* Projection of the triangles in 3D onto 2D such that the area of
	 the projection is maximized. */
/*

	if ((n_x > n_z) && (n_x >= n_y)) {
		// Project onto plane YZ

		P1[0] = q1[2]; P1[1] = q1[1];
		Q1[0] = p1[2]; Q1[1] = p1[1];
		R1[0] = r1[2]; R1[1] = r1[1];

		P2[0] = q2[2]; P2[1] = q2[1];
		Q2[0] = p2[2]; Q2[1] = p2[1];
		R2[0] = r2[2]; R2[1] = r2[1];

	}
	else if ((n_y > n_z) && (n_y >= n_x)) {
		// Project onto plane XZ

		P1[0] = q1[0]; P1[1] = q1[2];
		Q1[0] = p1[0]; Q1[1] = p1[2];
		R1[0] = r1[0]; R1[1] = r1[2];

		P2[0] = q2[0]; P2[1] = q2[2];
		Q2[0] = p2[0]; Q2[1] = p2[2];
		R2[0] = r2[0]; R2[1] = r2[2];

	}
	else {
		// Project onto plane XY

		P1[0] = p1[0]; P1[1] = p1[1];
		Q1[0] = q1[0]; Q1[1] = q1[1];
		R1[0] = r1[0]; R1[1] = r1[1];

		P2[0] = p2[0]; P2[1] = p2[1];
		Q2[0] = q2[0]; Q2[1] = q2[1];
		R2[0] = r2[0]; R2[1] = r2[1];
	}

	return tri_tri_overlap_test_2d(P1, Q1, R1, P2, Q2, R2);

};



/*
 *
 *  Three-dimensional Triangle-Triangle Intersection
 *
 */

 /*
  This macro is called when the triangles surely intersect
  It constructs the segment of intersection of the two triangles
  if they are not coplanar.
  */
/*
#define CONSTRUCT_INTERSECTION(p1,q1,r1,p2,q2,r2) { \
SUB(v1,q1,p1) \
SUB(v2,r2,p1) \
CROSS(N,v1,v2) \
SUB(v,p2,p1) \
if (DOT(v,N) > 0.0f) {\
SUB(v1,r1,p1) \
CROSS(N,v1,v2) \
if (DOT(v,N) <= 0.0f) { \
SUB(v2,q2,p1) \
CROSS(N,v1,v2) \
if (DOT(v,N) > 0.0f) { \
SUB(v1,p1,p2) \
SUB(v2,p1,r1) \
alpha = DOT(v1,N2) / DOT(v2,N2); \
SCALAR(v1,alpha,v2) \
SUB(source,p1,v1) \
SUB(v1,p2,p1) \
SUB(v2,p2,r2) \
alpha = DOT(v1,N1) / DOT(v2,N1); \
SCALAR(v1,alpha,v2) \
SUB(target,p2,v1) \
return 1; \
} else { \
SUB(v1,p2,p1) \
SUB(v2,p2,q2) \
alpha = DOT(v1,N1) / DOT(v2,N1); \
SCALAR(v1,alpha,v2) \
SUB(source,p2,v1) \
SUB(v1,p2,p1) \
SUB(v2,p2,r2) \
alpha = DOT(v1,N1) / DOT(v2,N1); \
SCALAR(v1,alpha,v2) \
SUB(target,p2,v1) \
return 1; \
} \
} else { \
return 0; \
} \
} else { \
SUB(v2,q2,p1) \
CROSS(N,v1,v2) \
if (DOT(v,N) < 0.0f) { \
return 0; \
} else { \
SUB(v1,r1,p1) \
CROSS(N,v1,v2) \
if (DOT(v,N) >= 0.0f) { \
SUB(v1,p1,p2) \
SUB(v2,p1,r1) \
alpha = DOT(v1,N2) / DOT(v2,N2); \
SCALAR(v1,alpha,v2) \
SUB(source,p1,v1) \
SUB(v1,p1,p2) \
SUB(v2,p1,q1) \
alpha = DOT(v1,N2) / DOT(v2,N2); \
SCALAR(v1,alpha,v2) \
SUB(target,p1,v1) \
return 1; \
} else { \
SUB(v1,p2,p1) \
SUB(v2,p2,q2) \
alpha = DOT(v1,N1) / DOT(v2,N1); \
SCALAR(v1,alpha,v2) \
SUB(source,p2,v1) \
SUB(v1,p1,p2) \
SUB(v2,p1,q1) \
alpha = DOT(v1,N2) / DOT(v2,N2); \
SCALAR(v1,alpha,v2) \
SUB(target,p1,v1) \
return 1; \
}}}} 



#define TRI_TRI_INTER_3D(p1,q1,r1,p2,q2,r2,dp2,dq2,dr2) { \
if (dp2 > 0.0f) { \
if (dq2 > 0.0f) CONSTRUCT_INTERSECTION(p1,r1,q1,r2,p2,q2) \
else if (dr2 > 0.0f) CONSTRUCT_INTERSECTION(p1,r1,q1,q2,r2,p2)\
else CONSTRUCT_INTERSECTION(p1,q1,r1,p2,q2,r2) }\
else if (dp2 < 0.0f) { \
if (dq2 < 0.0f) CONSTRUCT_INTERSECTION(p1,q1,r1,r2,p2,q2)\
else if (dr2 < 0.0f) CONSTRUCT_INTERSECTION(p1,q1,r1,q2,r2,p2)\
else CONSTRUCT_INTERSECTION(p1,r1,q1,p2,q2,r2)\
} else { \
if (dq2 < 0.0f) { \
if (dr2 >= 0.0f)  CONSTRUCT_INTERSECTION(p1,r1,q1,q2,r2,p2)\
else CONSTRUCT_INTERSECTION(p1,q1,r1,p2,q2,r2)\
} \
else if (dq2 > 0.0f) { \
if (dr2 > 0.0f) CONSTRUCT_INTERSECTION(p1,r1,q1,p2,q2,r2)\
else  CONSTRUCT_INTERSECTION(p1,q1,r1,q2,r2,p2)\
} \
else  { \
if (dr2 > 0.0f) CONSTRUCT_INTERSECTION(p1,q1,r1,r2,p2,q2)\
else if (dr2 < 0.0f) CONSTRUCT_INTERSECTION(p1,r1,q1,r2,p2,q2)\
else { \
*coplanar = 1; \
return coplanar_tri_tri3d(p1,q1,r1,p2,q2,r2,N1,N2);\
} \
}} }


  /*
   The following version computes the segment of intersection of the
   two triangles if it exists.
   coplanar returns whether the triangles are coplanar
   source and target are the endpoints of the line segment of intersection
   */
/*
int IntersectionTests::tri_tri_intersection_test_3d(real p1[3], real q1[3], real r1[3],
	real p2[3], real q2[3], real r2[3],
	int * coplanar,
	real source[3], real target[3])

{
	real dp1, dq1, dr1, dp2, dq2, dr2;
	real v1[3], v2[3], v[3];
	real N1[3], N2[3], N[3];
	real alpha;

	// Compute distance signs  of p1, q1 and r1 
	// to the plane of triangle(p2,q2,r2)


	SUB(v1, p2, r2)
		SUB(v2, q2, r2)
		CROSS(N2, v1, v2)

		SUB(v1, p1, r2)
		dp1 = DOT(v1, N2);
	SUB(v1, q1, r2)
		dq1 = DOT(v1, N2);
	SUB(v1, r1, r2)
		dr1 = DOT(v1, N2);

	if (((dp1 * dq1) > 0.0f) && ((dp1 * dr1) > 0.0f))  return 0;

	// Compute distance signs  of p2, q2 and r2 
	// to the plane of triangle(p1,q1,r1)


	SUB(v1, q1, p1)
		SUB(v2, r1, p1)
		CROSS(N1, v1, v2)

		SUB(v1, p2, r1)
		dp2 = DOT(v1, N1);
	SUB(v1, q2, r1)
		dq2 = DOT(v1, N1);
	SUB(v1, r2, r1)
		dr2 = DOT(v1, N1);

	if (((dp2 * dq2) > 0.0f) && ((dp2 * dr2) > 0.0f)) return 0;

	// Permutation in a canonical form of T1's vertices


	//  printf("d1 = [%f %f %f], d2 = [%f %f %f]\n", dp1, dq1, dr1, dp2, dq2, dr2);
	/*
	 // added by Aaron
	 if (ZERO_TEST(dp1) || ZERO_TEST(dq1) ||ZERO_TEST(dr1) ||ZERO_TEST(dp2) ||ZERO_TEST(dq2) ||ZERO_TEST(dr2))
	 {
	 coplanar = 1;
	 return 0;
	 }
	 */

/*
	if (dp1 > 0.0f) {
		if (dq1 > 0.0f) TRI_TRI_INTER_3D(r1, p1, q1, p2, r2, q2, dp2, dr2, dq2)
		else if (dr1 > 0.0f) TRI_TRI_INTER_3D(q1, r1, p1, p2, r2, q2, dp2, dr2, dq2)

		else TRI_TRI_INTER_3D(p1, q1, r1, p2, q2, r2, dp2, dq2, dr2)
	}
	else if (dp1 < 0.0f) {
		if (dq1 < 0.0f) TRI_TRI_INTER_3D(r1, p1, q1, p2, q2, r2, dp2, dq2, dr2)
		else if (dr1 < 0.0f) TRI_TRI_INTER_3D(q1, r1, p1, p2, q2, r2, dp2, dq2, dr2)
		else TRI_TRI_INTER_3D(p1, q1, r1, p2, r2, q2, dp2, dr2, dq2)
	}
	else {
		if (dq1 < 0.0f) {
			if (dr1 >= 0.0f) TRI_TRI_INTER_3D(q1, r1, p1, p2, r2, q2, dp2, dr2, dq2)
			else TRI_TRI_INTER_3D(p1, q1, r1, p2, q2, r2, dp2, dq2, dr2)
		}
		else if (dq1 > 0.0f) {
			if (dr1 > 0.0f) TRI_TRI_INTER_3D(p1, q1, r1, p2, r2, q2, dp2, dr2, dq2)
			else TRI_TRI_INTER_3D(q1, r1, p1, p2, q2, r2, dp2, dq2, dr2)
		}
		else {
			if (dr1 > 0.0f) TRI_TRI_INTER_3D(r1, p1, q1, p2, q2, r2, dp2, dq2, dr2)
			else if (dr1 < 0.0f) TRI_TRI_INTER_3D(r1, p1, q1, p2, r2, q2, dp2, dr2, dq2)
			else {
				// triangles are co-planar

				*coplanar = 1;
				return coplanar_tri_tri3d(p1, q1, r1, p2, q2, r2, N1, N2);
			}
		}
	}
};













/*
 *
 *  Two dimensional Triangle-Triangle Overlap Test
 *
 */


 /* some 2D macros */
/*
#define ORIENT_2D(a, b, c)  ((a[0]-c[0])*(b[1]-c[1])-(a[1]-c[1])*(b[0]-c[0]))


#define INTERSECTION_TEST_VERTEXA(P1, Q1, R1, P2, Q2, R2) {\
if (ORIENT_2D(R2,P2,Q1) >= 0.0f)\
if (ORIENT_2D(R2,Q2,Q1) <= 0.0f)\
if (ORIENT_2D(P1,P2,Q1) > 0.0f) {\
if (ORIENT_2D(P1,Q2,Q1) <= 0.0f) return 1; \
else return 0;} else {\
if (ORIENT_2D(P1,P2,R1) >= 0.0f)\
if (ORIENT_2D(Q1,R1,P2) >= 0.0f) return 1; \
else return 0;\
else return 0;}\
else \
if (ORIENT_2D(P1,Q2,Q1) <= 0.0f)\
if (ORIENT_2D(R2,Q2,R1) <= 0.0f)\
if (ORIENT_2D(Q1,R1,Q2) >= 0.0f) return 1; \
else return 0;\
else return 0;\
else return 0;\
else\
if (ORIENT_2D(R2,P2,R1) >= 0.0f) \
if (ORIENT_2D(Q1,R1,R2) >= 0.0f)\
if (ORIENT_2D(P1,P2,R1) >= 0.0f) return 1;\
else return 0;\
else \
if (ORIENT_2D(Q1,R1,Q2) >= 0.0f) {\
if (ORIENT_2D(R2,R1,Q2) >= 0.0f) return 1; \
else return 0; }\
else return 0; \
else  return 0; \
};

#define INTERSECTION_TEST_VERTEX(P1, Q1, R1, P2, Q2, R2) {\
  if (ORIENT_2D(R2,P2,Q1) >= 0.0f)\
    if (ORIENT_2D(R2,Q2,Q1) <= 0.0f)\
      if (ORIENT_2D(P1,P2,Q1) > 0.0f) {\
        if (ORIENT_2D(P1,Q2,Q1) <= 0.0f) return 1; \
        else return 0;} else {\
        if (ORIENT_2D(P1,P2,R1) >= 0.0f)\
          if (ORIENT_2D(Q1,R1,P2) >= 0.0f) return 1; \
          else return 0;\
        else return 0;}\
    else \
      if (ORIENT_2D(P1,Q2,Q1) <= 0.0f)\
        if (ORIENT_2D(R2,Q2,R1) <= 0.0f)\
          if (ORIENT_2D(Q1,R1,Q2) >= 0.0f) return 1; \
          else return 0;\
        else return 0;\
      else return 0;\
  else\
    if (ORIENT_2D(R2,P2,R1) >= 0.0f) \
      if (ORIENT_2D(Q1,R1,R2) >= 0.0f)\
        if (ORIENT_2D(P1,P2,R1) >= 0.0f) return 1;\
        else return 0;\
      else \
        if (ORIENT_2D(Q1,R1,Q2) >= 0.0f) {\
          if (ORIENT_2D(R2,R1,Q2) >= 0.0f) return 1; \
          else return 0; }\
        else return 0; \
    else  return 0; \
 };


#define INTERSECTION_TEST_EDGE(P1, Q1, R1, P2, Q2, R2) { \
if (ORIENT_2D(R2,P2,Q1) >= 0.0f) {\
if (ORIENT_2D(P1,P2,Q1) >= 0.0f) { \
if (ORIENT_2D(P1,Q1,R2) >= 0.0f) return 1; \
else return 0;} else { \
if (ORIENT_2D(Q1,R1,P2) >= 0.0f){ \
if (ORIENT_2D(R1,P1,P2) >= 0.0f) return 1; else return 0;} \
else return 0; } \
} else {\
if (ORIENT_2D(R2,P2,R1) >= 0.0f) {\
if (ORIENT_2D(P1,P2,R1) >= 0.0f) {\
if (ORIENT_2D(P1,R1,R2) >= 0.0f) return 1;  \
else {\
if (ORIENT_2D(Q1,R1,R2) >= 0.0f) return 1; else return 0;}}\
else  return 0; }\
else return 0; }}



int ccw_tri_tri_intersection_2d(real p1[2], real q1[2], real r1[2],
	real p2[2], real q2[2], real r2[2]) {
	if (ORIENT_2D(p2, q2, p1) >= 0.0f) {
		if (ORIENT_2D(q2, r2, p1) >= 0.0f) {
			if (ORIENT_2D(r2, p2, p1) >= 0.0f) return 1;
			else INTERSECTION_TEST_EDGE(p1, q1, r1, p2, q2, r2)
		}
		else {
			if (ORIENT_2D(r2, p2, p1) >= 0.0f)
				INTERSECTION_TEST_EDGE(p1, q1, r1, r2, p2, q2)
			else INTERSECTION_TEST_VERTEX(p1, q1, r1, p2, q2, r2)
		}
	}
	else {
		if (ORIENT_2D(q2, r2, p1) >= 0.0f) {
			if (ORIENT_2D(r2, p2, p1) >= 0.0f)
				INTERSECTION_TEST_EDGE(p1, q1, r1, q2, r2, p2)
			else  INTERSECTION_TEST_VERTEX(p1, q1, r1, q2, r2, p2)
		}
		else INTERSECTION_TEST_VERTEX(p1, q1, r1, r2, p2, q2)
	}
};


int IntersectionTests::tri_tri_overlap_test_2d(real p1[2], real q1[2], real r1[2],
	real p2[2], real q2[2], real r2[2]) {
	if (ORIENT_2D(p1, q1, r1) < 0.0f)
		if (ORIENT_2D(p2, q2, r2) < 0.0f)
			return ccw_tri_tri_intersection_2d(p1, r1, q1, p2, r2, q2);
		else
			return ccw_tri_tri_intersection_2d(p1, r1, q1, p2, q2, r2);
	else
		if (ORIENT_2D(p2, q2, r2) < 0.0f)
			return ccw_tri_tri_intersection_2d(p1, q1, r1, p2, r2, q2);
		else
			return ccw_tri_tri_intersection_2d(p1, q1, r1, p2, q2, r2);

};*/






















/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(double boxcenter[3],      */
/*          double boxhalfsize[3],double triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/
#include <math.h>
#include "IntersectionTests.h"

#define X 0
#define Y 1
#define Z 2

#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0]; 

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;

int IntersectionTests::planeBoxOverlap(double normal[3], double d, double maxbox[3])
{
	int q;
	double vmin[3], vmax[3];
	for (q = X; q <= Z; q++)
	{
		if (normal[q] > 0.0f)
		{
			vmin[q] = -maxbox[q];
			vmax[q] = maxbox[q];
		}
		else
		{
			vmin[q] = maxbox[q];
			vmax[q] = -maxbox[q];
		}
	}
	if (DOT(normal, vmin) + d > 0.0f) return 0;
	if (DOT(normal, vmax) + d >= 0.0f) return 1;

	return 0;
}

int IntersectionTests::planeBoxOverlap(float normal[3], float d, float maxbox[3])
{
	int q;
	float vmin[3], vmax[3];
	for (q = X; q <= Z; q++)
	{
		if (normal[q] > 0.0f)
		{
			vmin[q] = -maxbox[q];
			vmax[q] = maxbox[q];
		}
		else
		{
			vmin[q] = maxbox[q];
			vmax[q] = -maxbox[q];
		}
	}
	if (DOT(normal, vmin) + d > 0.0f) return 0;
	if (DOT(normal, vmax) + d >= 0.0f) return 1;

	return 0;
}
/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			       	   \
	p2 = a*v2[Y] - b*v2[Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			           \
	p1 = a*v1[Y] - b*v1[Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p2 = -a*v2[X] + b*v2[Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p1 = -a*v1[X] + b*v1[Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[X] - b*v1[Y];			           \
	p2 = a*v2[X] - b*v2[Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[X] - b*v0[Y];				   \
	p1 = a*v1[X] - b*v1[Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;

int IntersectionTests::triBoxOverlap(double boxcenter[3], double boxhalfsize[3], double triverts[3][3])
{

	/*    use separating axis theorem to test overlap between triangle and box */
	/*    need to test for overlap in these directions: */
	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
	/*       we do not even need to test these) */
	/*    2) normal of the triangle */
	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
	/*       this gives 3x3=9 more tests */
	double v0[3], v1[3], v2[3];
	// double axis[3];
	double min, max, d, p0, p1, p2, rad, fex, fey, fez;
	double normal[3], e0[3], e1[3], e2[3];

	/* This is the fastest branch on Sun */
	/* move everything so that the boxcenter is in (0,0,0) */
	SUB(v0, triverts[0], boxcenter);
	SUB(v1, triverts[1], boxcenter);
	SUB(v2, triverts[2], boxcenter);

	/* compute triangle edges */
	SUB(e0, v1, v0);      /* tri edge 0 */
	SUB(e1, v2, v1);      /* tri edge 1 */
	SUB(e2, v0, v2);      /* tri edge 2 */

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */
	fex = fabs(e0[X]);
	fey = fabs(e0[Y]);
	fez = fabs(e0[Z]);
	AXISTEST_X01(e0[Z], e0[Y], fez, fey);
	AXISTEST_Y02(e0[Z], e0[X], fez, fex);
	AXISTEST_Z12(e0[Y], e0[X], fey, fex);

	fex = fabs(e1[X]);
	fey = fabs(e1[Y]);
	fez = fabs(e1[Z]);
	AXISTEST_X01(e1[Z], e1[Y], fez, fey);
	AXISTEST_Y02(e1[Z], e1[X], fez, fex);
	AXISTEST_Z0(e1[Y], e1[X], fey, fex);

	fex = fabs(e2[X]);
	fey = fabs(e2[Y]);
	fez = fabs(e2[Z]);
	AXISTEST_X2(e2[Z], e2[Y], fez, fey);
	AXISTEST_Y1(e2[Z], e2[X], fez, fex);
	AXISTEST_Z12(e2[Y], e2[X], fey, fex);

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	FINDMINMAX(v0[X], v1[X], v2[X], min, max);
	if (min > boxhalfsize[X] || max < -boxhalfsize[X]) return 0;

	/* test in Y-direction */
	FINDMINMAX(v0[Y], v1[Y], v2[Y], min, max);
	if (min > boxhalfsize[Y] || max < -boxhalfsize[Y]) return 0;

	/* test in Z-direction */
	FINDMINMAX(v0[Z], v1[Z], v2[Z], min, max);
	if (min > boxhalfsize[Z] || max < -boxhalfsize[Z]) return 0;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	CROSS(normal, e0, e1);
	d = -DOT(normal, v0);  /* plane eq: normal.x+d=0 */
	if (!planeBoxOverlap(normal, d, boxhalfsize)) return 0;

	return 1;   /* box and triangle overlaps */
}

int IntersectionTests::triBoxOverlap(float boxhalfsize[3], float v0[3], float v1[3], float v2[3])
{

	/*    use separating axis theorem to test overlap between triangle and box */
	/*    need to test for overlap in these directions: */
	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
	/*       we do not even need to test these) */
	/*    2) normal of the triangle */
	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
	/*       this gives 3x3=9 more tests */
	 //double v0[3],v1[3],v2[3];
	// double axis[3];
	float min, max, d, p0, p1, p2, rad, fex, fey, fez;
	float normal[3], e0[3], e1[3], e2[3];

	/* This is the fastest branch on Sun */
	/* move everything so that the boxcenter is in (0,0,0) */
   // SUB(v0,trivertsA,boxcenter);
   // SUB(v1,trivertsB,boxcenter);
   // SUB(v2,trivertsC,boxcenter);

	/* compute triangle edges */
	//e0[0] = v1[0] - v0[0]; e0[1] = v1[1] - v0[1]; e0[2] = v1[2] - v0[2];
	//e1[0] = v2[0] - v1[0]; e1[1] = v2[1] - v1[1]; e1[2] = v2[2] - v1[2];
	//e2[0] = v0[0] - v2[0]; e2[1] = v0[1] - v2[1]; e2[2] = v0[2] - v2[2];
	SUB(e0, v1, v0);      /* tri edge 0 */
	SUB(e1, v2, v1);      /* tri edge 1 */
	SUB(e2, v0, v2);      /* tri edge 2 */

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */
	fex = e0[X];//fabs(e0[X]);
	fey = e0[Y];//fabs(e0[Y]);
	fez = e0[Z];//fabs(e0[Z]);
	if (e0[X] < 0)
		fex = -e0[X];
	if (e0[Y] < 0)
		fey = -e0[Y];
	if (e0[Z] < 0)
		fez = -e0[Z];
	AXISTEST_X01(e0[Z], e0[Y], fez, fey);
	AXISTEST_Y02(e0[Z], e0[X], fez, fex);
	AXISTEST_Z12(e0[Y], e0[X], fey, fex);

	fex = e1[X];//fabs(e1[X]);
	fey = e1[Y];//fabs(e1[Y]);
	fez = e1[Z];//fabs(e1[Z]);
	if (e1[X] < 0)
		fex = -e1[X];
	if (e1[Y] < 0)
		fey = -e1[Y];
	if (e1[Z] < 0)
		fez = -e1[Z];
	AXISTEST_X01(e1[Z], e1[Y], fez, fey);
	AXISTEST_Y02(e1[Z], e1[X], fez, fex);
	AXISTEST_Z0(e1[Y], e1[X], fey, fex);

	fex = e2[X];//fabs(e2[X]);
	fey = e2[Y];//fabs(e2[Y]);
	fez = e2[Z];//fabs(e2[Z]);
	if (e2[X] < 0)
		fex = -e2[X];
	if (e2[Y] < 0)
		fey = -e2[Y];
	if (e2[Z] < 0)
		fez = -e2[Z];
	AXISTEST_X2(e2[Z], e2[Y], fez, fey);
	AXISTEST_Y1(e2[Z], e2[X], fez, fex);
	AXISTEST_Z12(e2[Y], e2[X], fey, fex);

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	FINDMINMAX(v0[X], v1[X], v2[X], min, max);
	if (min > boxhalfsize[X] || max < -boxhalfsize[X]) return 0;

	/* test in Y-direction */
	FINDMINMAX(v0[Y], v1[Y], v2[Y], min, max);
	if (min > boxhalfsize[Y] || max < -boxhalfsize[Y]) return 0;

	/* test in Z-direction */
	FINDMINMAX(v0[Z], v1[Z], v2[Z], min, max);
	if (min > boxhalfsize[Z] || max < -boxhalfsize[Z]) return 0;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	CROSS(normal, e0, e1);
	d = -DOT(normal, v0);  /* plane eq: normal.x+d=0 */

	if (!planeBoxOverlap(normal, d, boxhalfsize)) return 0;

	return 1;   /* box and triangle overlaps */
}















/* Triangle/triangle intersection test routine,
 * by Tomas Moller, 1997.
 * See article "A Fast Triangle-Triangle Intersection Test",
 * Journal of Graphics Tools, 2(2), 1997
 *
 * Updated June 1999: removed the divisions -- a little faster now!
 * Updated October 1999: added {} to CROSS and SUB macros
 *
 * int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3],
 *                      float U0[3],float U1[3],float U2[3])
 *
 * parameters: vertices of triangle 1: V0,V1,V2
 *             vertices of triangle 2: U0,U1,U2
 * result    : returns 1 if the triangles intersect, otherwise 0
 *
 */

#include <math.h>
#define FABS(x) (float(fabs(x)))        /* implement as is fastest on your machine */

 /* if USE_EPSILON_TEST is true then we do a check:
		  if |dv|<EPSILON then dv=0.0;
	else no check is done (which is less robust)
 */
#define USE_EPSILON_TEST TRUE
#define EPSILON 0.000001


 /* some macros */
#define CROSS(dest,v1,v2){                     \
              dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
              dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
              dest[2]=v1[0]*v2[1]-v1[1]*v2[0];}

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2){         \
            dest[0]=v1[0]-v2[0]; \
            dest[1]=v1[1]-v2[1]; \
            dest[2]=v1[2]-v2[2];}

/* sort so that a<=b */
#define SORT(a,b)       \
             if(a>b)    \
             {          \
               float c; \
               c=a;     \
               a=b;     \
               b=c;     \
             }


/* this edge to edge test is based on Franlin Antonio's gem:
   "Faster Line Segment Intersection", in Graphics Gems III,
   pp. 199-202 */
#define EDGE_EDGE_TEST(V0,U0,U1)                      \
  Bx=U0[i0]-U1[i0];                                   \
  By=U0[i1]-U1[i1];                                   \
  Cx=V0[i0]-U0[i0];                                   \
  Cy=V0[i1]-U0[i1];                                   \
  f=Ay*Bx-Ax*By;                                      \
  d=By*Cx-Bx*Cy;                                      \
  if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
  {                                                   \
    e=Ax*Cy-Ay*Cx;                                    \
    if(f>0)                                           \
    {                                                 \
      if(e>=0 && e<=f) return 1;                      \
    }                                                 \
    else                                              \
    {                                                 \
      if(e<=0 && e>=f) return 1;                      \
    }                                                 \
  }

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
{                                              \
  float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
  Ax=V1[i0]-V0[i0];                            \
  Ay=V1[i1]-V0[i1];                            \
  /* test edge U0,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U0,U1);                    \
  /* test edge U1,U2 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U1,U2);                    \
  /* test edge U2,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U2,U0);                    \
}

#define POINT_IN_TRI(V0,U0,U1,U2)           \
{                                           \
  float a,b,c,d0,d1,d2;                     \
  /* is T1 completly inside T2? */          \
  /* check if V0 is inside tri(U0,U1,U2) */ \
  a=U1[i1]-U0[i1];                          \
  b=-(U1[i0]-U0[i0]);                       \
  c=-a*U0[i0]-b*U0[i1];                     \
  d0=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U2[i1]-U1[i1];                          \
  b=-(U2[i0]-U1[i0]);                       \
  c=-a*U1[i0]-b*U1[i1];                     \
  d1=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U0[i1]-U2[i1];                          \
  b=-(U0[i0]-U2[i0]);                       \
  c=-a*U2[i0]-b*U2[i1];                     \
  d2=a*V0[i0]+b*V0[i1]+c;                   \
  if(d0*d1>0.0)                             \
  {                                         \
    if(d0*d2>0.0) return 1;                 \
  }                                         \
}

int coplanar_tri_tri(float N[3], float V0[3], float V1[3], float V2[3],
	float U0[3], float U1[3], float U2[3])
{
	float A[3];
	short i0, i1;
	/* first project onto an axis-aligned plane, that maximizes the area */
	/* of the triangles, compute indices: i0,i1. */
	A[0] = FABS(N[0]);
	A[1] = FABS(N[1]);
	A[2] = FABS(N[2]);
	if (A[0] > A[1])
	{
		if (A[0] > A[2])
		{
			i0 = 1;      /* A[0] is greatest */
			i1 = 2;
		}
		else
		{
			i0 = 0;      /* A[2] is greatest */
			i1 = 1;
		}
	}
	else   /* A[0]<=A[1] */
	{
		if (A[2] > A[1])
		{
			i0 = 0;      /* A[2] is greatest */
			i1 = 1;
		}
		else
		{
			i0 = 0;      /* A[1] is greatest */
			i1 = 2;
		}
	}

	/* test all edges of triangle 1 against the edges of triangle 2 */
	EDGE_AGAINST_TRI_EDGES(V0, V1, U0, U1, U2);
	EDGE_AGAINST_TRI_EDGES(V1, V2, U0, U1, U2);
	EDGE_AGAINST_TRI_EDGES(V2, V0, U0, U1, U2);

	/* finally, test if tri1 is totally contained in tri2 or vice versa */
	POINT_IN_TRI(V0, U0, U1, U2);
	POINT_IN_TRI(U0, V0, V1, V2);

	return 0;
}



#define NEWCOMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,A,B,C,X0,X1) \
{ \
        if(D0D1>0.0f) \
        { \
                /* here we know that D0D2<=0.0 */ \
            /* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
                A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
        } \
        else if(D0D2>0.0f)\
        { \
                /* here we know that d0d1<=0.0 */ \
            A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
        } \
        else if(D1*D2>0.0f || D0!=0.0f) \
        { \
                /* here we know that d0d1<=0.0 or that D0!=0.0 */ \
                A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2; \
        } \
        else if(D1!=0.0f) \
        { \
                A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
        } \
        else if(D2!=0.0f) \
        { \
                A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
        } \
        else \
        { \
                /* triangles are coplanar */ \
                return coplanar_tri_tri(N1,V0,V1,V2,U0,U1,U2); \
        } \
}



int IntersectionTests::NoDivTriTriIsect(float V0[3], float V1[3], float V2[3],
	float U0[3], float U1[3], float U2[3])
{
	float E1[3], E2[3];
	float N1[3], N2[3], d1, d2;
	float du0, du1, du2, dv0, dv1, dv2;
	float D[3];
	float isect1[2], isect2[2];
	float du0du1, du0du2, dv0dv1, dv0dv2;
	short index;
	float vp0, vp1, vp2;
	float up0, up1, up2;
	float bb, cc, max;

	/* compute plane equation of triangle(V0,V1,V2) */
	SUB(E1, V1, V0);
	SUB(E2, V2, V0);
	CROSS(N1, E1, E2);
	d1 = -DOT(N1, V0);
	/* plane equation 1: N1.X+d1=0 */

	/* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
	du0 = DOT(N1, U0) + d1;
	du1 = DOT(N1, U1) + d1;
	du2 = DOT(N1, U2) + d1;

	/* coplanarity robustness check */
#if USE_EPSILON_TEST==TRUE
	if (FABS(du0) < EPSILON) du0 = 0.0;
	if (FABS(du1) < EPSILON) du1 = 0.0;
	if (FABS(du2) < EPSILON) du2 = 0.0;
#endif
	du0du1 = du0 * du1;
	du0du2 = du0 * du2;

	if (du0du1 > 0.0f && du0du2 > 0.0f) /* same sign on all of them + not equal 0 ? */
		return 0;                    /* no intersection occurs */

	  /* compute plane of triangle (U0,U1,U2) */
	SUB(E1, U1, U0);
	SUB(E2, U2, U0);
	CROSS(N2, E1, E2);
	d2 = -DOT(N2, U0);
	/* plane equation 2: N2.X+d2=0 */

	/* put V0,V1,V2 into plane equation 2 */
	dv0 = DOT(N2, V0) + d2;
	dv1 = DOT(N2, V1) + d2;
	dv2 = DOT(N2, V2) + d2;

#if USE_EPSILON_TEST==TRUE
	if (FABS(dv0) < EPSILON) dv0 = 0.0;
	if (FABS(dv1) < EPSILON) dv1 = 0.0;
	if (FABS(dv2) < EPSILON) dv2 = 0.0;
#endif

	dv0dv1 = dv0 * dv1;
	dv0dv2 = dv0 * dv2;

	if (dv0dv1 > 0.0f && dv0dv2 > 0.0f) /* same sign on all of them + not equal 0 ? */
		return 0;                    /* no intersection occurs */

	  /* compute direction of intersection line */
	CROSS(D, N1, N2);

	/* compute and index to the largest component of D */
	max = (float)FABS(D[0]);
	index = 0;
	bb = (float)FABS(D[1]);
	cc = (float)FABS(D[2]);
	if (bb > max) max = bb, index = 1;
	if (cc > max) max = cc, index = 2;

	/* this is the simplified projection onto L*/
	vp0 = V0[index];
	vp1 = V1[index];
	vp2 = V2[index];

	up0 = U0[index];
	up1 = U1[index];
	up2 = U2[index];

	/* compute interval for triangle 1 */
	float a, b, c, x0, x1;
	NEWCOMPUTE_INTERVALS(vp0, vp1, vp2, dv0, dv1, dv2, dv0dv1, dv0dv2, a, b, c, x0, x1);

	/* compute interval for triangle 2 */
	float d, e, f, y0, y1;
	NEWCOMPUTE_INTERVALS(up0, up1, up2, du0, du1, du2, du0du1, du0du2, d, e, f, y0, y1);

	float xx, yy, xxyy, tmp;
	xx = x0 * x1;
	yy = y0 * y1;
	xxyy = xx * yy;

	tmp = a * xxyy;
	isect1[0] = tmp + b * x1*yy;
	isect1[1] = tmp + c * x0*yy;

	tmp = d * xxyy;
	isect2[0] = tmp + e * xx*y1;
	isect2[1] = tmp + f * xx*y0;

	SORT(isect1[0], isect1[1]);
	SORT(isect2[0], isect2[1]);

	if (isect1[1] < isect2[0] || isect2[1] < isect1[0]) return 0;
	return 1;
}