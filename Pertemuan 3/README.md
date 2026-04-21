3.7 Pertanyaan Praktikum

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