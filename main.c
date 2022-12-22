/*
  Tugas:
  Buatlah program manajemen data mahasiswa, yang berupa struct dan berisi:
  1. NIM
  2. Nama
  3. Jenis kelamin
  4. IPK

  Pastikan program dapat menambah, mengubah, menghapus, mencetak data mahasiswa,
  serta mengimpor dan mengekspor data mahasiswa dari file .txt menggunakan File
  Handling yang sudah kalian pelajari. Kalian bisa membuat antarmuka untuk user
  agar dapat memilih keenam pilihan tersebut. Pastikan kalian memberikan
  penjelasan program kalian dengan memberi komentar di samping atau di atas kode
  yang dibuat. Selain ketentuan di atas, kalian bebas untuk mengkreasikan
  program kalian.

  Nama  : Rizal Dwi Anggoro
  NIM   : L0122142
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NIM_SIZE 10
#define NAMA_SIZE 24
#define JENIS_KELAMIN_SIZE 8
#define IPK_SIZE 6

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define SPACER puts("")
#define FSPACER(filePtr) fprintf(filePtr, "\n")
#define SCAN(petunjuk, format, dest) \
  puts(petunjuk);                    \
  printf(">> ");                     \
  scanf(format, dest);
#define HORIZONTAL_LINE(num)      \
  for (int a = 0; a < num; a++) { \
    if (a < (num - 1))            \
      printf("-");                \
    else                          \
      puts("-");                  \
  }
#define FHORIZONTAL_LINE(filePtr, num) \
  for (int a = 0; a < num; a++) {      \
    if (a < (num - 1))                 \
      fprintf(filePtr, "-");           \
    else                               \
      fprintf(filePtr, "-\n");         \
  }

// todo: membuat enum pengurutan data
enum urutan_data {
  BY_NIM,
  BY_IPK,
};
typedef enum urutan_data UrutanData;

// todo: membuat enum jenis kelamin
enum jenis_kelamin {
  PRIA = 1,
  WANITA,
};

// todo: mengaliaskan enum jenis kelamin -> JenisKelamin
typedef enum jenis_kelamin JenisKelamin;

// todo: membuat struct mahasiswa
struct mahasiswa {
  char nim[NIM_SIZE];
  char nama[NAMA_SIZE];
  JenisKelamin jenisKelamin;
  float ipk;
};

// todo: mengaliaskan struct mahasiswa -> Mahasiswa
typedef struct mahasiswa Mahasiswa;
typedef Mahasiswa *MahasiswaPtr;

// todo: membuat struct menu_item untuk mencetak daftar menu
struct menu_item {
  char judul[32];
  void (*fungsi)();
};

// todo: mengaliaskan struct menu_item -> MenuItem
typedef struct menu_item MenuItem;

// global variabel
MahasiswaPtr mahasiswaPtr = NULL;
int totalMahasiswa = 0;
UrutanData urutanDataSaatIni = BY_NIM;

// fungsi prototipe
// <~> utama
void tambahMahasiswa();
void lihatMahasiswa();
void ubahMahasiswa();
void hapusMahasiswa();
void imporMahasiswa();
void eksporMahasiswa();
void tentangProgram();
void keluarProgram();
void menuUtama();

// <~> utilitas
int cetakMenu(MenuItem arrayMenu[], int n);
void hapusTerminal();
bool ubahMahasiswaPtr(int index, char nim[NIM_SIZE], char nama[NAMA_SIZE],
                      JenisKelamin jenisKelamin, float ipk);
void tambahMahasiswaPtr(char nim[NIM_SIZE], char nama[NAMA_SIZE],
                        JenisKelamin jenisKelamin, float ipk);
bool hapusMahasiswaPtr(int index);
void hapusSemuaMahasiswa();
bool initDatabase();
bool tulisDatabase();
void bacaDatabase();
MahasiswaPtr urutkanData(UrutanData urutanData);
// akhir::fungsi prototipe

// fungsi utama
int main() {
  hapusTerminal();

  // todo: melakukan init database
  if (initDatabase()) {
    // todo: membaca data mahasiswa dari database
    bacaDatabase();

    // todo: menampilkan data mahasiswa / menu utama
    lihatMahasiswa(urutkanData(urutanDataSaatIni));
  } else {
    // todo: tampilkan pesan error ketika gagal init database
    puts("Terjadi kesalahan dalam menyiapkan database...");
    return -1;
  }

  return 0;
}

void tambahMahasiswa() {
  hapusTerminal();
  puts("Tambah Data Mahasiswa\n");

  char nim[NIM_SIZE];
  char nama[NAMA_SIZE];
  int jenisKelamin;
  float ipk;

  // todo: meminta input kepada user
  SCAN("Masukkan NIM mahasiswa", " %[^\n]s", nim);
  SPACER;
  SCAN("Masukkan nama mahasiswa [maks: 24]", " %[^\n]s", nama);
  SPACER;

pilih_jenis_kelamin:
  puts("Pilih jenis kelamin mahasiswa");
  MenuItem arrayJenisKelamin[] = {
      {"Pria", NULL},
      {"Wanita", NULL},
  };
  jenisKelamin = cetakMenu(arrayJenisKelamin, ARRAY_SIZE(arrayJenisKelamin));
  SPACER;
  if (jenisKelamin == -1)
    // todo: memilih ulang ketika opsi tidak valid
    goto pilih_jenis_kelamin;

scan_ipk:
  SCAN("Masukkan IPK mahasiswa", "%f", &ipk);
  SPACER;
  // todo: validasi ipk
  if (ipk > 4) {
    // todo: input ulang ketika ipk tidak valid
    puts("IPK tidak valid!");
    goto scan_ipk;
  }

  puts("Konfirmasi Data Mahasiswa");
  printf("NIM           : %s\n", nim);
  printf("Nama          : %s\n", nama);
  printf("Jenis Kelamin : %s\n", jenisKelamin == PRIA ? "Pria" : "Wanita");
  printf("IPK           : %.2f\n", ipk);
  SPACER;

  // todo: melakukan konfirmasi data sebelum penambahan
  char konfirmasi;
  SCAN("Tambahkan data mahasiswa tersebut? [y/n]", " %c", &konfirmasi);

  if (konfirmasi == 'y') {
    // todo: menambah data mahasiswa ke mahasiswaPtr
    tambahMahasiswaPtr(nim, nama, jenisKelamin, ipk);

    // todo: menulis database / menyimpan mahasiswaPtr ke database
    if (tulisDatabase()) {
      // todo: kembali ke menu utama ketika berhasil
      lihatMahasiswa(urutkanData(urutanDataSaatIni));
    } else {
      // todo: ketika gagal, tampilkan pesan error
      puts("Gagal menambah data mahasiswa!");
    }
  } else {
    // todo: kembali ke menu utama, ketika tidak jadi menambah data
    lihatMahasiswa(urutkanData(urutanDataSaatIni));
  }
}

void lihatMahasiswa(MahasiswaPtr data) {
  hapusTerminal();

  // todo: menampilkan judul pada tengah
  char *title = "PROGRAM MANAJEMEN DATA MAHASISWA";
  char *title2 = "by rizalanggoro";
  int lineWidth = NIM_SIZE + NAMA_SIZE + JENIS_KELAMIN_SIZE + IPK_SIZE +
                  ((3 * 3) + (2 * 2));
  int titleLen = strlen(title);
  int titleLen2 = strlen(title2);
  int titleMargin = (lineWidth - titleLen) / 2;
  int titleMargin2 = (lineWidth - titleLen2) / 2;
  printf("%*s%s\n", titleMargin, "", title);
  printf("%*s%s\n", titleMargin2, "", title2);
  SPACER;
  SPACER;

  // todo: menampilkan urutan data saat ini
  printf("Berdasarkan (%s)\n", urutanDataSaatIni == BY_NIM ? "NIM" : "IPK");

  // todo: menampilkan tabel data mahasiswa
  HORIZONTAL_LINE(lineWidth);
  printf("| %-*s | %-*s | %-*s | %-*s |\n", NIM_SIZE, "NIM", NAMA_SIZE, "Nama",
         JENIS_KELAMIN_SIZE, "Kelamin", IPK_SIZE, "IPK");
  HORIZONTAL_LINE(lineWidth);

  if (totalMahasiswa > 0) {
    for (int a = 0; a < totalMahasiswa; a++) {
      Mahasiswa mahasiswa = data[a];

      printf("| %-*s | %-*s | %-*s | %*.2f |\n", NIM_SIZE, mahasiswa.nim,
             NAMA_SIZE, mahasiswa.nama, JENIS_KELAMIN_SIZE,
             mahasiswa.jenisKelamin == PRIA ? "Pria" : "Wanita", IPK_SIZE,
             mahasiswa.ipk);
    }
  } else {
    // todo: ketika tidak ada data, tampilkan tabel kosong
    printf("| %*s |\n", (lineWidth - 4), "");
  }

  HORIZONTAL_LINE(lineWidth);
  printf("| Total Mahasiswa = %3d %*s |\n", totalMahasiswa, (lineWidth - 26),
         "");
  HORIZONTAL_LINE(lineWidth);

  SPACER;

  // todo: menampikan opsi yang bisa dipilih
  puts("Opsi:");
  MenuItem arrayMenu[] = {
      {"Tambah data mahasiswa", &tambahMahasiswa},
      {"Lihat berdasarkan NIM", NULL},
      {"Lihat berdasarkan IPK", NULL},
      {"Ubah data mahasiswa", &ubahMahasiswa},
      {"Hapus data mahasiswa", &hapusMahasiswa},
      {"Hapus semua data mahasiswa", &hapusSemuaMahasiswa},
      {"Impor data mahasiswa", &imporMahasiswa},
      {"Ekspor data mahasiswa", &eksporMahasiswa},
      {"Tentang program", &tentangProgram},
      {"Keluar program", &keluarProgram},
  };
  int opsi = cetakMenu(arrayMenu, ARRAY_SIZE(arrayMenu));

  if (opsi == 2) {
    urutanDataSaatIni = BY_NIM;
    lihatMahasiswa(urutkanData(BY_NIM));
  } else if (opsi == 3) {
    urutanDataSaatIni = BY_IPK;
    lihatMahasiswa(urutkanData(BY_IPK));
  }
}

void ubahMahasiswa() {
  char nim[NIM_SIZE];

  SPACER;
  puts("Ubah Data Mahasiswa\n");

  // todo: meminta user untuk memasukkan nim mahasiswa
  SCAN("Masukkan NIM mahasiswa", " %[^\n]s", nim);
  SPACER;

  // todo: mencari nim pada mahasiswa ptr dengan perulangan
  int index = -1;
  for (int a = 0; a < totalMahasiswa; a++)
    // todo: melakukan validasi nim satu per satu
    if (strcmp(mahasiswaPtr[a].nim, nim) == 0) {
      // todo: ketika valid, catat index serta lakukan break
      index = a;
      break;
    }

  if (index == -1) {
    // todo: menampilkan pesan error ketika nim tidak ditemukan
    puts("NIM mahasiswa tidak ditemukan!");
  } else {
    puts("Detail Mahasiswa");

    // todo: mengambil data mahasiswa dari mahasiswaPtr ke index
    Mahasiswa mahasiswa = mahasiswaPtr[index];

    // todo: menampilkan detail data mahasiswa
    printf("NIM           : %s\n", mahasiswa.nim);
    printf("Nama          : %s\n", mahasiswa.nama);
    printf("Jenis Kelamin : %s\n",
           mahasiswa.jenisKelamin == PRIA ? "Pria" : "Wanita");
    printf("IPK           : %.2f\n", mahasiswa.ipk);
    SPACER;

    // todo: mengisi nilai default
    char nim[NIM_SIZE];
    strcpy(nim, mahasiswa.nim);
    char nama[NAMA_SIZE];
    strcpy(nama, mahasiswa.nama);
    int jenisKelamin = mahasiswa.jenisKelamin;
    float ipk = mahasiswa.ipk;

    // todo: melakukan konfirmasi untuk perubahan
    char konfirmasi;
    SCAN("Ubah NIM? [y/n]", " %c", &konfirmasi);
    SPACER;
    if (konfirmasi == 'y') {
      SCAN("Masukkan NIM mahasiswa", " %[^\n]s", nim);
      SPACER;
    }

    SCAN("Ubah nama? [y/n]", " %c", &konfirmasi);
    SPACER;
    if (konfirmasi == 'y') {
      SCAN("Masukkan nama mahasiswa [maks: 24]", " %[^\n]s", nama);
      SPACER;
    }

    SCAN("Ubah jenis kelamin? [y/n]", " %c", &konfirmasi);
    SPACER;
    if (konfirmasi == 'y') {
    pilih_jenis_kelamin:
      puts("Pilih jenis kelamin");
      MenuItem arrayJenisKelamin[] = {
          {"Pria", NULL},
          {"Wanita", NULL},
      };
      jenisKelamin =
          cetakMenu(arrayJenisKelamin, ARRAY_SIZE(arrayJenisKelamin));
      SPACER;
      if (jenisKelamin == -1)
        // todo: pilih ulang ketika opsi tidak valid
        goto pilih_jenis_kelamin;
    }

    SCAN("Ubah IPK? [y/n]", " %c", &konfirmasi);
    SPACER;
    if (konfirmasi == 'y') {
    masukkan_ipk:
      SCAN("Masukkan IPK mahasiswa", "%f", &ipk);
      SPACER;
      if (ipk > 4) {
        // todo: input ulang ketika opsi tidak valid
        puts("IPK tidak valid!");
        goto masukkan_ipk;
      }
    }

    puts("Konfirmasi Data");
    printf("NIM           : %s\n", nim);
    printf("Nama          : %s\n", nama);
    printf("Jenis Kelamin : %s\n", jenisKelamin == PRIA ? "Pria" : "Wanita");
    printf("IPK           : %.2f\n", ipk);
    SPACER;

    // todo: melakukan konfirmasi data baru
    SCAN("Apakah data tersebut sudah benar? [y/n]", " %c", &konfirmasi);
    SPACER;
    if (konfirmasi == 'y') {
      // todo: mengubah data mahasiswa sekaligus menulis database
      if (ubahMahasiswaPtr(index, nim, nama, jenisKelamin, ipk)) {
        // todo: kembali ke menu utama jika berhasil
        lihatMahasiswa(urutkanData(urutanDataSaatIni));
      } else {
        // todo: tampilkan pesan error ketika gagal
        puts("Gagal mengubah data mahasiswa!");
      }
    }
  }
}

void hapusSemuaMahasiswa() {
  puts("Hapus Database\n");

  // todo: melakukan konfirmasi untuk menghapus semua data mahasiswa
  char konfirmasi;
  SCAN("Apakah Anda yakin akan menghapus database? [y/n]", " %c", &konfirmasi);
  SPACER;
  if (konfirmasi == 'y') {
    // todo: mengosongkan totalMahasiswa dan mahasiswaPtr
    totalMahasiswa = 0;
    mahasiswaPtr = realloc(mahasiswaPtr, sizeof(Mahasiswa) * totalMahasiswa);

    // todo: menulis database
    if (tulisDatabase()) {
      // todo: ketika berhasil, kembali ke menu utama / menu lihat mahasiswa
      lihatMahasiswa(urutkanData(urutanDataSaatIni));
    } else {
      // todo: ketika gagal, tampilkan pesan error
      puts("Gagal menghapus database!");
    }
  }
}

void hapusMahasiswa() {
  char nim[NIM_SIZE];

  SPACER;
  puts("Hapus Data Mahasiswa\n");

  // todo: meminta user memasukkan nim mahasiswa
  SCAN("Masukkan NIM mahasiswa", " %[^\n]s", nim);
  SPACER;

  // todo: mencari nim pada mahasiswa ptr dengan perulangan
  int index = -1;
  for (int a = 0; a < totalMahasiswa; a++)
    // todo: melakukan validasi nim satu per satu
    if (strcmp(mahasiswaPtr[a].nim, nim) == 0) {
      // todo: ketika nim berhasil ditemukan, catat index & lakukan break
      index = a;
      break;
    }

  if (index == -1) {
    // todo: ketika nim tidak ditemukan, keluarkan pesan error
    puts("NIM mahasiswa tidak ditemukan!");
  } else {
    // todo: mengambil data mahasiswa dari mahasiswaPtr ke index
    Mahasiswa mahasiswa = mahasiswaPtr[index];

    // todo: menampilkan data / detail mahasiswa
    printf("NIM           : %s\n", mahasiswa.nim);
    printf("Nama          : %s\n", mahasiswa.nama);
    printf("Jenis Kelamin : %s\n",
           mahasiswa.jenisKelamin == PRIA ? "Pria" : "Wanita");
    printf("IPK           : %.2f\n", mahasiswa.ipk);
    SPACER;

    // todo: melakukan konfirmasi sebelum penghapusan data
    char konfirmasi;
    SCAN("Apakah Anda yakin akan menghapus data tersebut? [y/n]", " %c",
         &konfirmasi);
    SPACER;
    if (konfirmasi == 'y') {
      // todo: melakukan penghapusan data
      if (hapusMahasiswaPtr(index)) {
        // todo: ketika data berhasil dihapus dari database
        // todo: kembali ke menu utama / menu lihat mahasiswa
        lihatMahasiswa(urutkanData(urutanDataSaatIni));
      } else {
        // todo: ketika gagal, keluarkan pesan error
        puts("Gagal menghapus data mahasiswa!");
      }
    }
  }
}

void imporMahasiswa() {
  SPACER;
  puts("Impor Data Mahasiswa");
  SPACER;

  char namaFile[128];

  // todo: meminta user untuk memasukkan nama file yang akan diimpor
  SCAN("Masukkan nama file", " %[^\n]s", namaFile);
  SPACER;

  // todo: membuka file yang telah dimasukkan dengan izin read
  FILE *filePtr = fopen(namaFile, "r");
  if (filePtr == NULL) {
    return;
  }

  char data[128];
  int indexMahasiswa = 1;
  // todo: melakukan scan pada file
  while (fscanf(filePtr, " %[^\n]s", data) != EOF) {
    char nim[NIM_SIZE];
    char nama[NAMA_SIZE];
    int jenisKelamin;
    float ipk;

    // todo: melakukan split data yang telah dibaca
    // todo: dengan menggunakan delimiter ;
    char *token = strtok(data, ";");
    int index = 0;
    while (token != NULL) {
      // todo: melakukan parsing data hasil split
      if (index == 0) strcpy(nim, token);
      if (index == 1) strcpy(nama, token);
      if (index == 2)
        jenisKelamin =
            ((strcmp(token, "pria") == 0 || strcmp(token, "Pria") == 0 ||
              strcmp(token, "PRIA") == 0)
                 ? 1
                 : 2);
      if (index == 3) ipk = atof(token);
      token = strtok(NULL, ";");
      index++;
    }

    printf("Menambahkan data mahasiswa ke-%d\n", indexMahasiswa);
    printf("  NIM           : %s\n", nim);
    printf("  Nama          : %s\n", nama);
    printf("  Jenis kelamin : %s\n", jenisKelamin == PRIA ? "Pria" : "Wanita");
    printf("  IPK           : %.2f\n", ipk);

    // todo: menambahkan data nim, ... ke mahasiswaPtr
    tambahMahasiswaPtr(nim, nama, jenisKelamin, ipk);

    indexMahasiswa++;
  }
  fclose(filePtr);
  SPACER;

  // todo: menulis database / menyimpan data impor ke database
  if (tulisDatabase())
    // todo: ketika berhasil menulis database
    // todo: kembali ke menu utama / menu lihat mahasiswa
    lihatMahasiswa(urutkanData(urutanDataSaatIni));
  else
    puts("Gagal mengimpor data mahasiswa...");
}

void eksporMahasiswa() {
  SPACER;
  puts("Ekspor Data Mahasiswa");
  SPACER;

  // todo: nama file yang diekspor secara default adalah ekspor.txt
  char namaFile[128] = "ekspor.txt";
  char konfirmasi;

  printf("Ekspor data mahasiswa sebagai \"%s\"\n", namaFile);

  // todo: melakukan konfirmasi untuk perubahan nama file
  SCAN("Ubah nama file? [y/n]", " %c", &konfirmasi);
  SPACER;
  if (konfirmasi == 'y') {
    SCAN("Masukkan nama file", " %[^\n]s", namaFile);
    SPACER;
  }

  // todo: membuka namaFile dengan izin write
  FILE *filePtr = fopen(namaFile, "w");
  if (filePtr == NULL) return;

  // todo: menulis file
  char *title = "TABEL DATA MAHASISWA";
  int lineWidth = NIM_SIZE + NAMA_SIZE + JENIS_KELAMIN_SIZE + IPK_SIZE +
                  ((3 * 3) + (2 * 2));
  int titleLen = strlen(title);
  int titleMargin = (lineWidth - titleLen) / 2;
  fprintf(filePtr, "%*s%s\n", titleMargin, "", title);
  FSPACER(filePtr);
  FSPACER(filePtr);

  fprintf(filePtr, "Berdasarkan (%s)\n",
          urutanDataSaatIni == BY_NIM ? "NIM" : "IPK");

  FHORIZONTAL_LINE(filePtr, lineWidth);
  fprintf(filePtr, "| %-*s | %-*s | %-*s | %-*s |\n", NIM_SIZE, "NIM",
          NAMA_SIZE, "Nama", JENIS_KELAMIN_SIZE, "Kelamin", IPK_SIZE, "IPK");
  FHORIZONTAL_LINE(filePtr, lineWidth);

  if (totalMahasiswa > 0) {
    for (int a = 0; a < totalMahasiswa; a++) {
      Mahasiswa mahasiswa = mahasiswaPtr[a];

      fprintf(filePtr, "| %-*s | %-*s | %-*s | %*.2f |\n", NIM_SIZE,
              mahasiswa.nim, NAMA_SIZE, mahasiswa.nama, JENIS_KELAMIN_SIZE,
              mahasiswa.jenisKelamin == PRIA ? "Pria" : "Wanita", IPK_SIZE,
              mahasiswa.ipk);
    }
  } else {
    fprintf(filePtr, "| %*s |\n", (lineWidth - 4), "");
  }

  FHORIZONTAL_LINE(filePtr, lineWidth);
  fprintf(filePtr, "| Total Mahasiswa = %3d %*s |\n", totalMahasiswa,
          (lineWidth - 26), "");
  FHORIZONTAL_LINE(filePtr, lineWidth);

  fclose(filePtr);

  puts("Data mahasiswa berhasil diekspor...\n");
  puts("Opsi");
  MenuItem arrayMenu[] = {
      {"Kembali ke menu utama", &menuUtama},
      {"Keluar program", &keluarProgram},
  };
  cetakMenu(arrayMenu, ARRAY_SIZE(arrayMenu));

  // ! diubah menjadi opsi
  // // todo: kembali ke menu utama / menu lihat mahasiswa
  // lihatMahasiswa(urutkanData(urutanDataSaatIni));
}

void tentangProgram() {
  hapusTerminal();
  puts("PROGRAM MANAJEMEN DATA MAHASISWA");
  SPACER;
  puts("Dibuat oleh");
  puts("  Nama  : Rizal Dwi Anggoro");
  puts("  NIM   : L0122142");
  puts("  Kelas : D");
  puts("  Repo  : https://github.com/rizalanggoro/PKP-Responsi-2");

  SPACER;
  MenuItem menu[] = {
      {"Kembali ke menu utama", &menuUtama},
      {"Keluar program", &keluarProgram},
  };
  cetakMenu(menu, ARRAY_SIZE(menu));
}

void keluarProgram() {
  puts("\nKeluar program...");
  exit(0);
}

void menuUtama() { lihatMahasiswa(urutkanData(urutanDataSaatIni)); }

// fungsi utilitas
bool hapusMahasiswaPtr(int index) {
  // todo: menghitung total perulangan dan index awal
  int loopCount = totalMahasiswa - (index + 1);
  int startIndex = index;

  // todo: melakukan perulangan guna menggeser posisi data
  // todo: agar berada pada index yang tepat
  for (int a = 0; a < loopCount; a++) {
    mahasiswaPtr[a + startIndex] = mahasiswaPtr[a + index + 1];
  }

  // todo: mengurangi total mahasiswa dan
  // todo: mengalokasikan ulang mahasiswaPtr
  totalMahasiswa--;
  mahasiswaPtr = realloc(mahasiswaPtr, sizeof(Mahasiswa) * totalMahasiswa);

  // todo: mengembalikan bool dari fungsi tulis database
  return tulisDatabase();
}

bool ubahMahasiswaPtr(int index, char nim[NIM_SIZE], char nama[NAMA_SIZE],
                      JenisKelamin jenisKelamin, float ipk) {
  // todo: mengubah data mahasiswa ke index
  strcpy(mahasiswaPtr[index].nim, nim);
  strcpy(mahasiswaPtr[index].nama, nama);
  mahasiswaPtr[index].jenisKelamin = jenisKelamin;
  mahasiswaPtr[index].ipk = ipk;

  // todo: mengembalikan bool dari tulis database
  // todo: tulis database -> menulis mahasiswaPtr ke database.bin
  return tulisDatabase();
}

void tambahMahasiswaPtr(char nim[NIM_SIZE], char nama[NAMA_SIZE],
                        JenisKelamin jenisKelamin, float ipk) {
  // todo: mengalokasikan memori ketika mahasiswa ptr == null
  if (mahasiswaPtr == NULL) mahasiswaPtr = malloc(sizeof(Mahasiswa));

  // todo: mengalokasikan ulang mahasiswa ptr
  // todo: guna menampung totalMahasiswa + 1
  mahasiswaPtr =
      realloc(mahasiswaPtr, sizeof(Mahasiswa) * (totalMahasiswa + 1));

  // todo: memasukkan data nim, nama, j.kelamin, ipk
  // todo: pada index terakhir atau totalMahasiswa
  strcpy(mahasiswaPtr[totalMahasiswa].nim, nim);
  strcpy(mahasiswaPtr[totalMahasiswa].nama, nama);
  mahasiswaPtr[totalMahasiswa].jenisKelamin = jenisKelamin;
  mahasiswaPtr[totalMahasiswa].ipk = ipk;

  // todo: menambah total mahasiswa
  totalMahasiswa++;
}

bool initDatabase() {
  // todo: membuka file database.bin dengan izin rb
  FILE *filePtr = fopen("database.bin", "rb");
  if (filePtr == NULL) {
    // todo: ketika file tidak ditemukan
    // todo: membuat file database.bin dengan cara
    // todo: fopen dengan izin wb
    filePtr = fopen("database.bin", "wb");

    // todo: ketika gagal membuat file database.bin
    // todo: return false
    if (filePtr == NULL) return false;
  }

  fclose(filePtr);
  return true;
}

bool tulisDatabase() {
  // todo: membuka file database.bin
  FILE *filePtr = fopen("database.bin", "wb");
  if (filePtr == NULL) return false;

  // todo: menulis total data mahasiwa
  if (fwrite(&totalMahasiswa, sizeof(int), 1, filePtr) != 1) return false;

  // todo: menulis data mahasiswa
  if (fwrite(mahasiswaPtr, sizeof(Mahasiswa), totalMahasiswa, filePtr) !=
      totalMahasiswa)
    return false;

  fclose(filePtr);

  return true;
}

void bacaDatabase() {
  // todo: membuka file database.bin
  FILE *filePtr = fopen("database.bin", "rb");
  if (filePtr == NULL) return;

  // todo: membaca total mahasiswa
  if (fread(&totalMahasiswa, sizeof(int), 1, filePtr) != 1) return;

  // todo: mengalokasikan mahasiswaptr sesuai dengan
  // todo: jumlah mahasiswa
  mahasiswaPtr = malloc(sizeof(Mahasiswa) * totalMahasiswa);

  // todo: membaca data mahasiswa
  if (fread(mahasiswaPtr, sizeof(Mahasiswa), totalMahasiswa, filePtr) !=
      totalMahasiswa)
    return;

  fclose(filePtr);
}

MahasiswaPtr urutkanData(UrutanData urutanData) {
  // todo: fungsi untuk mengurutkan mahasiswa ptr
  // todo: menggunakan bubble sort
  for (int step = 0; step < totalMahasiswa - 1; step++) {
    for (int i = 0; i < totalMahasiswa - step - 1; i++) {
      // todo: pengkondisian urutan data (by nim, by ipk)
      if (urutanData == BY_NIM
              ? (strcmp(mahasiswaPtr[i].nim, mahasiswaPtr[i + 1].nim) > 0)
              : (mahasiswaPtr[i].ipk < mahasiswaPtr[i + 1].ipk)) {
        Mahasiswa temp = mahasiswaPtr[i];
        mahasiswaPtr[i] = mahasiswaPtr[i + 1];
        mahasiswaPtr[i + 1] = temp;
      }
    }
  }

  return mahasiswaPtr;
}

int cetakMenu(MenuItem arrayMenu[], int n) {
  // todo: mencetak menu yang terdapat dalam array
  for (int a = 0; a < n; a++) printf("%2d. %s\n", (a + 1), arrayMenu[a].judul);

  printf(">> ");
  int opsi;
  scanf("%d", &opsi);

  if (opsi > 0 && opsi < (n + 1)) {
    // todo: mengecek fungsi yang direferensikan pada menu item
    if (arrayMenu[(opsi - 1)].fungsi == NULL)
      // todo: jika tidak ada fungsi yang direferensikan,
      // todo: maka kembalikan opsi yang dimasukkan
      return opsi;
    else
      // todo: jika ada fungsi yang direferensikan
      // todo: maka jalankan fungsi tersebut
      arrayMenu[(opsi - 1)].fungsi();
  } else {
    // todo: jika opsi yang dimasukkan tidak valid
    // todo: keluar dari fungsi cetak menu
    puts("Opsi yang dimasukkan tidak valid!");
    return -1;
  }
}

void hapusTerminal() {
  // todo: menghapus terminal
  if (system("cls")) system("clear");
}
// akhir::fungsi utilitas