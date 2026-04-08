#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Veri boyutu (online ortam için)
#define SIZE 10000

int main() {
    FILE *file;      // dosya pointer'ı
    int i, value;    // döngü ve okunan veri için değişken


    // 1. DOSYA OLUŞTURMA (WRITE)
    // "wb" → binary write (ikili yazma modu)
    file = fopen("data.bin", "wb");

    // Dosya açılamazsa hata ver
    if (file == NULL) {
        printf("Dosya acilamadi!\n");
        return 1;
    }

    // Dosyaya 0'dan SIZE'a kadar sayıları yazıyoruz
    // Bu veri disk üzerinde ardışık (sequential) yerleşir
    for (i = 0; i < SIZE; i++) {
        fwrite(&i, sizeof(int), 1, file);
        // &i → yazılacak verinin adresi
        // sizeof(int) → her verinin boyutu
        // 1 → kaç tane veri yazılacak
        // file → hedef dosya
    }

    fclose(file); // dosyayı kapat

 
    // 2. SEQUENTIAL SCAN (SIRALI OKUMA)
    // Dosyayı okuma modunda aç ("rb" → read binary)
    file = fopen("data.bin", "rb");

    if (file == NULL) {
        printf("Dosya acilamadi!\n");
        return 1;
    }

    clock_t start, end;  // süre ölçmek için

    start = clock();  // zaman ölçümü başlat

    // Baştan sona sırayla okuma
    for (i = 0; i < SIZE; i++) {
        fread(&value, sizeof(int), 1, file);
        // fread → dosyadan veri okur
        // &value → okunacak verinin tutulacağı yer
    }

    end = clock();  // zaman ölçümü bitir

    double seq_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Sequential Scan Time: %f seconds\n", seq_time);

    fclose(file);  // dosyayı kapat

    // 3. RANDOM ACCESS (RASTGELE ERİŞİM)
    file = fopen("data.bin", "rb");

    if (file == NULL) {
        printf("Dosya acilamadi!\n");
        return 1;
    }

    // Rastgele sayı üretimi için seed
    srand(time(NULL));

    start = clock();  // zaman ölçümü başlat

    for (i = 0; i < SIZE; i++) {

        // 0 - SIZE arasında rastgele index seç
        int random_index = rand() % SIZE;

        // fseek → dosyada belirtilen byte konumuna gider
        fseek(file, random_index * sizeof(int), SEEK_SET);
        // random_index * sizeof(int) → kaç byte ileri gidileceği
        // SEEK_SET → dosyanın başından itibaren

        // Seçilen konumdaki veriyi oku
        fread(&value, sizeof(int), 1, file);
    }

    end = clock();  // zaman ölçümü bitir

    double rand_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Random Access Time: %f seconds\n", rand_time);

    fclose(file);  // dosyayı kapat

    // 4. SONUÇ YORUMU (ders için önemli)
    printf("\nYorum:\n");

    if (seq_time < rand_time) {
        printf("Sequential scan daha hizlidir.\n");
        printf("Sebep: Locality + OS read-ahead + cache kullanimi.\n");
    } else {
        printf("Beklenmeyen sonuc! (Ortamdan kaynakli olabilir)\n");
    }

    return 0;
}