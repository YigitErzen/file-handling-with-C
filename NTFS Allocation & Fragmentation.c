#include <stdio.h>   // printf fonksiyonu için gerekli kütüphane

#define N 20         // Diskte toplam 20 cluster var

int disk[N];         // Disk dizisi: 0 = boş, 1-2-3... = dosya numarası

// Diski ekrana yazdıran fonksiyon
void print_disk() {
    for (int i = 0; i < N; i++)          // Tüm cluster'ları sırayla dolaş
        printf("[%d]", disk[i]);         // Her cluster'ın değerini yazdır
    printf("\n");                        // Alt satıra geç
}

// Boş cluster'lara dosya yerleştiren fonksiyon
void allocate_file(int file_id, int size) {
    int count = 0;                       // Kaç cluster verildiğini sayar

    for (int i = 0; i < N; i++) {        // Disk baştan sona taranır
        if (disk[i] == 0) {              // Cluster boşsa
            disk[i] = file_id;           // Bu cluster dosyaya atanır
            count++;                     // Verilen cluster sayısını artır

            if (count == size)           // İstenen kadar yer verildiyse
                break;                   // Döngüden çık
        }
    }
}

// Bir dosyanın kaç ayrı run içerdiğini hesaplar
int count_runs(int file_id) {
    int runs = 0;                        // Toplam run sayısı

    for (int i = 0; i < N; i++) {        // Disk baştan sona taranır
        if (disk[i] == file_id) {        // Bu cluster ilgili dosyaya aitse

            // İlk kez başlıyorsa veya bir önceki cluster aynı dosya değilse
            // burada yeni bir run başlar
            if (i == 0 || disk[i - 1] != file_id)
                runs++;
        }
    }

    return runs;                         // Run sayısını geri döndür
}

// Bir dosyanın run bilgilerini ekrana yazdırır
void show_runs(int file_id) {
    int start = -1;                      // Run başlangıç indexi
    int length = 0;                      // Run uzunluğu

    printf("File %d runs:\n", file_id);  // Başlık yazdır

    for (int i = 0; i < N; i++) {        // Tüm disk taranır

        if (disk[i] == file_id) {        // Bu cluster dosyaya aitse
            if (start == -1)             // Yeni run başlıyorsa
                start = i;               // Başlangıç indexini kaydet

            length++;                    // Run uzunluğunu artır
        }
        else {
            // Dosya parçası bittiyse run'ı yazdır
            if (start != -1) {
                printf("  Start = %d, Length = %d\n", start, length);
                start = -1;              // Yeni run için sıfırla
                length = 0;              // Uzunluğu sıfırla
            }
        }
    }

    // Eğer dosya en sonda bitiyorsa son run'ı da yazdır
    if (start != -1)
        printf("  Start = %d, Length = %d\n", start, length);
}

// Tüm dosyaları gruplayarak basit defragmentation yapar
void defragment() {
    int temp[N];                         // Geçici disk dizisi
    int j = 0;                           // Yeni dizide yazma pozisyonu

    for (int i = 0; i < N; i++)          // Önce geçici diziyi boş yap
        temp[i] = 0;

    // Dosyaları sırayla grupla
    for (int file_id = 1; file_id <= 9; file_id++) {
        for (int i = 0; i < N; i++) {
            if (disk[i] == file_id) {    // Bu cluster ilgili dosyaya aitse
                temp[j] = file_id;       // Yeni dizide sırayla yerleştir
                j++;
            }
        }
    }

    // Geçici diziyi ana diske kopyala
    for (int i = 0; i < N; i++)
        disk[i] = temp[i];
}

// Ana program
int main() {
    for (int i = 0; i < N; i++)          // Başlangıçta tüm disk boş
        disk[i] = 0;

    allocate_file(1, 3);                 // File 1 için 3 cluster ayır
    allocate_file(2, 4);                 // File 2 için 4 cluster ayır

    printf("Initial disk layout:\n");
    print_disk();

    // Bilinçli olarak boşluk oluştur
    disk[1] = 0;                         // File 1'in bir cluster'ı silindi
    disk[5] = 0;                         // File 2'nin bir cluster'ı silindi

    allocate_file(3, 3);                 // Yeni dosya boşluklara dağılarak yerleşir

    printf("\nAfter fragmentation:\n");
    print_disk();

    printf("\nRun information before defragmentation:\n");
    show_runs(1);
    show_runs(2);
    show_runs(3);

    printf("\nRun counts before defragmentation:\n");
    printf("File 1 -> %d run\n", count_runs(1));
    printf("File 2 -> %d run\n", count_runs(2));
    printf("File 3 -> %d run\n", count_runs(3));

    // Defragmentation uygula
    defragment();

    printf("\nAfter defragmentation:\n");
    print_disk();

    printf("\nRun information after defragmentation:\n");
    show_runs(1);
    show_runs(2);
    show_runs(3);

    printf("\nRun counts after defragmentation:\n");
    printf("File 1 -> %d run\n", count_runs(1));
    printf("File 2 -> %d run\n", count_runs(2));
    printf("File 3 -> %d run\n", count_runs(3));

    return 0;                            // Program sonu
}