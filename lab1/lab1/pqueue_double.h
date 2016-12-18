/*
 * Copyright 2010 Volkan Yazıcı <volkan.yazici@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */


/**
 * A simple usage of libpqueue. (Priorities are represented as "double" values.)
 */


#include <stdio.h>
#include <stdlib.h>

#include "pqueue.h"


typedef struct node_t
{
	double pri;
	int    val;
	size_t pos;
} node_t;


double get_pri(void *a);


pqueue_t * make_pqueue_double(int n);
