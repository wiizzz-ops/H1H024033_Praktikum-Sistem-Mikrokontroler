1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!
   
   Hasil dari praktikum ini membuktikan bahwa program berjalan sesuai dengan spesifikasi.
   Pada Percobaan 1A (Percabangan): Dihasilkan sebuah sistem di mana satu buah LED mengalami perubahan kecepatan kedip.
   LED menyala dengan jeda waktu yang semakin cepat (karena nilai timeDelay terus dikurangi di dalam blok else).
   Ketika batas waktu tertentu tercapai ( $\le 100$ ms), program berhasil masuk ke blok if untuk mereset durasi tunda kembali ke kondisi awal (1000 ms) setelah memberikan jeda 3 detik.
   Pada Percobaan 2A (Perulangan): Dihasilkan sistem running LED menggunakan susunan beberapa lampu. Rangkaian lampu berhasil menyala secara berurutan dari kiri ke kanan, lalu berbalik arah dari kanan ke kiri tanpa terputus. Hal ini menunjukkan bahwa iterasi penambahan dan pengurangan nilai pin pada fungsi for berjalan dengan presisi.
2. Bagaimana pengaruh penggunaan struktur perulangan (seperti for dan while) terhadap jalannya program pada Arduino?

   Penggunaan struktur perulangan sangat berpengaruh terhadap efisiensi memori dan kebersihan penulisan kode (clean code). Daripada menuliskan instruksi manual yang sama berulang kali (misalnya mendeklarasikan digitalWrite secara satu per satu untuk 6 pin yang berbeda), struktur for atau while memungkinkan instruksi tersebut diringkas ke dalam satu blok kode iteratif. Selain itu, perulangan memastikan jalannya program dapat mengeksekusi tugas yang bersifat sekuensial atau terus-menerus tanpa menghentikan sistem secara keseluruhan.

3. Bagaimana cara kerja percabangan (if-else) dalam menentukan kondisi LED menyala atau mati berdasarkan input yang diberikan?
  
   Percabangan if-else bekerja sebagai pembuat keputusan (decision maker) berdasarkan evaluasi logika. Program akan memeriksa apakah sebuah kondisi atau input bernilai benar (true) atau salah (false). Jika kondisi batas terpenuhi (misal: variabel memicu batas bawah atau sensor memberikan sinyal HIGH), maka mikrokontroler akan mengeksekusi perintah di dalam blok if (seperti menyalakan LED atau mereset variabel). Namun, jika kondisi tersebut tidak terpenuhi, mikrokontroler akan secara otomatis mengabaikan blok if dan beralih menjalankan instruksi alternatif yang berada di dalam blok else (seperti mematikan LED atau mengurangi nilai variabel).

4. Bagaimana kombinasi antara perulangan dan percabangan dapat digunakan untuk membuat sistem Arduino yang responsif terhadap perubahan kondisi lingkungan?
  
   Kombinasi keduanya adalah pondasi utama dalam menciptakan sistem tertanam (embedded system) yang otomatis dan real-time. Struktur perulangan (seperti fungsi bawaan void loop() atau while) memastikan Arduino terus terjaga untuk membaca input atau memantau sensor secara terus-menerus tanpa henti. Di dalam siklus pantauan tersebut, struktur percabangan (if-else) disisipkan untuk mengevaluasi data yang masuk secara langsung. Sehingga, ketika terjadi perubahan kondisi lingkungan (misalnya batas parameter sensor tercapai), percabangan akan langsung memicu tindakan spesifik (menyalakan/mematikan aktuator/LED) tanpa harus mematikan alur pemantauan utamanya.
