Pertanyaannya Praktikum 3.5.4
---

1. Jelaskan proses dari input keyboard hingga LED menyala/mati!


2. Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?


3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!


4. Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap sistem.


jawab
---

1. User mengetik karakter ('1' atau '0') di Serial Monitor lalu menekan Send. Data dikirim melalui USB ke Arduino via protokol UART (pin RX). Arduino mendeteksi data masuk lewat Serial.available(), lalu membacanya dengan Serial.read(). Jika karakternya '1' maka digitalWrite(PIN_LED, HIGH) dijalankan dan LED menyala. Jika '0' maka digitalWrite(PIN_LED, LOW) dan LED mati.


2. Serial.available() digunakan untuk mengecek apakah ada data yang sudah masuk ke buffer serial sebelum dibaca. Jika baris ini dihilangkan, Serial.read() tetap dipanggil meski buffer kosong dan akan mengembalikan nilai -1. Karena -1 bukan '1', '0', '\n', maupun '\r', program akan terus-menerus mencetak "Perintah tidak dikenal" ke Serial Monitor di setiap iterasi loop, membanjiri output.


3. Berikut kodenya:

```cpp
#include <Arduino.h>

const int PIN_LED = 8;
bool blinkMode = false;
bool ledState = false;
unsigned long previousMillis = 0;
const long blinkInterval = 500;

void setup() {
  Serial.begin(9600);
  Serial.println("Ketik '1' ON, '0' OFF, '2' BLINK");
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}

void loop() {
  // Cek input serial
  if (Serial.available() > 0) {
    char data = Serial.read();

    if (data == '1') {
      blinkMode = false;
      digitalWrite(PIN_LED, HIGH);
      Serial.println("LED ON");
    } else if (data == '0') {
      blinkMode = false;
      digitalWrite(PIN_LED, LOW);
      ledState = false;
      Serial.println("LED OFF");
    } else if (data == '2') {
      blinkMode = true;
      Serial.println("LED BLINK ON");
    } else if (data != '\n' && data != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }

  // Jalankan blink jika aktif
  if (blinkMode) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(PIN_LED, ledState ? HIGH : LOW);
    }
  }
}
```

4. Gunakan millis(). Alasannya, delay() bersifat blocking — seluruh program berhenti selama durasi delay, termasuk pembacaan Serial. Akibatnya ketika user mengirim perintah baru saat LED sedang blink, Arduino tidak akan merespons sampai delay selesai. Sebaliknya, millis() hanya mengecek selisih waktu tanpa menghentikan loop, sehingga Serial tetap terbaca dan sistem tetap responsif meski LED sedang berkedip.


Pertanyaannya Praktikum 3.6.4
---

1. Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian
tersebut!


2. Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan
pin kanan tertukar!


3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai
output) sehingga:
    - Data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor


    - Adapun data yang ditampilkan pada Serial Monitor sesuai dengan table berikut:

            ADC: 0          Volt: 0.00 V            Persen: 0%


         Tampilan jika potensiometer dalam kondisi diputar paling kiri

    - ADC: 0 0% | setCursor(0, 0) dan Bar (level) | setCursor(0, 1)


    - Berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

4. Lengkapi table berikut berdasarkan pengamatan pada Serial Monitor

        ADC Volt (V)            Persen (%)
            1
            21
            49
            74
            96


Jawab
---

1. Arduino bertindak sebagai Master dan LCD sebagai Slave. Komunikasi hanya menggunakan dua jalur yaitu SDA (pin A4) untuk data dan SCL (pin A5) untuk clock. Setiap kali Arduino ingin mengirim data, ia mengirimkan alamat LCD (0x27) terlebih dahulu melalui SDA, lalu LCD yang memiliki alamat tersebut akan merespons. Data tampilan kemudian dikirim bit per bit, disinkronkan oleh sinyal clock dari SCL. Library LiquidCrystal_I2C mengurus semua proses ini secara otomatis di balik fungsi seperti lcd.print() dan lcd.setCursor().

2. Pin potensiometer harus sesuai — kaki kiri ke GND, kaki tengah ke A0, kaki kanan ke 5V. Jika kaki kiri dan kanan tertukar (kiri ke 5V, kanan ke GND), pembacaan ADC akan terbalik. Yang seharusnya bernilai 0 saat diputar kiri menjadi 1023, dan sebaliknya. Artinya bar di LCD akan penuh saat potensiometer di posisi minimum dan kosong saat maksimum — berlawanan dengan yang diharapkan.

3. Berikut kodenya :

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD I2C alamat 0x27, 16 kolom, 2 baris
const int pinPot = A0;               // Pin analog potensiometer

void setup() {
  Serial.begin(9600);  // Inisialisasi komunikasi serial UART
  lcd.init();          // Inisialisasi LCD
  lcd.backlight();     // Nyalakan backlight LCD
}

void loop() {
  int nilai = analogRead(pinPot);                      // Baca nilai ADC (0-1023)
  float volt = nilai * (5.0 / 1023.0);                 // Konversi ADC ke tegangan (Volt)
  int persen = map(nilai, 0, 1023, 0, 100);            // Konversi ADC ke persentase (0-100%)
  int panjangBar = map(nilai, 0, 1023, 0, 16);         // Konversi ADC ke panjang bar LCD (0-16)

  // Output ke Serial Monitor (UART)
  Serial.print("ADC: "); Serial.print(nilai);
  Serial.print("  Volt: "); Serial.print(volt, 2); Serial.print(" V");
  Serial.print("  Persen: "); Serial.print(persen); Serial.println("%");

  // Baris 1 LCD: tampilkan nilai ADC dan persentase
  lcd.setCursor(0, 0);
  lcd.print("ADC:"); lcd.print(nilai);
  lcd.print(" "); lcd.print(persen); lcd.print("%  ");

  // Baris 2 LCD: tampilkan bar level
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255); // Karakter blok penuh sebagai bar
    } else {
      lcd.print(" ");       // Kosongkan sisa bar
    }
  }

  delay(200); // Jeda 200ms sebelum pembacaan berikutnya
}
```

4. Nilai Volt diperoleh dari rumus ADC × (5.0 / 1023), sedangkan Persen dari ADC / 1023 × 100%. Saat ADC bernilai 1, tegangan yang terbaca hampir nol yaitu sekitar 0.00 V dengan persentase 0%. Pada ADC 21, tegangan naik menjadi sekitar 0.10 V atau 2%. ADC 49 menghasilkan sekitar 0.24 V atau 5%, ADC 74 sekitar 0.36 V atau 7%, dan ADC 96 sekitar 0.47 V atau 9%. Secara keseluruhan terlihat bahwa semakin besar nilai ADC, semakin besar pula tegangan dan persentasenya secara proporsional, sesuai dengan karakteristik potensiometer sebagai pembagi tegangan.


