# 3.7 Pertanyaan Praktikum

1. Sebutkan dan jelaskan keuntungan dan kerugian menggunakan UART dan I2C!

    Protokol UART (Universal Asynchronous Receiver-Transmitter):

        • Keuntungan: Implementasinya sangat sederhana karena hanya membutuhkan dua jalur kabel (TX dan RX) dan tidak memerlukan jalur clock (detak sinkron). Sangat andal untuk komunikasi jarak menengah (misalnya via RS-232) atau komunikasi langsung ke PC/komputer (Serial Monitor).

        • Kerugian: Hanya mendukung topologi point-to-point (satu pengirim untuk satu penerima, tidak bisa di-branch). Selain itu, kecepatan baud rate antara kedua perangkat harus disetting sama persis; jika meleset sedikit saja, data yang diterima akan menjadi error (karakter sampah).

    Protokol I2C (Inter-Integrated Circuit):

        • Keuntungan: Sangat hemat pin mikrokontroler. Dengan topologi bus, hanya bermodalkan 2 kabel (SDA dan SCL), mikrokontroler dapat berkomunikasi dengan puluhan hingga ratusan modul/sensor yang berbeda secara bersamaan.

        • Kerugian: Arsitektur hardware dan software sedikit lebih rumit (membutuhkan resistor pull-up). Selain itu, karena kecepatannya dibatasi oleh kapasitansi kabel, I2C hanya ideal untuk komunikasi jarak sangat dekat (dalam satu papan atau rentang sentimeter/meter pendek) dan lebih rentan terhadap noise jika kabel terlalu panjang.

2. Bagaimana peran alamat I2C pada LCD (misalnya 0x27 vs 0x20)? Berikan penjelasan!

    Alamat I2C bertindak sebagai "Nomor Induk/Identitas Unik" bagi setiap perangkat keras (sebagai slave) yang terhubung pada jalur kabel I2C.
    Karena I2C menggunakan sistem bus (kabel SDA dan SCL dipakai secara paralel atau keroyokan oleh banyak perangkat sekaligus), mikrokontroler (master) membutuhkan cara untuk membedakan siapa yang sedang diajak bicara. Saat Arduino ingin mengirimkan teks ke LCD, ia akan mengawali pengiriman data dengan memanggil alamat 0x27 (atau 0x20). Akibatnya, hanya layar LCD dengan alamat 0x27 yang akan "menoleh", menerima data, dan menampilkan teks tersebut. Sementara itu, sensor-sensor lain yang terhubung di kabel yang sama (misal sensor suhu dengan alamat 0x76) akan mengabaikan paket data tersebut. Hal ini mencegah terjadinya tabrakan instruksi antar modul.

3. Jika UART dan I2C digabung dalam satu sistem (misalnya input dari Serial Monitor, output ke LCD), bagaimana alur kerja sistem tersebut? Bagaimana Arduino mengelola dua protokol sekaligus?

    Alur Kerja Sistem:

        1. Pengguna mengetikkan perintah/karakter di Serial Monitor komputer.
        2. Data tersebut dikirim via kabel USB dan ditangkap oleh pin RX Arduino menggunakan protokol UART.
        3. Program Arduino membaca input tersebut dari buffer memori menggunakan perintah Serial.read() dan menyimpannya ke dalam variabel.
        4. Setelah diproses oleh logika program (if/else), Arduino meneruskan hasil olahan data tersebut keluar melalui pin SDA dan SCL menggunakan protokol I2C (lcd.print) agar teks/informasi tersebut muncul di layar LCD.

    Cara Arduino Mengelolanya Sekaligus:
    Arduino dapat mengelolanya karena ia mendelegasikan tugas tersebut kepada dua blok hardware internal yang berbeda di dalam chip mikrokontrolernya. UART dikerjakan oleh modul hardware bawaan bernama USART, sedangkan I2C dikerjakan oleh modul antarmuka TWI (Two-Wire Interface).
    Keduanya memiliki sistem interrupt (interupsi) dan buffer (memori antrean) sendiri-sendiri. Sehingga, pada saat mikrokontroler sedang sibuk mencetak karakter ke layar LCD via I2C, perangkat keras USART tetap beroperasi di latar belakang untuk menampung ketikan baru dari Serial Monitor komputer, memastikan tidak ada paket data yang hilang atau tertukar.

    --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# 3.6.4 Pertanyaan Praktikum 

# 3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output)

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int pinPot = A0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  int nilaiADC = analogRead(pinPot);

  // Kalkulasi matematika untuk voltase dan persentase
  float voltase = (nilaiADC / 1023.0) * 5.0;
  int persen = map(nilaiADC, 0, 1023, 0, 100);
  int panjangBar = map(nilaiADC, 0, 1023, 0, 16);

  // OUTPUT KE SERIAL MONITOR (Protokol UART)
  // Menggunakan \t (tab) agar formatnya sejajar menyerupai tabel
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print("\t\tVolt: ");
  Serial.print(voltase);
  Serial.print(" V\t\tPersen: ");
  Serial.print(persen);
  Serial.println("%");

  // OUTPUT KE LAYAR LCD (Protokol I2C)
  // Baris 1: Menampilkan ADC dan Persen
  lcd.setCursor(0, 0);
  lcd.print("ADC: ");
  lcd.print(nilaiADC);
  lcd.print(" ");
  lcd.print(persen);
  lcd.print("%   "); // Spasi kosong untuk menghapus sisa karakter lama

  // Baris 2: Menampilkan Bar Level
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255); // Cetak kotak penuh
    } else {
      lcd.print(" ");       // Cetak spasi kosong
    }
  }

  delay(200); // Jeda pembacaan
}

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

# Penjelasan Kode: Modifikasi Pembacaan Potensiometer (UART & I2C)

Kode di bawah ini mengelola dua antarmuka protokol komunikasi secara bersamaan. Data pembacaan ADC mentah dari potensiometer dikonversi, lalu dikirim ke Serial Monitor menggunakan UART dan ke layar LCD menggunakan I2C.

#include <Wire.h> Mengimpor pustaka dasar untuk komunikasi I2C (Two-Wire Interface). 
#include <LiquidCrystal_I2C.h> Mengimpor pustaka khusus untuk mengendalikan modul *backpack* LCD I2C. 
LiquidCrystal_I2C lcd(0x27, 16, 2); Menginisialisasi objek layar LCD dengan alamat identitas I2C `0x27`, berukuran 16 kolom dan 2 baris. 
const int pinPot = A0; Menetapkan Pin Analog 0 (A0) sebagai jalur masuk data dari kaki tengah potensiometer. 
Serial.begin(9600); Mengaktifkan protokol UART untuk mengirim data ke komputer dengan kecepatan *baud rate* 9600 bps. 
lcd.init(); lcd.backlight(); Mengirim perintah bangun (*startup*) ke *chip* LCD dan menyalakan lampu latarnya. 
int nilaiADC = analogRead(pinPot); Fungsi inti ADC: mengubah voltase analog menjadi angka biner diskrit dengan rentang 0 hingga 1023. 
float voltase = (nilaiADC / 1023.0) * 5.0; Mengubah kembali nilai ADC ke skala tegangan aslinya (maksimal 5 Volt) dalam tipe desimal (*float*). 
int persen = map(nilaiADC, 0, 1023, 0, 100); Menyesuaikan batas skala awal 0-1023 menjadi skala persentase 0-100 menggunakan fungsi `map()`. 
Serial.print("ADC: "); ... Serial.println("%"); Blok pengiriman data menggunakan USART internal Arduino ke *Serial Monitor*. `\t` digunakan untuk memberi efek *tabulasi* agar tampilan di layar rapi memanjang ke samping layaknya tabel. 
lcd.setCursor(0, 0); Menempatkan titik mulai ketik (*cursor*) pada kolom 1 (indeks 0), baris 1 (indeks 0) LCD. 
lcd.print(nilaiADC); Menembakkan data angka ke layar LCD menggunakan jalur SDA dan SCL. 
lcd.setCursor(0, 1); Memindah posisi cetak ke kolom 1, baris 2 untuk mempersiapkan efek loading bar. 
for (int i = 0; i < 16; i++) { ... } Memulai perulangan 16 kali, mewakili 16 kotak karakter di baris kedua LCD. 
lcd.print((char)255); Jika posisi iterasi lebih kecil dari `panjangBar`, cetak ASCII bernilai 255 (berbentuk kotak balok penuh). 
