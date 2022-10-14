/*------------------------------------------------------------------------------
 util.h
------------------------------------------------------------------------------*/
#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//motive pt iesire
#define SUCCESS		0
#define OUT_OF_MEM	1
#define BAD_INPUT	2
#define FAILURE		3

void MyExit(int exit_code)
{
	switch(exit_code)
	{
	case SUCCESS:		printf("\n succes"); break;
	case OUT_OF_MEM:	printf("\n memorie insuficienta"); break;
	case BAD_INPUT:		printf("\n verifica fisierul de intrare"); break;
	case FAILURE:		printf("\n nu s-au gasit solutii");break;
	default:break;
	}

	printf("\n apasa o tasta");
	_getch();
	exit(exit_code);
}

//alocare memorie cu test
void *MyAlloc(size_t num_elements, size_t size_of_elements)
{
	void *address = calloc(num_elements,size_of_elements);

	if(address == NULL) MyExit(OUT_OF_MEM);

	return address;
}

#endif// _UTIL_H_
