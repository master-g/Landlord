/*
 *  main.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-19.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "card.h"

int main(int argc, const char * argv[])
{
    CardArray_Test();
    
    memtrack_list_allocations();
    return 0;
}
