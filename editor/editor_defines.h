#ifndef EDITOR_DEFINES_H
#define EDITOR_DEFINES_H

#define WINDOWS_SIZE_W 1024
#define WINDOWS_SIZE_H 768

#define CELL_SIZE_W 32
#define CELL_SIZE_H 32

#define DIALOG_SIZE_W 640 
#define DIALOG_SIZE_H 240

#define QUANTITY_CELLS (((WINDOWS_SIZE_W)/(CELL_SIZE_W))*((WINDOWS_SIZE_H)/(CELL_SIZE_H)))

#define IDCLASS_NULL 0
#define IDCLASS_CHELL 1
#define IDCLASS_BLOQUE_ROCA 2
#define IDCLASS_BLOQUE_METAL 3
#define IDCLASS_COMPUERTA_REG 4
#define IDCLASS_COMPUERTA_AND 5
#define IDCLASS_COMPUERTA_OR 6
#define IDCLASS_BOTON 7
#define IDCLASS_RECEPTOR 8
#define IDCLASS_PASTEL 9
#define IDCLASS_ACIDO 10
#define IDCLASS_ROCA 11
#define IDCLASS_BARRERA_VERTICAL 12
#define IDCLASS_BARRERA_HORIZONTAL 13

#define DIR_IMAGEN_CHELL_A ":/resources/shell.png"
#define DIR_IMAGEN_BLOQUE_ROCA ":/resources/bloqueRoca.png"
#define DIR_IMAGEN_BLOQUE_METAL ":/resources/bloqueMetal.png"
#define DIR_IMAGEN_COMPUERTA_REG ":/resources/compuerta1.png"
#define DIR_IMAGEN_COMPUERTA_AND ":/resources/compuerta2.png"
#define DIR_IMAGEN_COMPUERTA_OR ":/resources/compuerta3.png"
#define DIR_IMAGEN_BOTON ":/resources/boton.png"
#define DIR_IMAGEN_RECEPTOR ":/resources/receptorEnergia270.png"
#define DIR_IMAGEN_PASTEL ":/resources/cake.png"
#define DIR_IMAGEN_ACIDO ":/resources/acido2.png"
#define DIR_IMAGEN_ROCA ":/resources/roca1.png"
#define DIR_IMAGEN_BARRERA_VERTICAL ":/resources/barreraDeEnergiaVertical.png"
#define DIR_IMAGEN_BARRERA_HORIZONTAL ":/resources/barreraDeEnergiaHorizontal.png"


#endif // EDITOR_DEFINES_H
