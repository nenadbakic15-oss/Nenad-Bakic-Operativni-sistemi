
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAZIV_DATOTEKE "zahtevi.txt"
#define MAX_DUZINA 100

typedef struct Cvor {
    int redni_broj;
    char naziv_sloja[MAX_DUZINA];
    char zahtev[MAX_DUZINA];
    char odgovor[MAX_DUZINA];
    struct Cvor* sledeci;
} Cvor;


Cvor* kreiraj_cvor(int redni_broj, const char* naziv_sloja, const char* zahtev, const char* odgovor) {
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor));

    if (novi == NULL) {
        printf("Greska: nije moguca alokacija memorije.\n");
        return NULL;
    }

    novi->redni_broj = redni_broj;
    strcpy(novi->naziv_sloja, naziv_sloja);
    strcpy(novi->zahtev, zahtev);
    strcpy(novi->odgovor, odgovor);
    novi->sledeci = NULL;

    return novi;
}


void dodaj_na_kraj(Cvor** glava, int redni_broj, const char* naziv_sloja, const char* zahtev, const char* odgovor) {
    Cvor* novi = kreiraj_cvor(redni_broj, naziv_sloja, zahtev, odgovor);
    if (novi == NULL) {
        return;
    }

    if (*glava == NULL) {
        *glava = novi;
    } else {
        Cvor* tekuci = *glava;
        while (tekuci->sledeci != NULL) {
            tekuci = tekuci->sledeci;
        }
        tekuci->sledeci = novi;
    }
}


void obrisi_listu(Cvor** glava) {
    Cvor* tekuci = *glava;
    Cvor* pomocni = NULL;

    while (tekuci != NULL) {
        pomocni = tekuci;
        tekuci = tekuci->sledeci;
        free(pomocni);
    }

    *glava = NULL;
}


void prikazi_listu(Cvor* glava) {
    if (glava == NULL) {
        printf("\nLista je prazna.\n");
        return;
    }

    printf("\n--- SADRZAJ LISTE ---\n");
    while (glava != NULL) {
        printf("Redni broj: %d\n", glava->redni_broj);
        printf("Sloj: %s\n", glava->naziv_sloja);
        printf("Zahtev: %s\n", glava->zahtev);
        printf("Odgovor: %s\n", glava->odgovor);
        printf("------------------------------\n");

        glava = glava->sledeci;
    }
}


void obradi_zahtev(const char* zahtev, char* odgovor) {
    if (strcmp(zahtev, "citanje datoteke") == 0) {
        strcpy(odgovor, "Jezgro je obradilo zahtev za citanje datoteke.");
    }
    else if (strcmp(zahtev, "upis u datoteku") == 0) {
        strcpy(odgovor, "Jezgro je obradilo zahtev za upis u datoteku.");
    }
    else if (strcmp(zahtev, "komunikacija procesa") == 0) {
        strcpy(odgovor, "Jezgro je obradilo zahtev za medjuprocesnu komunikaciju.");
    }
    else if (strcmp(zahtev, "upravljanje memorijom") == 0) {
        strcpy(odgovor, "Jezgro je obradilo zahtev za upravljanje memorijom.");
    }
    else {
        strcpy(odgovor, "Nepoznat zahtev. Jezgro ne moze da obradi trazenu operaciju.");
    }
}

void unos_u_listu(Cvor** glava, int* brojac) {
    int broj_unosa, i;
    char naziv_sloja[MAX_DUZINA];
    char zahtev[MAX_DUZINA];
    char odgovor[MAX_DUZINA];

    printf("\nKoliko zahteva zelite da unesete? ");
    scanf("%d", &broj_unosa);
    getchar();

    for (i = 0; i < broj_unosa; i++) {
        printf("\nUnos %d.\n", i + 1);

        printf("Unesite naziv sloja (npr. Korisnicki sloj): ");
        fgets(naziv_sloja, MAX_DUZINA, stdin);
        naziv_sloja[strcspn(naziv_sloja, "\n")] = '\0';

        printf("Unesite zahtev (citanje datoteke / upis u datoteku / komunikacija procesa / upravljanje memorijom): ");
        fgets(zahtev, MAX_DUZINA, stdin);
        zahtev[strcspn(zahtev, "\n")] = '\0';

        obradi_zahtev(zahtev, odgovor);

        (*brojac)++;
        dodaj_na_kraj(glava, *brojac, naziv_sloja, zahtev, odgovor);

        printf("Zahtev je dodat u listu.\n");
    }
}


void snimi_listu_u_datoteku(Cvor* glava) {
    FILE* datoteka = fopen(NAZIV_DATOTEKE, "w");

    if (datoteka == NULL) {
        printf("\nGreska pri otvaranju datoteke za upis.\n");
        return;
    }

    if (glava == NULL) {
        printf("\nLista je prazna. Nema podataka za snimanje.\n");
        fclose(datoteka);
        return;
    }

    while (glava != NULL) {
        fprintf(datoteka, "%d|%s|%s|%s\n",
                glava->redni_broj,
                glava->naziv_sloja,
                glava->zahtev,
                glava->odgovor);
        glava = glava->sledeci;
    }

    fclose(datoteka);
    printf("\nSvi podaci iz liste su uspesno snimljeni u datoteku '%s'.\n", NAZIV_DATOTEKE);
}

void ucitaj_iz_datoteke_u_listu(Cvor** glava) {
    FILE* datoteka = fopen(NAZIV_DATOTEKE, "r");
    char linija[400];

    int redni_broj;
    char naziv_sloja[MAX_DUZINA];
    char zahtev[MAX_DUZINA];
    char odgovor[MAX_DUZINA];

    if (datoteka == NULL) {
        printf("\nGreska pri otvaranju datoteke za citanje.\n");
        return;
    }

    obrisi_listu(glava);

    while (fgets(linija, sizeof(linija), datoteka) != NULL) {
        linija[strcspn(linija, "\n")] = '\0';

        if (sscanf(linija, "%d|%99[^|]|%99[^|]|%99[^\n]",
                   &redni_broj, naziv_sloja, zahtev, odgovor) == 4) {
            dodaj_na_kraj(glava, redni_broj, naziv_sloja, zahtev, odgovor);
        }
    }

    fclose(datoteka);
    printf("\nPodaci su uspesno ucitani iz datoteke u listu.\n");
}


void prikazi_meni() {
    printf("\n=============================================\n");
    printf(" KOMUNIKACIJA SLOJEVA U OPERATIVNOM SISTEMU\n");
    printf("=============================================\n");
    printf("1. Unos zahteva u listu\n");
    printf("2. Prikaz trenutne liste na ekranu\n");
    printf("3. Snimanje cele liste u datoteku\n");
    printf("4. Ucitavanje iz datoteke u listu i prikaz na ekranu\n");
    printf("5. Brisanje liste iz memorije\n");
    printf("0. Izlaz iz programa\n");
    printf("=============================================\n");
    printf("Izaberite opciju: ");
}

int main() {
    Cvor* glava = NULL;
    int izbor;
    int brojac = 0;

    do {
        prikazi_meni();
        scanf("%d", &izbor);
        getchar();

        if (izbor == 1) {
            unos_u_listu(&glava, &brojac);
        }
        else if (izbor == 2) {
            prikazi_listu(glava);
        }
        else if (izbor == 3) {
            snimi_listu_u_datoteku(glava);
        }
        else if (izbor == 4) {
            ucitaj_iz_datoteke_u_listu(&glava);
            prikazi_listu(glava);
        }
        else if (izbor == 5) {
            obrisi_listu(&glava);
            printf("\nLista je obrisana iz memorije.\n");
        }
        else if (izbor == 0) {
            printf("\nIzlaz iz programa.\n");
        }
        else {
            printf("\nPogresan izbor. Pokusajte ponovo.\n");
        }

    } while (izbor != 0);

    obrisi_listu(&glava);
    return 0;
}
