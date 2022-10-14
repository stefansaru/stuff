/*------------------------------------------------------------------------------
 car.h
------------------------------------------------------------------------------*/
#ifndef _CAR_H_
#define _CAR_H_

#include <stdio.h>

//conventia de caractere care alcatuiesc masinile
#define SQ_EXIT			'*'
#define SQ_MAIN_CAR		'?'
#define SQ_HORIZ_HEAD	'<'
#define SQ_HORIZ_BODY	'-'
#define SQ_HORIZ_TAIL	'>'
#define SQ_VERTI_HEAD	'^'
#define SQ_VERTI_BODY	'|'
#define SQ_VERTI_TAIL	'v'

//sensurile de miscare
#define UP		1
#define DOWN	2
#define LEFT	3
#define RIGHT	4

//directiile de miscare
#define HORIZONTAL	1
#define VERTICAL	2

//tipul Car retine datele constante ale unei masini
struct Car
{
	int length;		//lungimea
	int direction;	//directia de miscare (oriz/vert)
	int x;			//linia de start, folosita daca se misca pe orizontala
	int y;			//coloana de start, pt miscarea pe verticala

	Car()
	{
		this->length = 0; //lungimea e incrementata manual, pe parcurs
	}

	void SetLocation(int x, int y)
	{
		//dupa ce gasesc pe harta un cap de masina (SQ_MAIN_CAR sau
		//SQ_HORIZ_HEAD sau SQ_VERTI_HEAD) inseamna ca aceasta e
		//pozitia de start
		this->x = x;
		this->y = y;
	}
};
#endif//_CAR_H_
