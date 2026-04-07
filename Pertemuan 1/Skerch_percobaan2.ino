int timer = 500; // delay 0.5 detik

void setup() {
  // Gunakan loop for untuk menginisialisasi setiap pin (2-7) sebagai output:
  for (int ledPin = 2; ledPin < 8; ledPin++) {
    pinMode(ledPin, OUTPUT);
  }
}

void loop() {
  // Loop melalui pin 2 hingga 7
  for (int ledPin = 2; ledPin < 8; ledPin++) {
    // Jika nomor pin genap, nyalakan
    if (ledPin % 2 == 0) {
      digitalWrite(ledPin, HIGH);
    } 
    // Jika nomor pin ganjil, matikan
    else {
      digitalWrite(ledPin, LOW);
    }
  }
  delay(timer); // Jeda
  
  // Balik logikanya (Ganjil nyala, Genap mati)
  for (int ledPin = 2; ledPin < 8; ledPin++) {
    if (ledPin % 2 == 0) {
      digitalWrite(ledPin, LOW);
    } 
    else {
      digitalWrite(ledPin, HIGH);
    }
  }
  delay(timer); // Jeda
}
