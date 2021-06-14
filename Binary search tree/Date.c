#include "Date.h"
#include "misc.h"
#include <stdio.h>
#include <stdlib.h>

/***************************************************************************************************************************************/
struct Date {
	int day,month,year;
};

/***************************************************************************************************************************************/
Date    DateCreate(char* date_line)
{
	Date d = (Date)AllocateMemory(sizeof(struct Date), __FILE__, __LINE__);

	if(sscanf(date_line, "%d/%d/%d", &(d->day), &(d->month), &(d->year)) != 3) {
		fprintf(stderr, "%s, line %d: fatal error, unexpected date line: %s\n\n", __FILE__, __LINE__, date_line);
		exit(-1);
	}

	return d;
}

/***************************************************************************************************************************************/
Date	DateCopy(Date d)
{
	Date new_d = (Date)AllocateMemory(sizeof(struct Date), __FILE__, __LINE__);
	new_d->day = d->day;
	new_d->month = d->month;
	new_d->year = d->year;

	return new_d;
}

/***************************************************************************************************************************************/
void    DateDestroy(Date d)
{
	FreeMemory(d);
}

/***************************************************************************************************************************************/
int     DateGetDay(Date d)
{
	return d->day;
}

/***************************************************************************************************************************************/
int     DateGetMonth(Date d)
{
	return d->month;
}

/***************************************************************************************************************************************/
int     DateGetYear(Date d)
{
	return d->year;
}
