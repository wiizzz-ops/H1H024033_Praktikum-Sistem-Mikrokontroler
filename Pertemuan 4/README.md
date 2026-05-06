# Hasil dan Analisis Praktikum
## Modul 4: Analog to Digital Converter (ADC) dan Pulse Width Modulation (PWM)

---

### Jawaban Pertanyaan Praktikum

**1. Bagaimana proses konversi sinyal analog menjadi digital pada Arduino? Jelaskan!**
Proses konversi pada Arduino dilakukan oleh sirkuit internal mikrokontroler bernama ADC (*Analog to Digital Converter*). Saat fungsi `analogRead()` dipanggil, sirkuit *sample and hold* di dalam *chip* akan mengambil sampel tegangan input (biasanya rentang 0-5V). Komparator kemudian mengkuantisasi tegangan tersebut berdasarkan tegangan referensi dan mengubahnya menjadi nilai digital diskrit. Karena Arduino Uno menggunakan arsitektur ADC 10-bit, maka tegangan 0 hingga 5 Volt tersebut dipetakan menjadi 1024 tingkatan nilai, yang direpresentasikan dalam angka 0 hingga 1023.

**2. Faktor apa saja yang dapat mempengaruhi keakuratan pembacaan ADC?**
Beberapa faktor yang sangat memengaruhi keakuratan ADC antara lain:
- **Stabilitas Tegangan Referensi (Vref):** Jika suplai daya (5V) dari USB atau baterai berfluktuasi, nilai referensi ADC akan ikut berubah, sehingga hasil pembacaan menjadi tidak akurat.
- **Kualitas Komponen (*Hardware*):** Potensiometer yang aus, kotor, atau murahan dapat menghasilkan lonjakan resistansi mekanis yang membuat data melompat-lompat.
- **Derau Elektromagnetik (*Noise*):** Penggunaan kabel jumper yang terlalu panjang atau *breadboard* yang tidak rapat dapat menangkap interferensi sinyal dari lingkungan sekitar.
- **Impedansi Sumber:** Jika sensor analog memiliki impedansi (hambatan) keluaran yang terlalu tinggi, kapasitor ADC internal tidak bisa terisi penuh secara ideal saat proses *sampling*.

**3. Apa kendala yang mungkin terjadi saat mengintegrasikan ADC dan PWM dalam satu sistem?**
Kendala utama saat menggabungkan keduanya adalah **Fluktuasi Sinyal (*Jitter/Flickering*)**. Sensor analog sangat sensitif terhadap perubahan kecil atau derau (*noise*). Ketika ADC membaca nilai yang terus berubah-ubah (misalnya naik-turun antara angka 500 dan 502 meskipun potensiometer tidak disentuh), perubahan ini akan langsung diterjemahkan ke output PWM. Akibatnya, aktuator tidak stabil; motor servo akan terus bergetar sendiri atau cahaya LED akan tampak berkedut. Solusinya adalah dengan menambahkan interval pemrosesan (seperti `delay()`) atau membuat algoritma pemfilteran nilai rata-rata (*smoothing/moving average*) di dalam kode.

---

### Modifikasi Program (Percobaan 1 & Percobaan 2)

#### Modifikasi Percobaan 1: ADC dan Pembatasan Rentang Sudut Servo
**Tujuan Modifikasi:** Membatasi pergerakan servo agar hanya bergerak dalam rentang 30° hingga 150°, meskipun rentang potensiometer tetap 0–1023.
**Penjelasan:** Modifikasi dilakukan secara matematis menggunakan fungsi `map()`. Parameter output minimum diubah menjadi 30, dan parameter maksimum diubah menjadi 150.

#include <Servo.h> 

Servo myservo; 

// Mendefinisikan pin A0 untuk input pembacaan potensiometer
const int potensioPin = A0;   
// Mendefinisikan Pin 9 (PWM) sebagai pin output sinyal data untuk servo
const int servoPin = 9;      

// Deklarasi variabel untuk menyimpan posisi sudut servo dan data ADC
int pos = 0; 
int val = 0; 

void setup() {
  myservo.attach(servoPin); // Menyambungkan objek servo ke Pin 9
  Serial.begin(9600);       // Menginisialisasi komunikasi Serial
}

void loop() {
  // Merekam voltase analog dari potensiometer lalu diubah ke nilai diskrit 0-1023
  val = analogRead(potensioPin); 

  // ===================== MODIFIKASI =====================
  // Mengonversi data 0-1023 ke rentang derajat terbatas, yaitu 30 sampai 150 derajat
  pos = map(val, 0, 1023, 30, 150); 
  // ======================================================

  // Mengeksekusi pergerakan motor servo menuju titik derajat hasil konversi
  myservo.write(pos); 

  // Mencetak log variabel ke Serial Monitor 
  Serial.print("ADC Potensio: ");
  Serial.print(val); 
  Serial.print(" | Sudut Servo: ");
  Serial.println(pos); 

  delay(50); // Jeda stabilitas
}

---

### Modifikasi Percobaan 2: PWM dan Rentang Spesifik Kecerahan LED
**Tujuan Modifikasi:** LED hanya menyala pada rentang kecerahan sedang, yaitu ketika nilai PWM berada pada rentang 50 sampai 200. Jika di luar rentang itu, LED mati.
**Penjelasan:** Modifikasi ditambahkan menggunakan struktur kontrol kondisional if...else. Program akan mengevaluasi nilai variabel pwm. Jika nilai berada di dalam parameter (>= 50 dan <= 200), LED diaktifkan dengan fungsi analogWrite. Jika tidak, LED diberi instruksi mati paksa (analogWrite(ledPin, 0)).

#include <Arduino.h> 

// Menetapkan A0 sebagai input tegangan variabel (sensor) dan Pin 3 sebagai output LED
const int potPin = A0;   
const int ledPin = 3;   

// Variabel default untuk menyimpan nilai konversi resolusi
int nilaiADC = 0;  
int pwm = 0;       

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  // Menyerap data voltase analog menjadi skala digital 0-1023
  nilaiADC = analogRead(potPin); 

  // Pemetaan linier menurunkan resolusi skala ke 0-255 agar kompatibel dengan register PWM
  pwm = map(nilaiADC, 0, 1023, 0, 255);  

  // ===================== MODIFIKASI =====================
  // Jika intensitas PWM berada di rentang 50 hingga 200 (inklusif)
  if (pwm >= 50 && pwm <= 200) {
    analogWrite(ledPin, pwm); // LED menyala sesuai proporsi
  } else {
    analogWrite(ledPin, 0);   // Jika di luar rentang, LED dimatikan paksa
  }
  // ======================================================

  // Menampilkan variabel ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC); 
  Serial.print(" | PWM: ");
  Serial.println(pwm); 

  delay(50); // Jeda stabilitas
}