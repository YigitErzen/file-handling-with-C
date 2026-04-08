#include <stdio.h>      // printf fonksiyonunu kullanmak için

#define N 20            // disk toplam 20 bloktan oluşur

int disk[N];            // disk dizisi (0 = boş, diğerleri dosya id)

// Diski ekrana yazdıran fonksiyon
void print_disk() {
    for (int i = 0; i < N; i++)        // tüm blokları sırayla dolaş
        printf("[%d]", disk[i]);       // her bloğu ekrana yaz
    printf("\n");                      // alt satıra geç
}

// Disk içeriğini başka bir diziye kopyalayan fonksiyon
void copy_disk(int target[], int source[]) {
    for (int i = 0; i < N; i++)        // tüm elemanları dolaş
        target[i] = source[i];         // kopyalama işlemi
}

// Dosya için boş bloklara yer ayıran fonksiyon
void allocate(int file, int size) {
    int count = 0;                     // kaç blok verildiğini tutar

    for (int i = 0; i < N; i++) {      // disk baştan sona taranır
        if (disk[i] == 0) {            // blok boşsa
            disk[i] = file;            // dosyayı buraya yerleştir
            count++;                   // yerleşen blok sayısını artır

            if (count == size)         // yeterli blok verildiyse
                break;                 // döngüyü bitir
        }
    }
}

// Fragmentation oranını hesaplayan fonksiyon
float fragmentation_ratio() {
    int fragments = 0;                 // Toplam parça sayısını tutar
    int used = 0;                      // Kullanılan toplam blok sayısını tutar

    for (int i = 0; i < N; i++) {      // disk baştan sona taranır
        if (disk[i] != 0) {            // blok doluysa
            used++;                    // kullanılan blok sayısını artır

            // Eğer ilk bloksa veya bir önceki bloktan farklıysa
            // yeni bir fragment başlamış demektir
            if (i == 0 || disk[i] != disk[i - 1])
                fragments++;
        }
    }

    if (used == 0)                     // Eğer hiç kullanılan blok yoksa bölme hatası olmaması için 0 döndür
        return 0.0;

    float ratio = (float)fragments / used;   // oran hesapla

    printf("Fragments: %d\n", fragments);
    printf("Used blocks: %d\n", used);
    printf("Fragmentation ratio: %.2f\n", ratio);

    return ratio;                      // sonucu döndür
}

//  aynı dosyanın bloklarını gruplayarak taşır
void defragment_grouped() {
    int temp[N];                       // geçici disk
    int j = 0;                         // yazma indexi

    // temp dizisini tamamen boş yap
    for (int i = 0; i < N; i++)
        temp[i] = 0;

    // her dosyayı sırayla ele al
    for (int file = 1; file <= 9; file++) {

        for (int i = 0; i < N; i++) {  // disk taranır

            if (disk[i] == file) {     // blok bu dosyaya aitse
                temp[j] = file;        // sırayla yan yana yerleştir
                j++;
            }
        }
    }

    // sonucu ana diske kopyala
    copy_disk(disk, temp);
}

// disk durumunu göster
void show_status(char title[]) {
    printf("\n%s\n", title);           // başlık yaz
    print_disk();                      // disk görünümü
    fragmentation_ratio();             // fragmentation bilgisi
}

// ana program
int main() {
    int saved[N];                      // Orijinal parçalanmış disk yapısını saklamak için dizi

    // başlangıçta disk tamamen boş
    for (int i = 0; i < N; i++)
        disk[i] = 0;

    allocate(1, 3);                    // 1 numaralı dosya için 3 blok ayır
    allocate(2, 4);                   // 2 numaralı dosya için 4 blok ayır

    printf("Initial disk:\n");
    print_disk();

     // Bilinçli olarak parçalanma oluşturmak için bazı blokları boşalt
    disk[1] = 0;
    disk[5] = 0;

    allocate(3, 3);                   // file3 boşluklara yerleşir

    show_status("Fragmented disk:");

    copy_disk(saved, disk);           // bu hali sakla

    defragment_grouped();             // defragmentation uygula

    show_status("After defragmentation:");

    return 0;
}