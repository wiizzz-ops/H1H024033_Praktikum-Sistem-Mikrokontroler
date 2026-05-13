### Jawaban Pertanyaan Praktikum (5.7)

**1. Jelaskan perbedaan antara `loop()` pada Arduino biasa dengan sistem yang menggunakan RTOS!**
* [cite_start]**Arduino Biasa (Non-OS):** Fungsi `loop()` berjalan secara sekuensial (berurutan) dari atas ke bawah dan mengeksekusinya berulang-ulang tanpa henti[cite: 348]. [cite_start]Pada sistem ini, jika ada instruksi yang memakan waktu lama atau menggunakan fungsi *blocking* seperti `delay()`, maka **seluruh sistem akan terhenti sementara** dan tidak bisa mengerjakan tugas lain[cite: 350].
* **Sistem RTOS:** Program tidak lagi bertumpu pada satu `loop()` raksasa. [cite_start]Sebagai gantinya, program dipecah menjadi beberapa *task* kecil yang mandiri, di mana setiap *task* memiliki perulangan `while(1)`-nya masing-masing[cite: 370]. [cite_start]Eksekusi *task* diatur oleh *Kernel Scheduler* RTOS[cite: 372]. [cite_start]Jika satu *task* sedang tertidur (`vTaskDelay`), CPU akan langsung melompat mengerjakan *task* lain, sehingga sistem berjalan secara *multitasking* dan bebas hambatan (non-blocking)[cite: 336].

**2. Mengapa fungsi `loop()` dibiarkan kosong?**
[cite_start]Dalam pemrograman Arduino yang menggunakan FreeRTOS, setelah kita memanggil fungsi `vTaskStartScheduler()` di dalam blok `setup()`, kontrol penuh CPU mikrokontroler langsung diambil alih oleh sistem operasi RTOS[cite: 469]. [cite_start]Pada arsitektur ini, fungsi `loop()` bawaan Arduino akan secara otomatis diturunkan fungsinya menjadi **Idle Task**[cite: 352, 362]. *Idle task* ini hanya akan dieksekusi oleh sistem apabila semua *task* utama buatan kita sedang dalam status menunggu/tertidur (*Blocked*). Oleh karena itu, `loop()` sengaja dibiarkan kosong agar tidak memboroskan siklus komputasi (*CPU cycles*) dan tidak mengacaukan jadwal *task* utama.

**3. Apa insight utama yang Anda pelajari dari praktikum ini?**
*Insight* utama dari praktikum ini adalah pemahaman bahwa pembuatan sistem *embedded* berskala kompleks tidak bisa lagi hanya mengandalkan logika pemrograman tradisional (sekuensial). [cite_start]Penggunaan RTOS sangat vital untuk membangun aplikasi yang membutuhkan respons seketika (*real-time*)[cite: 334, 335]. Melalui praktikum ini, terbukti bahwa dengan *Task Scheduler*, beberapa perangkat keras (seperti aktuator LED dan pencetakan data Serial) dapat dikendalikan seolah-olah secara bersamaan tanpa saling mengganggu. [cite_start]Selain itu, penggunaan fitur *Queue* memberikan wawasan baru tentang betapa pentingnya menjaga lalu lintas memori yang aman agar pertukaran data antar-*task* tidak bertabrakan (*race condition*)[cite: 569, 576].

---
# Modifikasi Kode Praktikum RTOS
## Modul 5: Multitasking dan Komunikasi Task

---

### 1. Modifikasi Percobaan 5A (Menambah Sensor Potensiometer)
**Tujuan:** Menggunakan nilai potensiometer untuk mengontrol kecepatan kedip salah satu LED secara *real-time* tanpa mengganggu jadwal *task* yang lain.

```cpp
// Mengimpor library utama FreeRTOS untuk Arduino
#include <Arduino_FreeRTOS.h>

// Mendeklarasikan purwarupa (prototype) untuk ketiga fungsi task
void TaskBlinkPotensio(void *pvParameters);
void TaskBlinkTetap(void *pvParameters);
void TaskPrint(void *pvParameters);

// Fungsi inisialisasi yang dieksekusi satu kali
void setup() {
  // Membuka jalur komunikasi serial dengan komputer pada baud rate 9600
  Serial.begin(9600);

  // Mendaftarkan Task 1 ke Kernel Scheduler: Kontrol LED dengan Potensiometer
  xTaskCreate(TaskBlinkPotensio, "Blink_Variabel", 128, NULL, 1, NULL);
  
  // Mendaftarkan Task 2 ke Kernel Scheduler: Kontrol LED dengan kecepatan tetap
  xTaskCreate(TaskBlinkTetap, "Blink_Konstan", 128, NULL, 1, NULL);
  
  // Mendaftarkan Task 3 ke Kernel Scheduler: Mencetak log ke Serial Monitor
  xTaskCreate(TaskPrint, "Print_Monitor", 128, NULL, 1, NULL);
}

// Fungsi perulangan utama bawaan Arduino
void loop() {
  // Sengaja dibiarkan kosong karena kontrol CPU telah diambil alih oleh FreeRTOS (menjadi Idle Task)
}

// ===================================================================
// TASK 1: LED Berkedip sesuai nilai Potensiometer
// ===================================================================
void TaskBlinkPotensio(void *pvParameters) {
  // Menetapkan Pin 10 sebagai jalur output tegangan untuk LED Merah
  pinMode(10, OUTPUT); 
  
  // Perulangan tak terbatas khusus untuk Task 1
  while (1) {
    // Membaca besaran tegangan fisik dari potensiometer menjadi angka digital (0-1023)
    int potValue = analogRead(A0); 
    
    // Mengonversi skala nilai ADC (0-1023) menjadi skala waktu tunda dalam milidetik (100ms - 2000ms)
    int delayTime = map(potValue, 0, 1023, 100, 2000); 

    // Mengalirkan arus agar LED menyala
    digitalWrite(10, HIGH);
    
    // Menidurkan task ini selama hasil kalkulasi waktu tunda (membebaskan CPU untuk task lain)
    vTaskDelay(delayTime / portTICK_PERIOD_MS); 
    
    // Memutus arus agar LED mati
    digitalWrite(10, LOW);
    
    // Menidurkan task kembali
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
  }
}

// ===================================================================
// TASK 2: LED Berkedip dengan durasi konstan
// ===================================================================
void TaskBlinkTetap(void *pvParameters) {
  // Menetapkan Pin 8 sebagai jalur output tegangan untuk LED Kuning
  pinMode(8, OUTPUT); 
  
  // Perulangan tak terbatas khusus untuk Task 2
  while (1) {
    // Mengalirkan arus agar LED menyala
    digitalWrite(8, HIGH);
    
    // Menidurkan task dengan durasi mutlak dan konstan selama 300 milidetik
    vTaskDelay(300 / portTICK_PERIOD_MS); 
    
    // Memutus arus agar LED mati
    digitalWrite(8, LOW);
    
    // Menidurkan task kembali
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// ===================================================================
// TASK 3: Print status ke Serial Monitor
// ===================================================================
void TaskPrint(void *pvParameters) {
  // Variabel lokal untuk menyimpan jumlah siklus perhitungan
  int counter = 0;
  
  // Perulangan tak terbatas khusus untuk Task 3
  while (1) {
    // Menambah nilai variabel perhitungan sebanyak 1 angka setiap siklus
    counter++;
    
    // Mencetak teks indikator ke layar monitor
    Serial.print("System Running... Counter: ");
    
    // Mencetak nilai angka perhitungan terkini
    Serial.println(counter);
    
    // Menidurkan task pencetakan ini secara konstan selama 1000 milidetik (1 detik)
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
  }
}

---
# Modifikasi Percobaan 5B (Menggunakan Sensor DHT11 Asli pada Queue)
// Mengimpor library utama FreeRTOS untuk Arduino
#include <Arduino_FreeRTOS.h>

// Mengimpor library khusus untuk mengelola antrean (Queue) antar-task
#include <queue.h>

// Mengimpor library untuk membaca sensor fisik DHT
#include <DHT.h> 

// Menetapkan Pin 2 digital sebagai jalur data komunikasi dari sensor DHT
#define DHTPIN 2     

// Mendefinisikan tipe sensor spesifik yang dipakai (DHT11)
#define DHTTYPE DHT11 

// Membuat objek sensor bernama 'dht' berdasarkan pin dan tipe yang telah didefinisikan
DHT dht(DHTPIN, DHTTYPE);

// Mendefinisikan kerangka paket data (struct) untuk membungkus variabel suhu dan kelembaban sekaligus
struct readings {
  float temp;
  float h;
};

// Mendeklarasikan variabel pegangan (handle) untuk melacak memori antrean yang akan dibuat
QueueHandle_t my_queue;

// Fungsi inisialisasi yang dieksekusi satu kali
void setup() {
  // Membuka jalur komunikasi serial ke monitor komputer
  Serial.begin(9600);
  
  // Membangunkan dan menginisialisasi chip pada sensor DHT
  dht.begin(); 

  // Mengalokasikan memori antrean (Queue) dengan kapasitas maksimum 1 paket data bertipe 'struct readings'
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Mendaftarkan Task Pembaca Sensor ke Kernel Scheduler
  xTaskCreate(read_data, "Read_Sensor", 128, NULL, 1, NULL);
  
  // Mendaftarkan Task Penampil Layar ke Kernel Scheduler
  xTaskCreate(display, "Display_Monitor", 128, NULL, 1, NULL);
}

// Fungsi perulangan utama dibiarkan kosong (diambil alih Idle Task)
void loop() {}

// ===================================================================
// TASK 1: Membaca sensor dunia nyata dan mendorongnya ke Queue
// ===================================================================
void read_data(void *pvParameters) {
  // Membuat variabel bungkus lokal 'x' dari cetakan struct 'readings'
  struct readings x; 
  
  // Perulangan tak terbatas khusus untuk Task 1
  for (;;) {
    // Menyerap data suhu (Celcius) dari udara sekitar menggunakan sensor DHT
    x.temp = dht.readTemperature(); 
    
    // Menyerap data kelembaban udara (%) menggunakan sensor DHT
    x.h = dht.readHumidity(); 

    // Mengecek apakah data yang ditarik adalah angka valid (bukan NaN/Not a Number)
    if (!isnan(x.temp) && !isnan(x.h)) {
      
      // Mengirimkan salinan data struct 'x' ke dalam memori antrean 'my_queue' secara aman
      // portMAX_DELAY menginstruksikan task untuk menunggu dengan sabar jika memori antrean sedang penuh
      xQueueSend(my_queue, &x, portMAX_DELAY);
    }
    
    // Memberikan jeda waktu 2000 milidetik (2 detik) karena sensor fisik DHT11 sangat lambat merespons perubahan
    vTaskDelay(2000 / portTICK_PERIOD_MS); 
  }
}

// ===================================================================
// TASK 2: Menarik data dari Queue dan mencetaknya
// ===================================================================
void display(void *pvParameters) {
  // Membuat variabel bungkus lokal 'x' sebagai wadah penerima
  struct readings x; 
  
  // Perulangan tak terbatas khusus untuk Task 2
  for (;;) {
    
    // Mengecek dan menarik keluar paket data dari 'my_queue' ke dalam wadah 'x'
    // pdPASS memastikan bahwa proses pencetakan hanya dieksekusi jika data benar-benar berhasil ditarik dari antrean
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      
      // Mencetak teks label Suhu
      Serial.print("Data Real-Time -> Suhu: ");
      
      // Mencetak nilai suhu yang berhasil diekstrak dari struct
      Serial.print(x.temp);
      
      // Mencetak teks label Kelembaban
      Serial.print(" C \t Kelembaban: ");
      
      // Mencetak nilai kelembaban yang berhasil diekstrak dari struct
      Serial.print(x.h);
      
      // Mengakhiri baris cetakan
      Serial.println(" %");
    }
  }
}