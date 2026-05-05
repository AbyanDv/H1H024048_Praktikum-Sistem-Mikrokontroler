Pertanyaannya Praktikum Modul 4 Percobaan 1
---

1. Apa fungsi perintah analogRead() pada rangkaian praktikum ini?


2. Mengapa diperlukan fungsi map() dalam program tersebut?


3. Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023. Jelaskan program pada file README.md


Jawab
---

1. `analogRead(potPin)` membaca tegangan analog dari pin A0 yang terhubung ke potensiometer, lalu mengonversinya menjadi nilai digital 10-bit dalam rentang 0 hingga 1023. Nilai 0 merepresentasikan tegangan 0V (potensiometer di posisi paling kiri) dan 1023 merepresentasikan tegangan 5V (posisi paling kanan). Hasil pembacaan ini disimpan ke variabel `nilaiADC` dan digunakan sebagai input untuk menentukan posisi sudut servo secara proporsional terhadap putaran potensiometer.


2. Fungsi `map()` diperlukan karena rentang keluaran `analogRead()` (0–1023) tidak kompatibel secara langsung dengan rentang parameter yang diterima oleh `servo.write()` (0°–180°). Tanpa `map()`, nilai ADC mentah sebesar 1023 tidak bisa dikirim ke servo karena sudut 1023° tidak valid dan di luar batas fisik servo. `map()` melakukan konversi linear proporsional sehingga setiap posisi potensiometer terpetakan ke sudut servo yang sesuai: ADC 0 → 0°, ADC 511 → sekitar 90°, ADC 1023 → 180°.


3. Berikut kodenya:

```cpp
#include <Arduino.h>
#include <Servo.h>

const int potPin = A0;   // Pin analog potensiometer
const int servoPin = 9;  // Pin sinyal servo

Servo myServo;           // Objek servo

int nilaiADC = 0;        // Variabel penyimpan hasil ADC
int sudut = 0;           // Variabel penyimpan sudut servo

void setup() {
  myServo.attach(servoPin); // Hubungkan objek servo ke pin 9
  Serial.begin(9600);       // Inisialisasi Serial Monitor
}

void loop() {
  nilaiADC = analogRead(potPin); // Baca nilai ADC dari potensiometer (0–1023)

  // Petakan ADC ke rentang sudut 30°–150° (bukan 0°–180°)
  // Sehingga meski ADC = 0, servo tidak bergerak di bawah 30°
  // dan meski ADC = 1023, servo tidak melampaui 150°
  sudut = map(nilaiADC, 0, 1023, 30, 150);

  myServo.write(sudut); // Kirim perintah sudut ke servo

  // Tampilkan nilai ADC dan sudut ke Serial Monitor untuk monitoring
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | Sudut: ");
  Serial.print(sudut);
  Serial.println("°");

  delay(50); // Jeda 50ms untuk stabilisasi pembacaan
}
```


Pertanyaannya Praktikum Modul 4 Percobaan 2
---

1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!


2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?


3. Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200. Jelaskan program pada file README.md.


Jawab
---

1. `analogWrite()` tidak benar-benar mengeluarkan tegangan analog, melainkan menggunakan teknik PWM (Pulse Width Modulation). Pin PWM seperti pin 9 mengeluarkan sinyal digital yang berganti-ganti HIGH dan LOW dengan frekuensi sekitar 490 Hz. Yang divariasikan adalah lebar pulsa HIGH-nya (duty cycle), ditentukan oleh nilai parameter 0–255. Nilai 0 berarti duty cycle 0% (selalu LOW, LED mati), nilai 255 berarti duty cycle 100% (selalu HIGH, LED penuh terang), dan nilai di antaranya menghasilkan tegangan rata-rata proporsional. Karena frekuensi switching jauh lebih cepat dari respons mata manusia, yang terlihat hanyalah tingkat kecerahan yang berbeda-beda.


2. Keduanya dihubungkan secara linear melalui fungsi `map(nilaiADC, 0, 1023, 0, 255)`. ADC memiliki resolusi 10-bit sehingga rentangnya 0–1023, sedangkan PWM pada Arduino Uno berbasis timer 8-bit sehingga rentangnya 0–255. Rasio konversinya adalah 255 / 1023 ≈ 0.249, artinya setiap kenaikan ADC sebesar ±4 setara dengan kenaikan PWM sebesar ±1. Contoh: ADC 512 (tengah) menghasilkan PWM ≈ 127 (setengah kecerahan).


3. Berikut kodenya:

```cpp
#include <Arduino.h>

const int potPin = A0;  // Pin analog potensiometer
const int ledPin = 9;   // Pin PWM LED

int nilaiADC = 0;       // Variabel penyimpan hasil ADC
int pwm = 0;            // Variabel penyimpan nilai PWM

void setup() {
  pinMode(ledPin, OUTPUT); // Atur pin LED sebagai output
  Serial.begin(9600);      // Inisialisasi Serial Monitor
}

void loop() {
  nilaiADC = analogRead(potPin); // Baca nilai ADC dari potensiometer (0–1023)

  // Konversi ADC ke nilai PWM (0–255) secara proporsional
  pwm = map(nilaiADC, 0, 1023, 0, 255);

  // LED hanya aktif jika PWM berada dalam rentang kecerahan sedang (50–200)
  // Di luar rentang tersebut, LED dimatikan paksa dengan analogWrite(ledPin, 0)
  if (pwm >= 50 && pwm <= 200) {
    analogWrite(ledPin, pwm); // Nyalakan LED dengan kecerahan sesuai PWM
  } else {
    analogWrite(ledPin, 0);   // Matikan LED jika PWM di luar rentang 50–200
  }

  // Tampilkan nilai ADC dan PWM ke Serial Monitor untuk monitoring
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | PWM: ");
  Serial.println(pwm);

  delay(50); // Jeda 50ms untuk stabilisasi pembacaan
}
```