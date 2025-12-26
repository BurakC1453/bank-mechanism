#include <stdio.h>
#include <string.h>

// --- FONKSİYON PROTOTİPLERİ ---
double faizHesapla(double mevcutBakiye, double faizOrani);
void bakiyeEkrani(double sonBakiye, const char kullaniciAdi[]);

// --- FONKSİYONLAR ---
double faizHesapla(double mevcutBakiye, double faizOrani) {
    return mevcutBakiye + (mevcutBakiye * faizOrani / 100.0);
}

void bakiyeEkrani(double sonBakiye, const char kullaniciAdi[]) {
    printf("\n--- Islem Ozeti ---\n");
    printf("Selamlar %s", kullaniciAdi); // fgets zaten \n ekler
    printf("Yeni bakiyeniz: %.2f TL\n", sonBakiye);
    printf("------------------\n");
}

// --- ANA PROGRAM ---
int main() {
    char kullaniciAdi[20];
    char sifre[20];
    double mevcutBakiye = 0.0, faizOrani, yatirilanTutar, cekilenTutar, sonBakiye;
    int hak = 3, secim;
    FILE *f;

    printf("Kullanici adinizi giriniz: ");
    fgets(kullaniciAdi, sizeof(kullaniciAdi), stdin);

    // Giriş Kontrolü
    while (hak > 0) {
        printf("Sifreyi giriniz: ");
        fgets(sifre, sizeof(sifre), stdin);

        if (strcmp(sifre, "1234\n") == 0) {
            printf("\nGiris basarili!\n");
            break;
        } else {
            hak--;
            printf("Sifre yanlis! Kalan hak: %d\n", hak);
        }
    }

    if (hak == 0) {
        printf("\nHesabiniz bloke olmustur. Banka ile iletisime geciniz.\n");
        return 0;
    }

    f = fopen("bakiye.txt", "r");

    if(f == NULL) {
        printf("Bakiye dosyasi bulunamadi. Varsayilan bakiye 0 olarak ayarlandi.\n");
        mevcutBakiye =0.0;    

    }   else {
        fscanf(f, "%lf", &mevcutBakiye);
        fclose(f);
    }


    // Menü Döngüsü
    do {
        printf("\n======= BANKACILIK MENUSU =======\n");
        printf("1. Bakiye Goruntuleme\n");
        printf("2. Faiz Hesaplama\n");
        printf("3. Para Yatirma\n");
        printf("4. Para Cekme\n");
        printf("0. Cikis\n");
        printf("Seciminiz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                printf("\n>> Mevcut bakiyeniz: %.2f TL\n", mevcutBakiye);
                break;

            case 2:
                if (mevcutBakiye >= 1000) {
                    printf("Uygulanacak faiz oranini giriniz (%%): ");
                    scanf("%lf", &faizOrani);
                    sonBakiye = faizHesapla(mevcutBakiye, faizOrani);
                    bakiyeEkrani(sonBakiye, kullaniciAdi);
                } else {
                    printf("\n[UYARI] Faiz icin bakiyeniz en az 1000 TL olmalidir.\n");
                }
                break;

            case 3:
                printf("Yatirilacak tutari giriniz: ");
                scanf("%lf", &yatirilanTutar);
                if (yatirilanTutar > 0) {
                    mevcutBakiye += yatirilanTutar;
                    printf("Yeni bakiyeniz: %.2f TL\n", mevcutBakiye);
                } else {
                    printf("Gecersiz tutar!\n");
                }
                break;

            case 4:
                printf("Cekilecek tutari giriniz: ");
                scanf("%lf", &cekilenTutar);
                if (cekilenTutar > mevcutBakiye) {
                    printf("\n[HATA] Yetersiz bakiye! Mevcut: %.2f\n", mevcutBakiye);
                } else if (cekilenTutar <= 0) {
                    printf("Gecersiz tutar!\n");
                } else {
                    mevcutBakiye -= cekilenTutar;
                    printf("Islem basarili. Yeni bakiyeniz: %.2f TL\n", mevcutBakiye);
                }
                break;

            case 0:
                printf("Cikis yapiliyor. Iyi gunler dileriz!\n");
                break;

            default:
                printf("\n[!] Gecersiz secim, tekrar deneyiniz.\n");
        }
    } while (secim != 0);

    // Bakiye dosyasini guncelle
    f = fopen("bakiye.txt", "w");
    if (f != NULL) {
        fprintf(f, "%.2f", mevcutBakiye);
        printf("Bakiye dosyasi guncellendi.\n");
        fclose(f);
    } else {
        printf("Bakiye dosyasi guncellenemedi!\n");
    }

    return 0;
}