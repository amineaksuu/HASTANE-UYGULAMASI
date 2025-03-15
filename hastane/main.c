#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Hasta bilgileri için struct
typedef struct {
    char id[20]; 
    char ad[50];
    char soyad[50];
    int yas;
    char cinsiyet[10];
    char hastalik[100];
    int oncelikSeviyesi; // 1 (en acil) - 5 (en az acil)
} Hasta;

// Reçete bilgileri için struct
typedef struct {
    char receteNo[15]; 
    char ilaclar[200];
    char doktorAdi[50];
} Recete;

// HL7 formatýna uygun hasta kaydetme fonksiyonu
void hastaKaydet(Hasta h) {
    FILE *fp = fopen("hasta_kayit.hl7", "a");
    if (fp == NULL) {
        printf("Dosya acma hatasi!\n");
        return;
    }
    fprintf(fp, "PID|1||%s^^^HASTANE^MRN||%s^%s||%d||%s\n", h.id, h.ad, h.soyad, h.yas, h.cinsiyet);
    fclose(fp);
}

// Triage sýralamasý (önceliðe göre hastalarý sýralar)
void triage(Hasta hastalar[], int n) {
    int i, j;
    Hasta temp;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (hastalar[i].oncelikSeviyesi > hastalar[j].oncelikSeviyesi) {
                temp = hastalar[i];
                hastalar[i] = hastalar[j];
                hastalar[j] = temp;
            }
        }
    }
}

// Reçete numarasý üretme fonksiyonu
void receteNoUret(char *receteNo) {
    srand(time(NULL));
    sprintf(receteNo, "R%04d%02d%02d%04d", rand() % 9999, rand() % 12 + 1, rand() % 31 + 1, rand() % 9999);
}

// Hasta taburcu etme
void hastaTaburcuEt(char *hastaID) {
    printf("%s ID'li hasta taburcu edildi.\n", hastaID);
}

// Ana program
int main() {
    int secim;
    int hastaSayisi = 0;
    Hasta hastalar[100]; // Maksimum 100 hasta kaydedilebilir.

    while (1) {
        printf("\n--- Acil Servis Yonetim Sistemi ---\n");
        printf("1. Hasta Kaydet\n");
        printf("2. Hasta Listesi ve Oncelik Siralama\n");
        printf("3. Recete Numarasi Uret\n");
        printf("4. Hasta Taburcu Et\n");
        printf("5. Cikis\n");
        printf("Seciminizi yapin: ");
        scanf("%d", &secim);
        getchar(); // \n karakterini temizle

        if (secim == 1) {
            // Hasta kaydetme iþlemi
            Hasta yeniHasta;
            printf("Hasta ID: ");
            scanf("%s", yeniHasta.id);
            printf("Ad: ");
            scanf("%s", yeniHasta.ad);
            printf("Soyad: ");
            scanf("%s", yeniHasta.soyad);
            printf("Yas: ");
            scanf("%d", &yeniHasta.yas);
            printf("Cinsiyet: ");
            scanf("%s", yeniHasta.cinsiyet);
            printf("Hastalik: ");
            scanf(" %[^\n]", yeniHasta.hastalik); // Boþluklarý alabilmek için
            printf("Oncelik Seviyesi (1-5): ");
            scanf("%d", &yeniHasta.oncelikSeviyesi);

            hastalar[hastaSayisi++] = yeniHasta;
            hastaKaydet(yeniHasta);
            printf("Hasta basariyla kaydedildi!\n");

        } else if (secim == 2) {
            if (hastaSayisi == 0) {
                printf("Kayitli hasta yok!\n");
                continue;
            }
            // Öncelik sýralamasýna göre listeleme
            triage(hastalar, hastaSayisi);
            printf("\n--- Oncelik Sirasina Gore Hastalar ---\n");
            for (int i = 0; i < hastaSayisi; i++) {
                printf("%s %s (Oncelik: %d)\n", hastalar[i].ad, hastalar[i].soyad, hastalar[i].oncelikSeviyesi);
            }

        } else if (secim == 3) {
            // Reçete numarasý üretme
            char receteNo[15];
            receteNoUret(receteNo);
            printf("Olusturulan Recete No: %s\n", receteNo);

        } else if (secim == 4) {
            if (hastaSayisi == 0) {
                printf("Taburcu edilecek hasta yok!\n");
                continue;
            }
            // Hasta taburcu etme
            char hastaID[20];
            printf("Taburcu edilecek hastanin ID'sini girin: ");
            scanf("%s", hastaID);

            int bulundu = 0;
            for (int i = 0; i < hastaSayisi; i++) {
                if (strcmp(hastalar[i].id, hastaID) == 0) {
                    hastaTaburcuEt(hastaID);
                    // Hastayý diziden çýkar
                    for (int j = i; j < hastaSayisi - 1; j++) {
                        hastalar[j] = hastalar[j + 1];
                    }
                    hastaSayisi--;
                    bulundu = 1;
                    break;
                }
            }
            if (!bulundu) {
                printf("Hasta bulunamadi!\n");
            }

        } else if (secim == 5) {
            printf("Cikis yapiliyor...\n");
            break;
        } else {
            printf("Gecersiz secim! Tekrar deneyin.\n");
        }
    }
    
    return 0;
}
