/* Ionita Alexandra-Mihaela - 314CB */

#include "t2.h"

/* Creeaza coada vida */

TCoada* InitQ() {
    TCoada* c = NULL;
    c = (TCoada*)malloc(sizeof(TCoada));
    if (!c) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    c->inc = c->sf = NULL;
    return c;
}

/* Adauga element la sfarsitul cozii */

void IntrQ(TCoada *c, TArb r) {
    TLista aux = NULL;
    aux = (TLista)malloc(sizeof(TCelula));
    if (!aux) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    aux->info = r;
    aux->urm = NULL;

    if (c->sf != NULL) {
        c->sf->urm = aux;
    } else {
        c->inc = aux;
    }
    c->sf = aux;
}

/* Extrage primul element din coada */

void ExtrQ(TCoada *c) {
    TLista p = NULL;
    p = c->inc;

    if (c->inc == NULL) {
        return;
    }

    c->inc = c->inc->urm;
    free(p);
}

/* Distruge coada */

void DistrQ(TCoada *c) {
  TLista p = NULL, aux = NULL;
  p = c->inc;
  while (p) {
    aux = p;
    p = p->urm;
    free(aux);
  }
  free(c);
  c = NULL;
}

/* Afla maximul dintre cele 4 variabile. Aceasta functie ajuta la aflarea
numarului de niveluri din arbore. */

int maxx(int nc1, int nc2, int nc3, int nc4) {
    int max = nc1;
    if (max < nc2) {
        max = nc2;
    }
    if (max < nc3) {
        max = nc3;
    }
    if (max < nc4) {
        max = nc4;
    }

    return max;
}

/* Afla numarul de niveluri din arbore recursiv */

int niv_number(TArb arb) {
    int nc1 = 0, nc2 = 0, nc3 = 0, nc4 = 0;
    if (!arb) {
        return 0;
    }

    nc1 = niv_number(arb->copil1);
    nc2 = niv_number(arb->copil2);
    nc3 = niv_number(arb->copil3);
    nc4 = niv_number(arb->copil4);

    return 1 + maxx(nc1, nc2, nc3, nc4);
}

/* Afla numarul de frunze din arbore recursiv, adica numărul de blocuri din
imagine pentru care scorul similaritătii pixelilor este mai mic sau egal decât
factorul furnizat */

int nr_frunze(TArb r) {
    if (!r) {
        return 0;
    }
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
    c1 = nr_frunze(r->copil1);
    c2 = nr_frunze(r->copil2);
    c3 = nr_frunze(r->copil3);
    c4 = nr_frunze(r->copil4);
    if (!r->copil1 && !r->copil2 && !r->copil3 && !r->copil4) {
        return 1 + c1 + c2 + c3 + c4;
    }
    return 0 + c1 + c2 + c3 + c4;
}

/* Afla minimul dintre cele 4 variabile */

int minn(int nc1, int nc2, int nc3, int nc4) {
    int min = nc1;
    if (min > nc2) {
        min = nc2;
    }
    if (min > nc3) {
        min = nc3;
    }
    if (min > nc4) {
        min = nc4;
    }

    return min;
}

/* Afla nivelul la care se afla cea mai de sus frunza, adica nivelul la care
se gaseste cea mai mare zonă din imagine care a rămas nedivizată, recursiv */

int niv_minim(TArb r, int niv_curr) {
    int nc1 = 0, nc2 = 0, nc3 = 0, nc4 = 0;
    if (!r) {
        return 0;
    }
    if (!r->copil1 && !r->copil2 && !r->copil3 && !r->copil4) {
        return niv_curr;
    }
    nc1 = niv_minim(r->copil1, niv_curr + 1);
    nc2 = niv_minim(r->copil2, niv_curr + 1);
    nc3 = niv_minim(r->copil3, niv_curr + 1);
    nc4 = niv_minim(r->copil4, niv_curr + 1);

    return minn(nc1, nc2, nc3, nc4);
}

/* Afla dimensiunea laturii pătratului pentru cea mai mare zonă din imagine
care a rămas nedivizată, cautand nodurile care se afla pe nivelul
gasit in functia anterioara, nodurile acelea avand acelasi size, recursiv */

unsigned int last_req1(TArb r, int niv_curr, int niv) {
    unsigned int nc1 = 0, nc2 = 0, nc3 = 0, nc4 = 0;
    if (!r) {
        return 0;
    }
    if (niv_curr == niv) {
        return r->size;
    }
    nc1 = last_req1(r->copil1, niv_curr + 1, niv);
    nc2 = last_req1(r->copil2, niv_curr + 1, niv);
    nc3 = last_req1(r->copil3, niv_curr + 1, niv);
    nc4 = last_req1(r->copil4, niv_curr + 1, niv);
}

/* Rezolva cele 3 "subcerinte" de la cerinta 1 si scrie rezultatele
intr-un fisier text */

void first_req(TArb arb, const char *output) {
    FILE *out = fopen(output, "w");
    int niv = niv_number(arb);
    int frunze = nr_frunze(arb);
    int NivMin = niv_minim(arb, 0);
    unsigned int size = last_req1(arb, 0, NivMin);
    fprintf(out, "%d\n%d\n%d\n", niv, frunze, size);
    fclose(out);
}

/* Scrie intr-un fisier binar datele cerute la cerinta 2: dimensiunea imaginii,
tipul nodului din arbore: pentru nod intern: 0, pentru nod frunza: 1, la cel
din urma adaugand si valorile corespunzatoare culorilor rosu, verde, albastru */

void second_req(TArb arb, const char *output) {
    FILE *out = fopen(output, "wb");

    /* Ma folosesc de coada pentru a parcurge arborele pe niveluri */

    TCoada *c = NULL;
    c = InitQ();
    IntrQ(c, arb);
    fwrite(&arb->size, sizeof(unsigned int), 1, out);

    while (c->inc) {
        if (c->inc->info->type == 0) {
            /* Daca este nod intern ii afisez tipul: 0 si introduc in
            coada copiii acestuia pentru a-i putea parcurge */

            fwrite(&c->inc->info->type, sizeof(unsigned char), 1, out);
            IntrQ(c, c->inc->info->copil1);
            IntrQ(c, c->inc->info->copil2);
            IntrQ(c, c->inc->info->copil3);
            IntrQ(c, c->inc->info->copil4);
        } else {
            /* Daca este nod frunza scriu in fisierul binar datele cerute */

            fwrite(&c->inc->info->type, sizeof(unsigned char), 1, out);
            fwrite(&c->inc->info->values.red, sizeof(unsigned char), 1, out);
            fwrite(&c->inc->info->values.green, sizeof(unsigned char), 1, out);
            fwrite(&c->inc->info->values.blue, sizeof(unsigned char), 1, out);
        }

        /* Indiferent de tipul nodului, il elimin din coada pentru a parcurge
        celelalte noduri */

        ExtrQ(c);
    }

    fclose(out);
    DistrQ(c);
}

/* Calculeaza scorul similaritatii pentru nodul introdus, introduce valorile
corespunzatoare culorilor rosu, verde, albastru si scorul similaritatii 
in datele nodului de tip TArb, cu ajutorul acestui scor determinand si
tipul nodului si introducand 0 sau 1 in campul type, in functie de tipul
acestuia: intern sau frunza */

void verify_mean(TArb arb, Image *image) {
    /* Determina pentru fiecare canal red green blue media culorii si
    introduce valorile obtinute in campurile corespunzatoare aflate
    in componenta nodului */

    long long int red = 0, gr = 0, bl = 0;
    for (unsigned int i = arb->coord.x; i < arb->coord.x + arb->size; i++) {
        for (unsigned int j = arb->coord.y; j < arb->coord.y + arb->size; j++) {
            red += (long long int)(image->pixels[i][j].red);
            gr += (long long int)(image->pixels[i][j].green);
            bl += (long long int)(image->pixels[i][j].blue);
        }
    }
    red = red / (arb->size * arb->size);
    bl = bl / (arb->size * arb->size);
    gr = gr / (arb->size * arb->size);
    arb->values.red = (unsigned char)red;
    arb->values.green = (unsigned char)gr;
    arb->values.blue = (unsigned char)bl;

    /* Calculeaza scorul similaritatii si il introduce in campul corespunzator
    din structura nodului */

    long long int mean = 0, g = 0, b = 0;
    for (unsigned int i = arb->coord.x; i < arb->coord.x + arb->size; i++) {
        for (unsigned int j = arb->coord.y; j < arb->coord.y + arb->size; j++) {
            mean += (red - (long long int)(image->pixels[i][j].red)) * (red - (long long int)(image->pixels[i][j].red));
            g = (gr - (long long int)(image->pixels[i][j].green)) * (gr - (long long int)(image->pixels[i][j].green));
            mean += g;
            b = (bl - (long long int)(image->pixels[i][j].blue)) * (bl - (long long int)(image->pixels[i][j].blue));
            mean += b;
        }
    }
    arb->mean = mean / (3 * arb->size * arb->size);

    /* Compara rezultatul cu factorul oferit si determina si introduce in structura
    nodului tipul corespunzator */

    if (arb->mean <= image->factor) {
        arb->type = 1;
    } else {
        arb->type = 0;
    }
}

/* Construieste primul nod din arbore, corespunzator intregii imagini */

void head(TArb arb, Image *image) {
    arb->coord.x = 0;
    arb->coord.y = 0;
    arb->size = image->size;
    verify_mean(arb, image);
}

/* Construieste intreg arborele de compresie */

void arb_construction(Image *image, TArb arb, TCoada *c) {
    /* Am folosit o coada pentru parcurgerea arborelui pe niveluri */

    while (c->inc) {
        if (c->inc->info->type == 0) {
            /* Daca arborele este intern, fac fiecare copil in parte de tip
            frunza folosind o functie explicata mai jos Constr_Frunza,
            introduc coordonatele blocului corespunzator fiecaruia si ii
            adaug celelalte date cu functia verify_mean explicata anterior.
            Introduc apoi copiii in coada pentru a putea fi parcursi
            ulterior. */

            c->inc->info->copil1 = Constr_Frunza(c->inc->info->size / 2);
            c->inc->info->copil1->coord.x = c->inc->info->coord.x;
            c->inc->info->copil1->coord.y = c->inc->info->coord.y;
            verify_mean(c->inc->info->copil1, image);
            IntrQ(c, c->inc->info->copil1);

            c->inc->info->copil2 = Constr_Frunza(c->inc->info->size / 2);
            c->inc->info->copil2->coord.x = c->inc->info->coord.x;
            c->inc->info->copil2->coord.y = c->inc->info->coord.y + (c->inc->info->size / 2);
            verify_mean(c->inc->info->copil2, image);
            IntrQ(c, c->inc->info->copil2);

            c->inc->info->copil3 = Constr_Frunza(c->inc->info->size / 2);
            c->inc->info->copil3->coord.x = c->inc->info->coord.x + (c->inc->info->size / 2);
            c->inc->info->copil3->coord.y = c->inc->info->coord.y + (c->inc->info->size / 2);
            verify_mean(c->inc->info->copil3, image);
            IntrQ(c, c->inc->info->copil3);

            c->inc->info->copil4 = Constr_Frunza(c->inc->info->size / 2);
            c->inc->info->copil4->coord.x = c->inc->info->coord.x + (c->inc->info->size / 2);
            c->inc->info->copil4->coord.y = c->inc->info->coord.y;
            verify_mean(c->inc->info->copil4, image);
            IntrQ(c, c->inc->info->copil4);
        }

        /* Indiferent de tipul nodului, il scot din coada pentru a parcurge celelalte noduri */

        ExtrQ(c);
    }
}

/* Aloca un element de tip TArb */

TArb alloc_arb() {
    TArb arb = (TArb)malloc(sizeof(TNod));
    if (!arb) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    arb->copil1 = NULL;
    arb->copil2 = NULL;
    arb->copil3 = NULL;
    arb->copil4 = NULL;

    return arb;
}

/* Dezaloca un element de tip Image */

void free_image(Image *image) {
    for (int i = 0; i < image->size; i++) {
        free(image->pixels[i]);
    }
    free(image->pixels);
    free(image);
}

/* Construieste matricea corespunzatoare imaginii */

void image_matrix(const char *file, FILE *InputFile, long long int x, Image *image) {
    /* Dupa ce deschid fisierul in format binar, ma folosesc de comanda\
    fseek pentru a pointa catre locul in care am ramas in fisier. Fac acest
    lucru deoarece fisierul a fost deschis initial in format text pentru
    accesarea unor date, iar acum trebuie deschis in format binar pentru 
    accesarea imaginii. */

    InputFile = fopen(file, "rb");
    fseek(InputFile, x, SEEK_SET);

    /* Aloc matricea corespunzatoare imaginii */

    image->pixels = (pixel**)malloc(image->size * sizeof(pixel*));
    if (!image->pixels) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    for (int i = 0; i < image->size; i++) {
        image->pixels[i] = (pixel*)malloc(image->size * sizeof(pixel));
        if (!image->pixels[i]) {
            fprintf(stderr, "Alloc failed");
            exit(1);
        }
    }

    /* Folosindu-ma de variabila c de tip unsigned char, citesc din fisier,
    pe rand, valoarea corespunzatoare fiecarei culori ce alcatuieste un pixel
    si introduc valoarea in matricea imaginii */

    for (int i = 0; i < image->size; i++) {
        for (int j = 0; j < image->size; j++) {
            unsigned char c = 0;
            fread(&c, sizeof(c), 1, InputFile);
            image->pixels[i][j].red = c;
            fread(&c, sizeof(c), 1, InputFile);
            image->pixels[i][j].green = c;
            fread(&c, sizeof(c), 1, InputFile);
            image->pixels[i][j].blue = c;
        }
    }

    fclose(InputFile);
}

/* Functie ajutatoare pentru cerinta 3. Aceasta citeste din fisierul binar
tipul nodului curent. Daca este nod frunza, stiu ca urmeaza in fisier date
legate de valorile celor trei culori ce alcatuiesc un pixel, asa ca
introduc aceste valori in structura nodului */

void make_nod(TArb arb, FILE *file) {
    fread(&arb->type, sizeof(unsigned char), 1, file);
    if (arb->type == 1) {
        fread(&arb->values.red, sizeof(unsigned char), 1, file);
        fread(&arb->values.green, sizeof(unsigned char), 1, file);
        fread(&arb->values.blue, sizeof(unsigned char), 1, file);
    }

    arb->copil1 = arb->copil2 = NULL;
    arb->copil3 = arb->copil4 = NULL;
}

/* Creeaza imaginea de la cerinta 3 */

void compressed_image(Image *image, TArb arb, const char *out) {
    /* Aloc matricea de pixeli */

    image->pixels = (pixel**)malloc(image->size * sizeof(pixel*));
    if (!image->pixels) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    for (int i = 0; i < image->size; i++) {
        image->pixels[i] = (pixel*)malloc(image->size * sizeof(pixel));
        if (!image->pixels[i]) {
            fprintf(stderr, "Alloc failed");
            exit(1);
        }
    }

    /* Ma folosesc de coada pentru a parcurge arborele pe niveluri */

    TCoada *c = NULL;
    c = InitQ();
    IntrQ(c, arb);

    while (c->inc) {
        if (c->inc->info->type == 0) {
            /* Daca nodul curent este intern, ii adaug copiii in coada 
            pentru a-i parcurge ulterior */

            IntrQ(c, c->inc->info->copil1);
            IntrQ(c, c->inc->info->copil2);
            IntrQ(c, c->inc->info->copil3);
            IntrQ(c, c->inc->info->copil4);
        } else {
            /* Daca nodul este frunza, parcurg matricea de pixeli care imi va construi
            imaginea in functie de coordonatele si dimensiunea blocului corespunzator
            nodului si adaug valorile aflate in nod pentru culorile ce alcatuiesc
            un pixel in matricea de pixeli */

            for (unsigned int i = c->inc->info->coord.x; i < c->inc->info->coord.x + c->inc->info->size; i++) {
                for (unsigned int j = c->inc->info->coord.y; j < c->inc->info->coord.y + c->inc->info->size; j++) {
                    image->pixels[i][j].red = c->inc->info->values.red;
                    image->pixels[i][j].green = c->inc->info->values.green;
                    image->pixels[i][j].blue = c->inc->info->values.blue;
                }
            }
        }

        /* Elimin din coada nodul abia parcurs pentru a le parcurge pe celelate */

        ExtrQ(c);
    }

    /* Deschid fisierul pentru a introduce date in format text, apoi il inchid
    si il redeschid cu ab (append binary) pentru a se deschide de unde a ramas
    dupa introducerea primelor date si a adauga date in format binar */

    FILE *comp_img = fopen(out, "w");
    fprintf(comp_img, "P6\n%d %d\n255\n", image->size, image->size);
    fclose(comp_img);
    comp_img = fopen(out, "ab");

    /* Scriu in fisier valorile culorilor rosu, verde, albastru, corespunzatoare
    fiecarui pixel in parte, pentru a crea imaginea */

    for (int i = 0; i < image->size; i++) {
        for (int j = 0; j < image->size; j++) {
            fwrite(&image->pixels[i][j].red, sizeof(unsigned char), 1, comp_img);
            fwrite(&image->pixels[i][j].green, sizeof(unsigned char), 1, comp_img);
            fwrite(&image->pixels[i][j].blue, sizeof(unsigned char), 1, comp_img);
        }
    }

    fclose(comp_img);
    DistrQ(c);
}

/* Rezolva cerinta 3 */

void req_3(const char *in, const char *out) {
    /* Deschid fisierul binar in care se afla datele ce vor duce la rezolvarea
    cerintei 3 */

    FILE *InputFile = fopen(in, "rb");
    if (!InputFile) {
        fprintf(stderr, "Failed to open the file");
        exit(1);
    }

    Image *image = malloc(sizeof(Image));
    if (!image) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    /* Construiesc primul nod, corespunzator intregii imagini. Il aloc,
    apoi adaug dimensiunea blocului caruia ii corespunde nodul, folosesc
    functia make_nod pentru a introduce anumite date, daca este cazul,
    si adaug coordonatele corespunzatoare blocului */

    TArb arb = alloc_arb();
    fread(&image->size, sizeof(unsigned int), 1, InputFile);
    arb->size = image->size;
    make_nod(arb, InputFile);
    arb->coord.x = 0;
    arb->coord.y = 0;

    /* Folosesc o coada pentru a parcurge arborele pe niveluri */

    TCoada *c = NULL;
    c = InitQ();
    IntrQ(c, arb);

    while (c->inc) {
        if (c->inc->info->type == 0) {
            /* Daca nodul curent este intern, ii construiesc cei 4 copii,
            deoarece fiind nod intern, stiu sigur ca acestia exista. Folosesc
            functia Constr_Frunza pentru a-i aloca si a le adauga in campul
            corespunzator dimensiunea blocului caruia ii corespunde fiecare.
            Folosesc functia make_nod pentru a le adauga datele ce se regasesc
            in fisierul binar. Introduc fiecare copil in coada pentru a-i
            parcurge ulterior */

            c->inc->info->copil1 = Constr_Frunza(c->inc->info->size / 2);
            c->inc->info->copil1->coord.x = c->inc->info->coord.x;
            c->inc->info->copil1->coord.y = c->inc->info->coord.y;
            make_nod(c->inc->info->copil1, InputFile);
            IntrQ(c, c->inc->info->copil1);

            c->inc->info->copil2 = Constr_Frunza(c->inc->info->size / 2);
            c->inc->info->copil2->coord.x = c->inc->info->coord.x;
            c->inc->info->copil2->coord.y = c->inc->info->coord.y + c->inc->info->copil2->size;
            make_nod(c->inc->info->copil2, InputFile);
            IntrQ(c, c->inc->info->copil2);

            c->inc->info->copil3 = Constr_Frunza(c->inc->info->size / 2);
            c->inc->info->copil3->coord.x = c->inc->info->coord.x + c->inc->info->copil3->size;
            c->inc->info->copil3->coord.y = c->inc->info->coord.y + c->inc->info->copil3->size;
            make_nod(c->inc->info->copil3, InputFile);
            IntrQ(c, c->inc->info->copil3);

            c->inc->info->copil4 = Constr_Frunza(c->inc->info->size / 2);
            c->inc->info->copil4->coord.x = c->inc->info->coord.x + c->inc->info->copil4->size;
            c->inc->info->copil4->coord.y = c->inc->info->coord.y;
            make_nod(c->inc->info->copil4, InputFile);
            IntrQ(c, c->inc->info->copil4);
        }

        /* Indiferent de tipul nodului, il elimin pentru a parcurge celelalte
        noduri */

        ExtrQ(c);
    }

    /* Apelez functia ce construieste imaginea */

    compressed_image(image, arb, out);

    fclose(InputFile);
    DistrQ(c);
    DistrugeArb(arb);
    free_image(image);
}

/* Rezolva primele doua cerinte */

void req_1_2(const char *file, const char *Factor, const char *req, const char *output) {
    Image *image = malloc(sizeof(Image));
    if (!image) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    /* Citesc datele de tip text din fisier si le introduc unde am nevoie */

    char *type = malloc(sz * sizeof(char));
    if (!type) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    char *image_size = malloc(sz * sizeof(char));
    if (!image_size) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    char *Width = malloc(sz * sizeof(char));
    if (!Width) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    char *Height = malloc(sz * sizeof(char));
    if (!Height) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    char *max_value = malloc(sz * sizeof(char));
    if (!max_value) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }

    FILE *InputFile = fopen(file, "r");
    if (!InputFile) {
        fprintf(stderr, "Failed to open %s file", file);
        exit(1);
    }

    if (!fgets(type, sz, InputFile)) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }
    type[strcspn(type, "\n")] = '\0';

    if (!fgets(image_size, sz, InputFile)) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }
    image_size[strcspn(image_size, "\n")] = '\0';

    sscanf(image_size, "%s %s", Width, Height);
    image->size = atoi(Width);

    if (!fgets(max_value, sz, InputFile)) {
        fprintf(stderr, "Alloc failed");
        exit(1);
    }
    max_value[strcspn(max_value, "\n")] = '\0';

    /* Folosesc ftell pentru a retine in x unde am ramas in fisier dupa citirea
    datelor text, deoarece vor urma niste date binare pentru care voi redeschide
    fisierul in format binar cu rb */

    long long int x = ftell(InputFile);
    fclose(InputFile);

    /* Creez matricea corespunzatoare imaginii */

    image_matrix(file, InputFile, x, image);

    /* Aloc un element de tip TArb pe care il fac sa pointeze catre 
    primul nod, corespunzator intregii imagini */

    TArb arbore = alloc_arb();
    image->factor = atoi(Factor);
    head(arbore, image);

    /* Construiesc o coada in care adaug elementul creat mai sus si apelez
    functia ce imi construieste intregul arbore de comprimare */

    TCoada *c = NULL;
    c = InitQ();
    IntrQ(c, arbore);
    arb_construction(image, arbore, c);

    /* Aflu ce cerinta trebuie rezolvata: 1 sau 2 */

    if (!strcmp(req, "-c1")) {
        first_req(arbore, output);
    }

    if (!strcmp(req, "-c2")) {
        second_req(arbore, output);
    }

    free(type);
    free(Height);
    free(Width);
    free(image_size);
    free(max_value);
    free_image(image);
    DistrugeArb(arbore);
    DistrQ(c);
}

/* Aloca un element de tip TArb si returneaza pointerul aferent */

TArb Constr_Frunza(unsigned int dimensiune) {
    TArb aux = (TArb) malloc(sizeof(TNod));
    if (!aux)
        return NULL;

    aux->size = dimensiune;
    aux->copil1 = aux->copil2 = NULL;
    aux->copil3 = aux->copil4 = NULL;

    return aux;
}

/* Dezalocare memorie arbore */

void DistrugeArb(TArb adrArbore) {
    if (!adrArbore) {
        return;
    }

    DistrugeArb(adrArbore->copil1);
    DistrugeArb(adrArbore->copil2);
    DistrugeArb(adrArbore->copil3);
    DistrugeArb(adrArbore->copil4);

    free(adrArbore);
}
