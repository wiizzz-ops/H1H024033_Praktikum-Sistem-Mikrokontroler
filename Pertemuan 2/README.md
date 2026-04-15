# Hasil dan Analisis Praktikum
## Modul 2: Kendali GPIO Pemrograman Input dan Output

### 1. Uraian hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan
- **Percobaan 2A (Seven Segment):** Rangkaian *Seven Segment* berhasil menampilkan siklus karakter berbasis heksadesimal, yaitu dari angka `0` hingga `9` dan dilanjutkan ke huruf `A` hingga `F`. Sistem bekerja secara sekuensial dan *looping* otomatis dengan memanfaatkan penulisan *array* dua dimensi untuk mengatur pin-pin GPIO mana saja yang harus diberi logika HIGH/LOW.
- **Percobaan 1B (Kontrol Counter):** Sistem *Seven Segment* berhasil diintegrasikan dengan *Push Button*. Angka pada layar tidak lagi berjalan otomatis, melainkan bertambah (inkremen) secara statis hanya ketika tombol ditekan. Implementasi fungsi *edge detection* berhasil mencegah terjadinya lompatan angka (*glitch*) saat tombol ditekan terlalu lama.

---

### 2. Prinsip kerja dari Seven Segment Display dalam menampilkan angka dan karakter
*Seven Segment Display* pada dasarnya adalah modul yang terdiri dari 7 buah lampu LED berbentuk batang (dilabeli pin `a` sampai `g`) dan 1 LED titik (`dp`). Prinsip kerjanya bergantung pada penyalaan kombinasi LED tersebut secara spesifik. 
Berdasarkan kode pemrograman yang digunakan, modul ini bertipe **Common Cathode (Katoda Bersama)**, di mana semua kutub negatif (GND) dari LED disatukan. Untuk menyalakan segmen tertentu, mikrokontroler harus mengirimkan sinyal digital `HIGH` (logika `1` pada *array* `digitPattern`). Contohnya, untuk membentuk angka `0`, mikrokontroler menyalakan segmen `a, b, c, d, e, f`, dan mematikan segmen `g`.

---

### 3. Penjelasan cara kerja sistem counter pada program
Sistem *counter* bekerja menggunakan deteksi perubahan status (*edge detection*) alih-alih deteksi status statis.
- Mikrokontroler terus membaca status *push button* melalui fungsi `digitalRead(btnUp)` dalam siklus `loop()`.
- Program memiliki variabel `lastUpState` (status tombol di sepersekian detik yang lalu) dan `upState` (status tombol saat ini).
- Syarat mutlak angka bertambah adalah struktur percabangan: `if(lastUpState == HIGH && upState == LOW)`. Ini berarti mikrokontroler mendeteksi momen *tepat saat* sakelar ditekan ke bawah (jatuh dari `HIGH` ke `LOW`).
- Jika syarat tersebut terpenuhi, variabel `currentDigit` ditambah 1 (`++`), lalu dikirimkan ke *Seven Segment*. Jika ditahan, angka tidak akan lari cepat karena kondisinya tidak bergeser dari `HIGH` ke `LOW` lagi.
- Jika variabel `currentDigit` sudah melampaui 15 (huruf F), mikrokontroler otomatis mereset nilainya kembali ke 0.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

### Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai 
penambahan (increment) dan pengurangan (decrement) pada sistem counter dan 
berikan penjelasan disetiap baris kode nya dalam bentuk README.md! 

// Mengimpor pustaka standar Arduino untuk mendukung fungsi dasar seperti GPIO.
#include <Arduino.h>

// Mendeklarasikan array pin digital Arduino yang terhubung ke segmen a, b, c, d, e, f, g, dan dp secara berurutan.
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Matriks dua dimensi yang menyimpan pola biner (logika 1/0) untuk membentuk karakter heksadesimal 0-F pada Seven Segment tipe Common Cathode.
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}, // 9
  {1,1,1,0,1,1,1,0}, // A
  {0,0,1,1,1,1,1,0}, // b
  {1,0,0,1,1,1,0,0}, // C
  {0,1,1,1,1,0,1,0}, // d
  {1,0,0,1,1,1,1,0}, // E
  {1,0,0,0,1,1,1,0}  // F
};

// Menetapkan Pin 2 sebagai pin input untuk tombol penambah angka (increment).
const int btnUp = 2;

// Menetapkan Pin 3 sebagai pin input untuk tombol pengurang angka (decrement).
const int btnDown = 3;

// Variabel untuk menyimpan nilai angka yang sedang ditampilkan saat ini (0-15).
int currentDigit = 0;

// Menyimpan status terakhir tombol Up untuk keperluan deteksi transisi (edge detection).
bool lastUpState = HIGH;

// Menyimpan status terakhir tombol Down untuk keperluan deteksi transisi.
bool lastDownState = HIGH;

// Fungsi bantuan untuk mengirimkan sinyal digital ke setiap pin segmen berdasarkan pola biner.
void displayDigit(int num) {
  for(int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

// Fungsi inisialisasi yang dijalankan satu kali saat mikrokontroler dinyalakan.
void setup() {
  // Mengatur semua pin yang terhubung ke segmen sebagai output menggunakan perulangan.
  for(int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Mengatur pin tombol sebagai input dengan mengaktifkan resistor pull-up internal agar status default adalah HIGH.
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);

  // Menampilkan angka awal (0) ke layar saat pertama kali menyala.
  displayDigit(currentDigit);
}

// Fungsi utama yang dijalankan terus-menerus oleh mikrokontroler (looping).
void loop() {
  // Membaca status logika saat ini pada pin tombol Up dan Down.
  bool upState = digitalRead(btnUp);
  bool downState = digitalRead(btnDown);

  // Logika Increment: Jika status tombol sebelumnya dilepas (HIGH) dan sekarang ditekan (LOW).
  if(lastUpState == HIGH && upState == LOW) {
    // Menambah nilai variabel angka sebanyak 1 poin.
    currentDigit++;
    
    // Logika batas atas: Jika angka melebihi 15 (karakter F), maka nilai di-reset kembali ke 0.
    if(currentDigit > 15) currentDigit = 0;
    
    // Mengirimkan nilai terbaru ke Seven Segment untuk ditampilkan.
    displayDigit(currentDigit);
    
    // Fungsi debounce sederhana untuk mencegah pembacaan ganda akibat getaran mekanis pada tombol.
    delay(50); 
  }

  // Logika Decrement: Jika status tombol sebelumnya dilepas (HIGH) dan sekarang ditekan (LOW).
  if(lastDownState == HIGH && downState == LOW) {
    // Mengurangi nilai variabel angka sebanyak 1 poin.
    currentDigit--;
    
    // Logika batas bawah: Jika angka kurang dari 0, maka nilai di-reset ke angka tertinggi yaitu 15 (F).
    if(currentDigit < 0) currentDigit = 15; 
    
    // Mengirimkan nilai terbaru ke Seven Segment untuk ditampilkan.
    displayDigit(currentDigit);
    
    // Fungsi debounce sederhana.
    delay(50); 
  }

  // Memperbarui variabel status terakhir dengan status tombol saat ini untuk evaluasi di iterasi berikutnya.
  lastUpState = upState;
  lastDownState = downState;
}