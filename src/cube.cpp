#include "cube.h"

namespace rubik_cube
{

cube_t::cube_t()
{
	for(int i = 0; i != 6; ++i)
	{
		int fid = i * 9;
		for(int j = 0; j != 9; ++j)
			_C[fid + j] = i;
	}

	for(int i = 0; i != 20; ++i)
		_B[i] = i;
}

cube_t::~cube_t()
{
}

void cube_t::rotate(face_t::face_type type, int count)
{
	static const int permutation[][12] = 
	{
		{ 39, 38, 37, 12, 11, 10, 21, 20, 19, 30, 29, 28 }, // top
		{  7,  6,  5, 37, 44, 43, 52, 51, 50, 23, 22, 21 }, // front
		{  1,  8,  7, 10, 17, 16, 50, 49, 48, 32, 31, 30 }, // left
		{  3,  2,  1, 19, 26, 25, 48, 47, 46, 41, 40, 39 }, // back
		{  5,  4,  3, 28, 35, 34, 46, 53, 52, 14, 13, 12 }, // right
		{ 43, 42, 41, 34, 33, 32, 25, 24, 23, 16, 15, 14 }  // bottom
	};

	static const int permutation_block[][8] = 
	{
		{  4, 16,  5, 17,  6, 18,  7, 19 }, // top
		{  7, 18,  6, 14,  2, 10,  3, 15 }, // front
		{  4, 19,  7, 15,  3, 11,  0, 12 }, // left
		{  4, 12,  0,  8,  1, 13,  5, 16 }, // back
		{  5, 13,  1,  9,  2, 14,  6, 17 }, // right
		{  0, 11,  3, 10,  2,  9,  1,  8 }  // bottom
	};

	const int* P = permutation[type], *PB = permutation_block[type];

	int fid = type * 9;
	count = (count % 4 + 4) % 4;

	while(count --> 0)
	{
		int T[3] = { _C[P[9]], _C[P[10]], _C[P[11]] };

		for(int i = 11; i >= 3; --i)
			_C[P[i]] = _C[P[i - 3]];

		_C[P[0]] = T[0];
		_C[P[1]] = T[1];
		_C[P[2]] = T[2];

		int T2[2] = { _C[fid + 7], _C[fid + 8] };
		for(int i = 8; i >= 3; --i)
			_C[fid + i] = _C[fid + i - 2];
		_C[fid + 1] = T2[0];
		_C[fid + 2] = T2[1];

		// rotate corners
		int T3 = _B[PB[6]];
		_B[PB[6]] = _B[PB[4]];
		_B[PB[4]] = _B[PB[2]];
		_B[PB[2]] = _B[PB[0]];
		_B[PB[0]] = T3;

		// rotate edges
		T3 = _B[PB[7]];
		_B[PB[7]] = _B[PB[5]];
		_B[PB[5]] = _B[PB[3]];
		_B[PB[3]] = _B[PB[1]];
		_B[PB[1]] = T3;
	}
}

face_t cube_t::getFace(face_t::face_type type) const
{
	static const int map[9] = { 1, 2, 3,
	                            8, 0, 4,
	                            7, 6, 5 };

	int fid = type * 9;
	face_t F;
	for(int i = 0; i != 9; ++i)
		F.C[i] = _C[fid + map[i]];
	return F;
}

block_t cube_t::getBlock(int level, int x, int y) const
{
	// { top, left, back, right, front, bottom }
	static const int map[27][7] = 
		  // level 0 (bottom)
		{ { -1, 25, 32, -1, -1, 48,  0 }, // (0, 0, 0)
		  { -1, -1, 33, -1, -1, 47,  8 }, // (0, 0, 1)
		  { -1, -1, 34, 41, -1, 46,  1 }, // (0, 0, 2)
		  { -1, 24, -1, -1, -1, 49, 11 }, // (0, 1, 0)
		  { -1, -1, -1, -1, -1, 45, 20 }, // (0, 1, 1)
		  { -1, -1, -1, 42, -1, 53,  9 }, // (0, 1, 2)
		  { -1, 23, -1, -1, 16, 50,  3 }, // (0, 2, 0)
		  { -1, -1, -1, -1, 15, 51, 10 }, // (0, 2, 1)
		  { -1, -1, -1, 43, 14, 52,  2 }, // (0, 2, 2)

		  // level 1 (middle)
		  { -1, 26, 31, -1, -1, -1, 12 }, // (1, 0, 0)
		  { -1, -1, 27, -1, -1, -1, 23 }, // (1, 0, 1)
		  { -1, -1, 35, 40, -1, -1, 13 }, // (1, 0, 2)
		  { -1, 18, -1, -1, -1, -1, 26 }, // (1, 1, 0)
		  { -1, -1, -1, -1, -1, -1, 21 }, // (1, 1, 1)
		  { -1, -1, -1, 36, -1, -1, 24 }, // (1, 1, 2)
		  { -1, 22, -1, -1, 17, -1, 15 }, // (1, 2, 0)
		  { -1, -1, -1, -1,  9, -1, 25 }, // (1, 2, 1)
		  { -1, -1, -1, 44, 13, -1, 14 }, // (1, 2, 2)

		  // level 2 (top)
		  {  1, 19, 30, -1, -1, -1,  4 }, // (2, 0, 0)
		  {  2, -1, 29, -1, -1, -1, 16 }, // (2, 0, 1)
		  {  3, -1, 28, 39, -1, -1,  5 }, // (2, 0, 2)
		  {  8, 20, -1, -1, -1, -1, 19 }, // (2, 1, 0)
		  {  0, -1, -1, -1, -1, -1, 22 }, // (2, 1, 1)
		  {  4, -1, -1, 38, -1, -1, 17 }, // (2, 1, 2)
		  {  7, 21, -1, -1, 10, -1,  7 }, // (2, 2, 0)
		  {  6, -1, -1, -1, 11, -1, 18 }, // (2, 2, 1)
		  {  5, -1, -1, 37, 12, -1,  6 }  // (2, 2, 2)
		};

	const int *M = map[level * 9 + x * 3 + y];
	block_t B;

	B.id     = _B[M[6]];
	B.top    = M[0] == -1 ? -1 : _C[M[0]];
	B.left   = M[1] == -1 ? -1 : _C[M[1]];
	B.back   = M[2] == -1 ? -1 : _C[M[2]];
	B.right  = M[3] == -1 ? -1 : _C[M[3]];
	B.front  = M[4] == -1 ? -1 : _C[M[4]];
	B.bottom = M[5] == -1 ? -1 : _C[M[5]];

	return B;
}

corner_block_t cube_t::getCornerBlock() const
{
	static const int map[] = { 48, 46, 52, 50, 1, 3, 5, 7 };
	corner_block_t cb;
	for(int i = 0; i != 8; ++i)
	{
		cb.permutation[i] = _B[i];
		cb.top_bottom_color[i] = _C[map[i]];
	}

	return cb;
}

edge_block_t cube_t::getEdgeBlock() const
{
	static const int map[] = { 47, 53, 51, 49, 31, 35, 13, 17, 2, 4, 6, 8 };
	edge_block_t eb;
	for(int i = 0; i != 12; ++i)
	{
		eb.permutation[i] = _B[i + 8];
		eb.color[i] = _C[map[i]];
	}
	return eb;
}

}
