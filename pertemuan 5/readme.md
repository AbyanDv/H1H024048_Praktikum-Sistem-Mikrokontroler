Pertanyaannya Praktikum Modul 5 Percobaan 1
---

1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!


2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!


3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan program pada file README.md.


Jawab
---

1. Ketiga task berjalan secara bergantian, bukan benar-benar bersamaan/paralel. Namun karena pergantiannya sangat cepat, seolah-olah ketiganya berjalan bersamaan. FreeRTOS menggunakan scheduler preemptive berbasis time-slicing: setiap task mendapat jatah waktu CPU secara bergantian. Ketika sebuah task memanggil vTaskDelay(), task tersebut masuk ke kondisi blocked dan CPU langsung diserahkan ke task lain yang berstatus ready. Karena TaskBlink1, TaskBlink2, dan Taskprint semuanya memiliki prioritas yang sama (prioritas 1), scheduler mengeksekusinya secara round-robin. TaskBlink1 berkedip setiap 200ms, TaskBlink2 setiap 300ms, dan Taskprint mencetak counter setiap 500ms, masing-masing memblokir diri selama menunggu, sehingga task lain dapat berjalan di antaranya.


2. Untuk menambahkan task keempat, pertama deklarasikan prototipe fungsi task baru di awal program, misalnya void TaskKeempat(void *pvParameters);. Kemudian daftarkan task tersebut ke scheduler menggunakan xTaskCreate() di dalam fungsi setup() sebelum vTaskStartScheduler() dipanggil, dengan mengisi parameter nama task, ukuran stack, prioritas, dan handle. Terakhir, implementasikan fungsi TaskKeempat() di bawah fungsi-fungsi task lainnya, lengkap dengan loop while(1) atau for(;;) dan pemanggilan vTaskDelay() agar task dapat melepas CPU saat tidak aktif.


3. Berikut kodenya:

```cpp
#include <Arduino_FreeRTOS.h>

// Variabel global untuk menyimpan delay LED yang dikontrol potensiometer
volatile int ledDelay = 200;

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);
void TaskReadPot(void *pvParameters);

void setup() {
  Serial.begin(9600);

  xTaskCreate(TaskBlink1,  "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2,  "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint,   "task3", 128, NULL, 1, NULL);
  xTaskCreate(TaskReadPot, "task4", 128, NULL, 1, NULL); // Task keempat: baca potensiometer

  vTaskStartScheduler();
}

void loop() {}

// Task 1: LED di pin 8 berkedip dengan kecepatan yang dikontrol potensiometer
void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);
  while (1) {
    Serial.println("Task1");
    digitalWrite(8, HIGH);
    vTaskDelay(ledDelay / portTICK_PERIOD_MS);
    digitalWrite(8, LOW);
    vTaskDelay(ledDelay / portTICK_PERIOD_MS);
  }
}

// Task 2: LED di pin 7 berkedip dengan kecepatan tetap 300ms
void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);
  while (1) {
    Serial.println("Task2");
    digitalWrite(7, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(7, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// Task 3: Mencetak counter ke Serial Monitor setiap 500ms
void Taskprint(void *pvParameters) {
  int counter = 0;
  while (1) {
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task 4: Membaca nilai potensiometer dari A0 dan mengonversinya
// menjadi nilai delay (100ms–1000ms) untuk mengontrol kecepatan LED Task1
void TaskReadPot(void *pvParameters) {
  while (1) {
    int nilaiADC = analogRead(A0);                    // Baca potensiometer (0–1023)
    ledDelay = map(nilaiADC, 0, 1023, 100, 1000);    // Petakan ke rentang delay 100–1000ms

    Serial.print("ADC: ");
    Serial.print(nilaiADC);
    Serial.print(" | Delay LED: ");
    Serial.print(ledDelay);
    Serial.println("ms");

    vTaskDelay(100 / portTICK_PERIOD_MS);             // Perbarui nilai delay setiap 100ms
  }
}
```


Pertanyaannya Praktikum Modul 5 Percobaan 2
---

1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!


2. Apakah program ini berpotensi mengalami race condition? Jelaskan!


3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file README.md.


Jawab
---

1. Kedua task berjalan secara bergantian (concurrent), bukan paralel, dikelola oleh FreeRTOS scheduler. Task read_data mengisi struct readings dengan nilai suhu dan kelembaban, lalu mengirimkannya ke queue melalui xQueueSend() dan masuk ke delay 100ms. Sementara itu, task display memblokir dirinya sendiri dengan xQueueReceive() sambil menunggu data tersedia di queue. Begitu read_data berhasil mengirim data, display di-unblock oleh scheduler dan langsung membaca isi queue untuk dicetak ke Serial Monitor. Mekanisme queue inilah yang menjadi jembatan sinkronisasi antar kedua task tanpa keduanya harus aktif di waktu yang sama.


2. Program ini berpotensi sangat rendah mengalami race condition karena sudah menggunakan mekanisme queue FreeRTOS (QueueHandle_t) yang bersifat thread-safe secara internal. Queue menggunakan mutual exclusion bawaan sehingga hanya satu task yang dapat mengakses data pada satu waktu, read_data tidak dapat menulis saat display sedang membaca, dan sebaliknya. Namun perlu diperhatikan bahwa ukuran queue hanya 1: jika read_data mencoba mengirim data baru sebelum display sempat mengambil data sebelumnya, xQueueSend() dengan parameter portMAX_DELAY akan memblokir read_data hingga slot queue kosong, sehingga data lama tidak tertimpa begitu saja dan risiko race condition tetap terjaga.


3. Berikut kodenya:

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

#define DHTPIN  2        // Pin data sensor DHT terhubung ke pin digital 2
#define DHTTYPE DHT11    // Tipe sensor: DHT11 (ganti DHT22 jika menggunakan DHT22)

DHT dht(DHTPIN, DHTTYPE); // Inisialisasi objek sensor DHT

struct readings {
  float temp; // Menggunakan float agar nilai suhu DHT lebih akurat
  float h;    // Menggunakan float agar nilai kelembaban DHT lebih akurat
};

QueueHandle_t my_queue;

void setup() {
  Serial.begin(9600);
  dht.begin(); // Inisialisasi sensor DHT sebelum task dimulai

  my_queue = xQueueCreate(1, sizeof(struct readings));

  xTaskCreate(read_data, "read sensors", 256, NULL, 0, NULL);
  xTaskCreate(display,   "display",      256, NULL, 0, NULL);
}

void loop() {}

// Task pembaca sensor: membaca suhu dan kelembaban aktual dari DHT
// lalu mengirimkan hasilnya ke queue setiap 2000ms (sesuai sampling rate DHT)
void read_data(void *pvParameters) {
  struct readings x;
  for (;;) {
    x.temp = dht.readTemperature(); // Baca suhu dalam °C dari sensor DHT
    x.h    = dht.readHumidity();    // Baca kelembaban relatif (%) dari sensor DHT

    // Hanya kirim data jika pembacaan sensor valid (bukan NaN)
    if (!isnan(x.temp) && !isnan(x.h)) {
      xQueueSend(my_queue, &x, portMAX_DELAY);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS); // DHT membutuhkan minimal ~2 detik antar pembacaan
  }
}

// Task penampil: menunggu data dari queue lalu mencetak ke Serial Monitor
void display(void *pvParameters) {
  struct readings x;
  for (;;) {
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.print("temp = ");
      Serial.println(x.temp);       // Menampilkan suhu aktual dari sensor DHT
      Serial.print("humidity = ");
      Serial.println(x.h);          // Menampilkan kelembaban aktual dari sensor DHT
    }
  }
}
```