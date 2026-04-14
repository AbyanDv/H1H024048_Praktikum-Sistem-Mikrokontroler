2.5.4 Pertanyaan Praktikum:

1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
2. Apa yang terjadi jika nilai num lebih dari 15?
3. Apakah program ini menggunakan common cathode atau common anode? Jelaskan alasannya!
4. Modifikasi program agar tampilan berjalan dari F ke 0 dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md

Jawab :

1. https://www.tinkercad.com/things/jvo9r3gpfdz-7-segment-no-button?sharecode=hRgBBVawg9GaGUfsvjcGoQkjtZ6-ALPQcdRyCHhMis0

2. Array digitPattern hanya memiliki indeks 0–15. Jika num > 15, terjadi out-of-bounds access — program membaca memori di luar array yang isinya tidak terdefinisi, sehingga tampilan seven segment akan menampilkan pola acak/tidak valid.

3. Program ini menggunakan Common Anode (CA). Buktinya ada pada baris:

digitalWrite(segmentPins[i], !digitPattern[num][i]); (dibalik).

Pada CA, segmen menyala saat pin diberi logika LOW. Nilai 1 dalam digitPattern berarti "segmen aktif", maka dibalik (!) agar menghasilkan LOW. Jika Common Cathode, tidak perlu pembalikan karena segmen menyala saat HIGH.

4. Cukup ubah arah loop di loop():

void loop()
{
for(int i = 15; i >= 0; i--) // F (15) turun ke 0
{
displayDigit(i);
delay(1000);
}
}

2.6.4 Pertanyaan Praktikum

1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
2. Mengapa pada push button digunakan mode INPUT_PULLUP pada Arduino Uno?
   Apa keuntungannya dibandingkan rangkaian biasa?
3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari
   sisi hardware maupun software?
4. Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai
   penambahan (increment) dan pengurangan (decrement) pada sistem counter dan
   berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

Jawab :

1. https://www.tinkercad.com/things/7gKXJLQ3ZL6-7-segment-with-button?sharecode=yrMragxyP6qNO7-wotnE-21eWiVxxilXv3yNklcrq_o

2. INPUT_PULLUP mengaktifkan resistor pull-up internal Arduino sehingga pin terbaca HIGH saat tombol tidak ditekan, dan LOW saat ditekan. Keuntungannya: tidak perlu resistor pull-up eksternal, rangkaian lebih sederhana, dan menghindari kondisi floating pin yang menyebabkan pembacaan tidak stabil/noise.

3. Kemungkinan penyebab permasalahan pada rangkaian dapat ditinjau dari dua aspek utama, yaitu hardware dan software. Dari sisi hardware, gangguan dapat terjadi akibat kabel jumper yang lepas atau tidak terpasang pada posisi yang benar, kerusakan pada resistor 220Ω, maupun pin pada seven segment yang mengalami kerusakan sehingga tidak dapat mengalirkan arus dengan baik. Sementara itu, dari sisi software, kesalahan dapat disebabkan oleh penentuan nomor pin yang tidak sesuai pada array segmentPins[], penyusunan pola pada digitPattern yang tidak tepat untuk segmen tertentu, serta penggunaan logika inversi (!) yang tidak sesuai dengan jenis seven segment yang digunakan, baik Common Anode (CA) maupun Common Cathode (CC).

4. Berikut kodenya:

#include <Arduino.h>

// Pin mapping segment: a b c d e f g dp
const int segmentPins[8] = {7, 6, 5, 10, 11, 8, 9, 4};

// Pin push button
const int btnUp   = 3; // tombol increment (tambah)
const int btnDown = 2; // tombol decrement (kurang)

// Pola digit 0-F untuk Common Anode
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, //0
  {0,1,1,0,0,0,0,0}, //1
  {1,1,0,1,1,0,1,0}, //2
  {1,1,1,1,0,0,1,0}, //3
  {0,1,1,0,0,1,1,0}, //4
  {1,0,1,1,0,1,1,0}, //5
  {1,0,1,1,1,1,1,0}, //6
  {1,1,1,0,0,0,0,0}, //7
  {1,1,1,1,1,1,1,0}, //8
  {1,1,1,1,0,1,1,0}, //9
  {1,1,1,0,1,1,1,0}, //A
  {0,0,1,1,1,1,1,0}, //b
  {1,0,0,1,1,1,0,0}, //C
  {0,1,1,1,1,0,1,0}, //d
  {1,0,0,1,1,1,1,0}, //E
  {1,0,0,0,1,1,1,0}  //F
};

// Counter saat ini (0-15)
int currentDigit = 0;

// State tombol sebelumnya untuk edge detection
bool lastUpState   = HIGH;
bool lastDownState = HIGH;

// Fungsi menampilkan digit ke seven segment
void displayDigit(int num)
{
  for (int i = 0; i < 8; i++)
  {
    // Logika dibalik karena Common Anode: segmen aktif = LOW
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

void setup()
{
  // Set semua pin segmen sebagai OUTPUT
  for (int i = 0; i < 8; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Set tombol sebagai INPUT_PULLUP (tidak tekan = HIGH, tekan = LOW)
  pinMode(btnUp,   INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);

  // Tampilkan nilai awal (0)
  displayDigit(currentDigit);
}

void loop()
{
  bool upState   = digitalRead(btnUp);
  bool downState = digitalRead(btnDown);

  // Deteksi tekan btnUp: transisi HIGH -> LOW
  if (lastUpState == HIGH && upState == LOW)
  {
    currentDigit++;
    if (currentDigit > 15) currentDigit = 0; // wrap dari F ke 0
    displayDigit(currentDigit);
    delay(200); // debounce sederhana
  }

  // Deteksi tekan btnDown: transisi HIGH -> LOW
  if (lastDownState == HIGH && downState == LOW)
  {
    currentDigit--;
    if (currentDigit < 0) currentDigit = 15; // wrap dari 0 ke F
    displayDigit(currentDigit);
    delay(200); // debounce sederhana
  }

  // Simpan state sekarang untuk iterasi berikutnya
  lastUpState   = upState;
  lastDownState = downState;
}
