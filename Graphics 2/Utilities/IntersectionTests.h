/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(float boxcenter[3],      */
/*          float boxhalfsize[3],float triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/
#pragma once
class IntersectionTests
{
public:
	typedef float real;

	static int tri_tri_overlap_test_3d(real p1[3], real q1[3], real r1[3],
		real p2[3], real q2[3], real r2[3]);


	static int coplanar_tri_tri3d(real  p1[3], real  q1[3], real  r1[3],
		real  p2[3], real  q2[3], real  r2[3],
		real  N1[3], real  N2[3]);


	static int tri_tri_overlap_test_2d(real p1[2], real q1[2], real r1[2],
		real p2[2], real q2[2], real r2[2]);


	static int tri_tri_intersection_test_3d(real p1[3], real q1[3], real r1[3],
		real p2[3], real q2[3], real r2[3],
		int * coplanar,
		real source[3], real target[3]);

	static int planeBoxOverlap(float normal[3], float d, float maxbox[3]);
	static int planeBoxOverlap(double normal[3], double d, double maxbox[3]);
	static int triBoxOverlap(float boxhalfsize[3], float v0[3], float v1[3], float v2[3]);
	static int triBoxOverlap(double boxcenter[3], double boxhalfsize[3], double triverts[3][3]);
	static bool OBB_RAY_Intersect(float fromRay[3], float toRay[3]);
};