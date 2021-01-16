#include <stdlib.h>
#include <stdio.h>

#include "pga2d.h"

void ga_print(const PGA2D a)
{
  size_t n = 0;
  for (size_t i = 0, j = 0; i < 8; i++)
    if (a[i] != 0.0f)
    {
      n++;
      printf("%s%0.7g%s",
        // sign between terms
        (j > 0) ? " + " : "",
        // coeff
        a[i],
        (i == 0) ? "" : pga2d_basis[i]);
      j++;
    };
    if (n==0) printf("0");
    printf("\n");
}

PGA2Dp ga_point(PGA2Dp point, const float x, const float y)
{
	PGA2D xe = {0}, ye = {0};
	return pga2d_add3(
		point,
		pga2d_muls(xe, pga2d_e02, -x),
		pga2d_muls(ye, pga2d_e01, y),
		pga2d_e12);
}

PGA2Dp ga_line(PGA2Dp line, const float a, const float b, const float c)
{
	PGA2D ae = {0}, be = {0}, ce = {0};
	return pga2d_add3(
		line,
		pga2d_muls(ae, pga2d_e1, a),
		pga2d_muls(be, pga2d_e2, b),
		pga2d_muls(ce, pga2d_e0, c));
}

PGA2Dp ga_rotor(PGA2Dp rotor, PGA2D point, float angle)
{
	PGA2D pn = {0};
	return pga2d_adds(
		rotor,
		pga2d_muls(
			pn,
			pga2d_normalized(pn, point),
			sinf(angle / 2.0f)),
		cosf(angle / 2.0f));
}

PGA2Dp ga_translator(PGA2Dp translator, PGA2D point, float dist)
{
	PGA2D pn = {0};
	return pga2d_adds(
		translator,
		pga2d_muls(
			pn,
			point,
			dist / 2.0f),
		1.0f);
}
