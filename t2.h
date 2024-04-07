/* Ionita Alexandra-Mihaela - 314CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sz 100

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

typedef struct {
    unsigned int size;
    int factor;
    pixel **pixels;
} Image;

typedef struct {
    unsigned int x;
    unsigned int y;
} Coordonates;

typedef struct nod {
    pixel values;
    unsigned char type;
    unsigned int size;
    Coordonates coord;
    long long int mean;
    struct nod *copil1, *copil2, *copil3, *copil4;
} TNod, *TArb;

typedef struct celula {
  TArb info;
  struct celula* urm;
} TCelula, *TLista;

typedef struct coada {
  TLista inc, sf;
} TCoada;

TCoada* InitQ();
void IntrQ(TCoada *c, TArb r);
void ExtrQ(TCoada *c);
int maxx(int nc1, int nc2, int nc3, int nc4);
int niv_number(TArb arb);
int nr_frunze(TArb r);
int minn(int nc1, int nc2, int nc3, int nc4);
int niv_minim(TArb r, int niv_curr);
unsigned int last_req1(TArb r, int niv_curr, int niv);
void first_req(TArb arb, const char *output);
void second_req(TArb arb, const char *output);
void verify_mean(TArb arb, Image *image);
void head(TArb arb, Image *image);
void arb_construction(Image *image, TArb arb, TCoada *c);
TArb alloc_arb();
void free_image(Image *image);
void image_matrix(const char *file, FILE *InputFile, long long int x, Image *image);
void make_nod(TArb arb, FILE *file);
void compressed_image(Image *image, TArb arb, const char *out);
void req_3(const char *in, const char *out);
void req_1_2(const char *file, const char *Factor, const char *req, const char *output);
TArb Constr_Frunza(unsigned int dimensiune);
void DistrugeArb(TArb adrArbore);
