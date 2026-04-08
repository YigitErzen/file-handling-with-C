#include <stdio.h> //dosya işlemleri için
#include <stdint.h> //sabit boyutlu integer tipleri
#include <string.h> //string işlemleri

#define LOG_FILE "database.log" //log dosyasının adı
#define MAX_KEYS 100  //desteklenen maksimum anahtar sayısı

// index tablosu: key -> log dosyasında offset (byte konumu)
long index_table[MAX_KEYS];

// Log dosyasını okuyarak index'i yeniden oluşturur
void rebuild_index()
{
    // tüm index değerlerini başlangıçta -1 yapıyoruz
    for(int i = 0; i<MAX_KEYS; i++)
    index_table[i] = -1;

    // log dosyasını okuma modunda aç
    FILE *file = fopen(LOG_FILE, "rb");

     // dosya yoksa hiçbir şey yapmadan çık
    if(file == NULL)
        return;

    // dosya sonuna kadar kayıtları okuyacağız
    while(1)
    {
        // şu an bulunduğumuz byte konumunu al
        long offset = ftell(file);

        int key;
        int length;

        // key okunamazsa dosya bitmiş demektir
        if(fread(&key, sizeof(int), 1, file))
            break;
        // veri uzunluğunu oku
        fread(&length,sizeof(int),1,file);

        // veri kısmını atla (okumaya gerek yok)
        fseek(file,length,SEEK_CUR);

        // index güncelle (en son görülen kayıt günceldir)
        if(key >=0 && key < MAX_KEYS)
            index_table[key] = offset;
    }
    //dosyayı kapat
    fclose(file);
}
// Yeni veri ekleme (append only write)
void put_value(int key, char *value)
{
    // log dosyasını append modunda aç
    FILE *file = fopen(LOG_FILE, "ab");

    // dosya açılamazsa çık
    if(file == NULL)
        return;

    // string uzunluğunu hesapla
    int length = strlen(value);
    
    // yazmaya başlamadan önce offset al
    long offset = ftell(file);

    // key yaz
    fwrite(&key,sizeof(int),1,file);

    // veri uzunluğunu yaz
    fwrite(&length,sizeof(int),1,file);

    // veri içeriğini yaz
    fwrite(value,1,length,file);

    // dosyayı kapat
    fclose(file);

    // index tablosunu güncelle
    index_table[key] = offset;
}

// Veri okuma
void get_value(int key)
{
    // index yoksa veri bulunamaz
    if(index_table[key] == -1)
    {
        printf("Key bulunamadi\n");
        return;
    }

    // log dosyasını aç
    FILE *file = fopen(LOG_FILE,"rb");

    // index'te belirtilen konuma git
    fseek(file,index_table[key],SEEK_SET);

    int stored_key;
    int length;

    // key oku
    fread(&stored_key,sizeof(int),1,file);

    // veri uzunluğunu oku
    fread(&length,sizeof(int),1,file);

    char buffer[100];

    // veriyi oku
    fread(buffer,1,length,file);

    buffer[length] = '\0';

    // sonucu yazdır
    printf("Key %d -> %s\n",stored_key,buffer);

    // dosyayı kapat
    fclose(file);
}

// Eski kayıtları temizleyen basit cleaner
void clean_log()
{
    // yeni temiz log dosyası
    FILE *new_file = fopen("clean.log","wb");

    // eski log dosyası
    FILE *old_file = fopen(LOG_FILE,"rb");

    if(old_file == NULL)
        return;

    int key,length;
    char buffer[100];

    // tüm log'u baştan sona oku
    while(fread(&key,sizeof(int),1,old_file)==1)
    {
        fread(&length,sizeof(int),1,old_file);
        fread(buffer,1,length,old_file);

        // sadece en güncel kayıtları yaz
        if(index_table[key] == ftell(old_file) - (length + sizeof(int)*2))
        {
            fwrite(&key,sizeof(int),1,new_file);
            fwrite(&length,sizeof(int),1,new_file);
            fwrite(buffer,1,length,new_file);
        }
    }

    fclose(old_file);
    fclose(new_file);

    printf("Log temizlendi\n");
}



// Program başlangıcı
int main()
{
    // program başlarken index'i log'dan yeniden kur
    rebuild_index();

    // bazı veriler ekleyelim
    put_value(1,"Ali");
    put_value(2,"Veli");
    put_value(1,"Ali Yilmaz");   // update -> append

    // veri okuyalım
    get_value(1);
    get_value(2);

    // log temizleme işlemi
    clean_log();

    return 0;
}