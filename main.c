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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NIM_SIZE 16
#define NAMA_SIZE 64

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define SPACER puts("")
#define SCAN(petunjuk, format, dest) \
  puts(petunjuk);                    \
  printf(">> ");                     \
  scanf(format, dest);

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
void tambahMahasiswaPtr(char nim[NIM_SIZE], char nama[NAMA_SIZE],
                        JenisKelamin jenisKelamin, float ipk);
void tulisDatabase();
void bacaDatabase();
// akhir::fungsi prototipe

// fungsi utama
int main() {
  hapusTerminal();

  puts("Manajemen Data Mahasiswa\n");

  MenuItem arrayMenu[] = {
      {"Tambah Mahasiswa", &tambahMahasiswa},       // todo: create
      {"Lihat Data Mahasiwa", &lihatMahasiswa},     // todo: read
      {"Ubah Data Mahasiswa", &ubahMahasiswa},      // todo: update
      {"Hapus Data Mahasiswa", &hapusMahasiswa},    // todo: delete
      {"Impor Data Mahasiswa", &imporMahasiswa},    // todo: import
      {"Ekspor Data Mahasiswa", &eksporMahasiswa},  // todo: export
      {"Tentang Program", &tentangProgram},
      {"Keluar Program", &keluarProgram},
  };
  cetakMenu(arrayMenu, ARRAY_SIZE(arrayMenu));

  return 0;
}

void tambahMahasiswa() {
  hapusTerminal();
  puts("Tambah Data Mahasiswa\n");

  char nim[NIM_SIZE];
  char nama[NAMA_SIZE];
  int jenisKelamin;
  float ipk;

  SCAN("Masukkan NIM mahasiswa", " %[^\n]s", nim);
  SPACER;
  SCAN("Masukkan nama mahasiswa", " %[^\n]s", nama);
  SPACER;

pilih_jenis_kelamin:
  puts("Pilih jenis kelamin mahasiswa");
  MenuItem arrayJenisKelamin[] = {
      {"Pria", NULL},
      {"Wanita", NULL},
  };
  jenisKelamin = cetakMenu(arrayJenisKelamin, ARRAY_SIZE(arrayJenisKelamin));
  SPACER;
  if (jenisKelamin == -1) goto pilih_jenis_kelamin;

scan_ipk:
  SCAN("Masukkan IPK mahasiswa", "%f", &ipk);
  SPACER;
  // todo: validasi ipk
  if (ipk > 4) {
    puts("IPK tidak valid!");
    goto scan_ipk;
  }

  puts("Konfirmasi Data Mahasiswa");
  printf("NIM           : %s\n", nim);
  printf("Nama          : %s\n", nama);
  printf("Jenis Kelamin : %s\n", jenisKelamin == PRIA ? "Pria" : "Wanita");
  printf("IPK           : %.2f\n", ipk);
  SPACER;

  puts("Tambahkan data tersebut?");
  MenuItem arrayKonfirmasi[] = {
      {"Ya", NULL},
      {"Ulangi", NULL},
      {"Batal", &menuUtama},
  };
  int respon = cetakMenu(arrayKonfirmasi, ARRAY_SIZE(arrayKonfirmasi));
  if (respon == 1) {
    // todo: tambahkan data ke database
    puts("ok");
  } else if (respon == 2)
    // todo: ulangi pengisian data
    tambahMahasiswa();
}

void lihatMahasiswa() {}

void ubahMahasiswa() {}

void hapusMahasiswa() {}

void imporMahasiswa() {
  hapusTerminal();
  puts("Impor Data Mahasiswa\n");

  char namaFile[128];

  SCAN("Masukkan nama file", " %[^\n]s", namaFile);
  SPACER;

  FILE *filePtr = fopen(namaFile, "r");
  if (filePtr == NULL) {
    return;
  }

  char data[128];
  int indexMahasiswa = 1;
  while (fscanf(filePtr, " %[^\n]s", data) != EOF) {
    char nim[NIM_SIZE];
    char nama[NAMA_SIZE];
    int jenisKelamin;
    float ipk;

    char *token = strtok(data, ";");
    int index = 0;
    while (token != NULL) {
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

    tambahMahasiswaPtr(nim, nama, jenisKelamin, ipk);

    indexMahasiswa++;
  }
  fclose(filePtr);

  for (int a = 0; a < totalMahasiswa; a++) {
    printf("%s\n", mahasiswaPtr[a].nama);
  }

  tulisDatabase();
}

void eksporMahasiswa() { bacaDatabase(); }

void tentangProgram() {}

void keluarProgram() {
  puts("\nKeluar program...");
  exit(0);
}

void menuUtama() { main(); }

// fungsi utilitas
void tambahMahasiswaPtr(char nim[NIM_SIZE], char nama[NAMA_SIZE],
                        JenisKelamin jenisKelamin, float ipk) {
  if (mahasiswaPtr == NULL) mahasiswaPtr = malloc(sizeof(Mahasiswa));

  realloc(mahasiswaPtr, sizeof(Mahasiswa) * (totalMahasiswa + 1));
  strcpy(mahasiswaPtr[totalMahasiswa].nim, nim);
  strcpy(mahasiswaPtr[totalMahasiswa].nama, nama);
  mahasiswaPtr[totalMahasiswa].jenisKelamin = jenisKelamin;
  mahasiswaPtr[totalMahasiswa].ipk = ipk;

  totalMahasiswa++;
}

void tulisDatabase() {
  FILE *filePtr = fopen("database", "a");
  if (filePtr == NULL) return;

  for (int a = 0; a < totalMahasiswa; a++) {
    Mahasiswa mahasiswa = mahasiswaPtr[a];
    fprintf(filePtr, "%s;%s;%s;%.2f\n", mahasiswa.nim, mahasiswa.nama,
            mahasiswa.jenisKelamin == PRIA ? "Pria" : "Wanita", mahasiswa.ipk);
  }

  // if (fwrite(mahasiswaPtr, sizeof(Mahasiswa), totalMahasiswa, filePtr) ==
  //     totalMahasiswa)
  //   puts("Berhasil menulis database");
  // else
  //   puts("Gagal menulis database");

  fclose(filePtr);
}

void bacaDatabase() {
  FILE *filePtr = fopen("database", "r");
  if (filePtr == NULL) return;

  char data[128];
  while (fscanf(filePtr, " %[^\n]s", data) != EOF) {
    printf("%s\n", data);
  }

  fclose(filePtr);
}

int cetakMenu(MenuItem arrayMenu[], int n) {
  // todo: mencetak menu yang terdapat dalam array
  for (int a = 0; a < n; a++) printf("%d. %s\n", (a + 1), arrayMenu[a].judul);

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
  if (system("cls")) system("clear");
}
// akhir::fungsi utilitas