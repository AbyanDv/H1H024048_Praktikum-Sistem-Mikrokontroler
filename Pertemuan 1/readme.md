1.5.4 Pertanyaan Praktikum:

1. Pada kondisi apa program masuk ke blok if?
2. Pada kondisi apa program masuk ke blok else?
3. Apa fungsi dari perintah delay(timeDelay)?
4. Jika program yang dibuat memiliki alur mati → lambat → cepat → reset (mati),
   ubah menjadi LED tidak langsung reset → tetapi berubah dari cepat → sedang →
   mati dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

Jawab :

1. Program masuk ke blok if ketika nilai variabel timeDelay kurang dari atau sama dengan 100 (timeDelay <= 100). Kondisi ini menandakan bahwa LED telah mencapai kecepatan kedip maksimum, sehingga program memberikan jeda 3000 ms sebelum mereset timeDelay kembali ke nilai awal 1000 ms.

2. Program masuk ke blok else ketika nilai timeDelay masih lebih besar dari 100. Pada kondisi ini, program mengurangi nilai timeDelay sebesar 100 ms setiap satu siklus kedip, sehingga LED berkedip semakin cepat secara bertahap.

3. Fungsi delay(timeDelay) berfungsi menghentikan eksekusi program sementara selama durasi yang ditentukan oleh nilai variabel timeDelay dalam satuan milidetik. Fungsi ini bersifat blocking, artinya tidak ada instruksi lain yang dijalankan selama waktu tunggu berlangsung. Penggunaan variabel sebagai argumen memungkinkan durasi jeda berubah secara dinamis setiap siklus, yang menjadi dasar efek percepatan kedipan LED.

4. Perubahan dilakukan dengan menambahkan satu fase tambahan setelah kecepatan maksimum tercapai, yaitu memperlambat kembali timeDelay secara bertahap hingga LED berhenti, sebelum akhirnya mereset ke awal.

const int ledPin = 6;
int timeDelay = 1000;
bool percepat = true; // true = mempercepat, false = memperlambat

void setup() {
pinMode(ledPin, OUTPUT);
}

void loop() {
// Nyalakan LED
digitalWrite(ledPin, HIGH);
delay(timeDelay);

// Matikan LED
digitalWrite(ledPin, LOW);
delay(timeDelay);

if (percepat) {
// Fase percepatan: kurangi delay hingga 100ms
if (timeDelay > 100) {
timeDelay -= 100; // percepat bertahap
} else {
percepat = false; // beralih ke fase perlambatan
}
} else {
// Fase perlambatan: tambah delay hingga 1000ms lalu reset
if (timeDelay < 1000) {
timeDelay += 100; // perlambat bertahap
} else {
percepat = true; // reset ke fase percepatan
}
}
}

1.6.4 Pertanyaan Praktikum

1. Gambarkan rangkaian schematic 5 LED running yang digunakan pada percobaan!
2. Jelaskan bagaimana program membuat efek LED berjalan dari kiri ke kanan!
3. Jelaskan bagaimana program membuat LED kembali dari kanan ke kiri!
4. Buatkan program agar LED menyala tiga LED kanan dan tiga LED kiri secara bergantian
   dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

Jawab :

1. https://www.tinkercad.com/things/jzW3OVjzFlD-pertemuan-1-praktikum-sismik?sharecode=cZulcZdu-5LWgXwWYoFCszb-Ov3X0ICnXHGH5dvOeyw

2. Program menggunakan for loop pertama dengan variabel ledPin yang dimulai dari nilai 2 dan bertambah satu setiap iterasi hingga mencapai 7. Pada setiap iterasi, pin yang aktif dinyalakan dengan digitalWrite(ledPin, HIGH), ditunggu selama timer milidetik, lalu dimatikan kembali dengan digitalWrite(ledPin, LOW) sebelum iterasi berpindah ke pin berikutnya. Hasilnya, LED menyala satu per satu dari kiri (Pin 2) ke kanan (Pin 7) secara berurutan.

3. Program menggunakan for loop kedua dengan variabel ledPin yang dimulai dari nilai 7 dan berkurang satu setiap iterasi (ledPin--) hingga mencapai 2. Logika nyala dan mati LED identik dengan loop pertama, namun arah iterasi yang terbalik menyebabkan LED menyala satu per satu dari kanan (Pin 7) kembali ke kiri (Pin 2), menciptakan efek gerakan bolak-balik yang berkesinambungan.

4. Berikut kodenya:
   // LED kiri : Pin 2, 3, 4
   // LED kanan: Pin 5, 6, 7
   int timer = 500; // durasi nyala setiap kelompok (ms)

void setup() {
// Inisialisasi Pin 2 hingga 7 sebagai output
for (int ledPin = 2; ledPin < 8; ledPin++) {
pinMode(ledPin, OUTPUT);
}
}

## Deskripsi

Program mengatur kecepatan kedipan LED pada Pin 6 melalui dua fase:
mempercepat secara bertahap lalu memperlambat kembali sebelum reset.

## Penjelasan Kode

- `timeDelay = 1000` : Nilai awal delay 1 detik (kedip lambat)
- `percepat = true` : Flag penanda fase saat ini (percepat/perlambat)
- `digitalWrite(HIGH/LOW)` : Menyalakan dan mematikan LED
- `delay(timeDelay)` : Jeda sesuai nilai timeDelay saat ini
- `if (percepat)` : Cek apakah sedang dalam fase percepatan
- `timeDelay -= 100` : Kurangi delay 100ms per siklus → LED makin cepat
- `percepat = false` : Beralih ke fase perlambatan saat delay ≤ 100ms
- `timeDelay += 100` : Tambah delay 100ms per siklus → LED makin lambat
- `percepat = true` : Reset ke fase percepatan saat delay kembali 1000ms

void loop() {
// Nyalakan tiga LED kiri (Pin 2-4), matikan tiga LED kanan (Pin 5-7)
for (int ledPin = 2; ledPin <= 4; ledPin++) {
digitalWrite(ledPin, HIGH); // hidupkan LED kiri
}
for (int ledPin = 5; ledPin <= 7; ledPin++) {
digitalWrite(ledPin, LOW); // matikan LED kanan
}
delay(timer); // tahan selama durasi timer

// Matikan tiga LED kiri, nyalakan tiga LED kanan
for (int ledPin = 2; ledPin <= 4; ledPin++) {
digitalWrite(ledPin, LOW); // matikan LED kiri
}
for (int ledPin = 5; ledPin <= 7; ledPin++) {
digitalWrite(ledPin, HIGH); // hidupkan LED kanan
}
delay(timer); // tahan selama durasi timer
}

## Deskripsi

Program menyalakan tiga LED kiri (Pin 2–4) dan tiga LED kanan (Pin 5–7)
secara bergantian dengan interval waktu yang dapat disesuaikan.

## Penjelasan Kode

- `timer = 500` : Durasi setiap kelompok LED menyala (500ms)
- `for (ledPin = 2; <8)` : Inisialisasi Pin 2–7 sebagai OUTPUT
- `for (ledPin = 2; <=4)` : Loop untuk mengakses tiga LED kiri (Pin 2,3,4)
- `for (ledPin = 5; <=7)` : Loop untuk mengakses tiga LED kanan (Pin 5,6,7)
- `digitalWrite(ledPin, HIGH)` : Menyalakan LED pada pin yang dituju
- `digitalWrite(ledPin, LOW)` : Mematikan LED pada pin yang dituju
- `delay(timer)` : Jeda 500ms sebelum beralih ke kelompok berikutnya
- Pola bergantian terjadi karena dua blok loop dieksekusi secara
  berurutan di dalam fungsi loop() yang berjalan terus-menerus.
