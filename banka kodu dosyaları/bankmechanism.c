#include <stdio.h>
#include <string.h>

// Kişisel bakiye olayı için ayrı struct kurulumu ve genel hesap bilgilerini toplama
struct musteriHesabi {
    char kullaniciAdi[20];

    char sifre[20];
    double mevcutBakiye;
};

// FONKSİYON PROTOTİPLERİ 
double faizHesapla(double bakiye, double faizOrani);
void bakiyeEkrani(double sonBakiye, const char kullaniciAdi[]);

// FONKSİYONLAR 
double faizHesapla(double bakiye, double faizOrani) {
    return bakiye + (bakiye * faizOrani / 100.0);
}

void bakiyeEkrani(double sonBakiye, const char kullaniciAdi[]) {
    printf("\n--- Islem Ozeti ---\n");
    printf("Selamlar %s\n", kullaniciAdi);
    printf("Su an ki bakiyenizi faize yatirmaniz sonucu elde edeceginiz bakiye: %.2f TL\n", sonBakiye);
    printf("------------------\n");
}

void dekontYaz(const char isim[], const char islemTuru[], double tutar) {
    FILE *f = fopen("dekont.txt", "a"); // "a" modu: Append (Sonuna Ekle)
    
    if (f != NULL) {
        // Dosyaya şu formatta yaz: [Kullanıcı] [İşlem] [Miktar]
        fprintf(f, "Musteri: %-10s | Islem: %-15s | Tutar: %.2f TL\n", isim, islemTuru, tutar);
        fclose(f);
    }
}

//  ANA PROGRAM 
int main() {
   
    struct musteriHesabi musteriler[50];
    int musteriSayisi = 0;
    double faizOrani, yatirilanTutar, cekilenTutar, sonBakiye;
    int hak = 3, secim;
    char girilenAd[20], girilenSifre[20];
    int aktifKullaniciIndex = -1; 
    FILE *f;


    f = fopen("hesaplar.txt", "r");
    if (f == NULL) {
        printf("Veritabani dosyasi (hesaplar.txt) bulunamadi! Once dosya olusturun.\n");
        return 1;
    }

    // Dosyadaki herkesi tek tek diziye aktarıyoruz
    while (fscanf(f, "%s %s %lf", musteriler[musteriSayisi].kullaniciAdi, 
                                   musteriler[musteriSayisi].sifre, 
                                   &musteriler[musteriSayisi].mevcutBakiye) != EOF) {
        musteriSayisi++;
    }
    fclose(f);
    printf("Sistem yuklendi. Toplam Musteri: %d\n", musteriSayisi);

    // YENİ GİRİŞ VE KAYIT ALGORİTMASI
    
    printf("\n--- BANKA GIRIS EKRANI ---\n");
    printf("Kullanici Adinizi Giriniz: ");
    scanf("%s", girilenAd);

    // 1. ADIM: Önce sadece bu kullanıcı var mı diye bakıyoruz
    aktifKullaniciIndex = -1; // Başlangıçta kimse yok
    
    for(int i = 0; i < musteriSayisi; i++) {
        if (strcmp(girilenAd, musteriler[i].kullaniciAdi) == 0) {
            aktifKullaniciIndex = i; // Kullanıcıyı bulduk!
            break; 
        }
    }

    // 2. ADIM: Duruma göre işlem yapma
    if (aktifKullaniciIndex != -1) {
        // --- SENARYO A: KULLANICI VAR -> ŞİFRE KONTROLÜ ---

        
        printf("Kullanici bulundu. Lutfen sifrenizi giriniz.\n");
        
        int girisBasarili = 0; // Döngüden ne durumda çıktığımızı anlamak için
        
        while (hak > 0) {
            printf("Sifre (Kalan hak %d): ", hak);
            scanf("%s", girilenSifre);

            if (strcmp(girilenSifre, musteriler[aktifKullaniciIndex].sifre) == 0) {
                printf("\nGiris Basarili! Hosgeldin %s\n", musteriler[aktifKullaniciIndex].kullaniciAdi);
                girisBasarili = 1;
                break; // Şifre doğru, döngüden çık
            } else {
                hak--;
                printf("Hatali sifre!\n");
            }
        }

        // Hak bittiyse veya giriş başarısızsa programı kapat
        if (girisBasarili == 0) {
            printf("\n3 defa hatali giris yaptiniz. Hesabiniz bloke oldu.\n");
            return 0; // Programı sonlandır
        }

    } else {
        //  SENARYO B: KULLANICI YOK -> HESAP OLUŞTURMA 
        
        printf("\nBu isimde bir kullanici bulunamadi.\n");
        printf("Yeni hesap olusturmak ister misiniz? (1: Evet, 0: Hayir): ");
        int cevap;
        scanf("%d", &cevap);

        if (cevap == 1) {
            // Dizi dolu mu kontrol et
            if (musteriSayisi >= 50) {
                printf("Banka kapasitesi dolu! Yeni musteri alinamiyor.\n");
                return 0;
            }

            // Yeni müşteriyi kaydetme işlemi
            // Kullanıcı adı zaten 'girilenAd' içinde duruyor, onu kopyalayalım
            strcpy(musteriler[musteriSayisi].kullaniciAdi, girilenAd);
            
            printf("Yeni hesabiniz icin bir sifre belirleyin: ");
            scanf("%s", musteriler[musteriSayisi].sifre);
            
            musteriler[musteriSayisi].mevcutBakiye = 0.0; // Yeni hesap 0 TL ile başlar

            printf("\nHesap basariyla olusturuldu! Aramiza hosgeldin %s.\n", girilenAd);
            
            // Yeni oluşturulan kullanıcıyı "giriş yapmış" sayıyoruz
            aktifKullaniciIndex = musteriSayisi;
            
            // Toplam müşteri sayısını arttırmayı unutma!
            musteriSayisi++; 

        } else {
            printf("Iyi gunler dileriz.\n");
            return 0; // Kayıt olmak istemedi, çıkış.
        }
    }


    // MENÜ İŞLEMLERİ 

    do {
        printf("\n======= BANKACILIK MENUSU =======\n 1. Bakiye Goruntuleme \n 2. Para Cekme \n 3. Para Yatirma \n 4. Faiz Hesaplama \n 0. Cikis \n Seciminiz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                printf("\n>> Mevcut bakiyeniz: %.2f TL\n", musteriler[aktifKullaniciIndex].mevcutBakiye);
                break;

            case 2:
                printf("Cekilecek tutari giriniz: ");
                scanf("%lf", &cekilenTutar);
                if (cekilenTutar > musteriler[aktifKullaniciIndex].mevcutBakiye) {
                    printf("\n[HATA] Yetersiz bakiye!\n");
                } else if (cekilenTutar <= 0) {
                    printf("Gecersiz tutar!\n");
                    dekontYaz(musteriler[aktifKullaniciIndex].kullaniciAdi, "HATALI CEKIM", cekilenTutar);
                } else {
                    musteriler[aktifKullaniciIndex].mevcutBakiye -= cekilenTutar;
                    printf("Islem basarili. Yeni bakiyeniz: %.2f TL\n", musteriler[aktifKullaniciIndex].mevcutBakiye);
                    dekontYaz(musteriler[aktifKullaniciIndex].kullaniciAdi, "PARA CEKME", cekilenTutar);
                }
                break;

            case 3:
                printf("Yatirilacak tutari giriniz: ");
                scanf("%lf", &yatirilanTutar);
                if (yatirilanTutar > 0) {
                    musteriler[aktifKullaniciIndex].mevcutBakiye += yatirilanTutar;
                    printf("Yeni bakiyeniz: %.2f TL\n", musteriler[aktifKullaniciIndex].mevcutBakiye);
                    dekontYaz(musteriler[aktifKullaniciIndex].kullaniciAdi, "PARA YATIRMA", yatirilanTutar);
                } else {
                    printf("Gecersiz tutar!\n");
                }
                break;

            case 4:
                if (musteriler[aktifKullaniciIndex].mevcutBakiye >= 1000) {
                    printf("Uygulanacak faiz oranini giriniz (%%): ");
                    scanf("%lf", &faizOrani);
                    // Fonksiyona sadece sayıyı gönderiyoruz:
                    sonBakiye = faizHesapla(musteriler[aktifKullaniciIndex].mevcutBakiye, faizOrani);
                    bakiyeEkrani(sonBakiye, musteriler[aktifKullaniciIndex].kullaniciAdi);
                } else {
                    printf("\n[UYARI] Faiz icin bakiyeniz en az 1000 TL olmalidir.\n");
                }
                break;

            case 0:
                printf("Cikis yapiliyor...\n");
                break;

            default:
                printf("\n[!] Gecersiz secim.\n");
        }
    } while (secim != 0);


    // ADIM 4: TÜM VERİTABANINI GÜNCELLEME 

    f = fopen("hesaplar.txt", "w");
    if (f != NULL) {
        // Dizideki tüm müşterileri (değişiklik yapmayanı bile) dosyaya geri yazıyoruz
        for(int i = 0; i < musteriSayisi; i++) {
            fprintf(f, "%s %s %.2f\n", musteriler[i].kullaniciAdi, 
                                        musteriler[i].sifre, 
                                        musteriler[i].mevcutBakiye);
        }
        printf("Veritabani guncellendi. Iyi gunler!\n");
        fclose(f);
    } else {
        printf("Dosya guncellenirken hata olustu!\n");
    }

    return 0;

}
