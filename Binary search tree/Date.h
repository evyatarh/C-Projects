#ifndef DATE_H
#define DATE_H

typedef struct Date* Date;

// We expect a string in the format dd/mm/yyyy
Date	DateCreate(char* date_line);
Date    DateCopy(Date d);
void	DateDestroy(Date d);
int	DateGetDay(Date d);
int	DateGetMonth(Date d);
int	DateGetYear(Date d);

#endif
