
#include "stdafx.h"
#include "pqueue_double.h"
#include "pqueue.h"

static int
cmp_pri(double next, double curr)
{
	return (next < curr);
}


double
get_pri(void *a)
{
	return (double) ((node_t *) a)->pri;
}


static void
set_pri(void *a, double pri)
{
	((node_t *) a)->pri = pri;
}


static size_t
get_pos(void *a)
{
	return ((node_t *) a)->pos;
}


static void
set_pos(void *a, size_t pos)
{
	((node_t *) a)->pos = pos;
}

pqueue_t * make_pqueue_double(int n) {
  return pqueue_init(n, cmp_pri, get_pri, set_pri, get_pos, set_pos);
}
