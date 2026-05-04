#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Definisi Pin
const int POT_PIN = A0;
const int LED_PIN = 8;

// Deklarasi Queue Handle (Sebagai jembatan sinkronisasi antar Task)
QueueHandle_t sensorQueue;

// Deklarasi fungsi Task
void TaskReadSensor(void *pvParameters);
void TaskProcessData(void *pvParameters);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  // Queue
  sensorQueue = xQueueCreate(5, sizeof(int));

  if (sensorQueue != NULL) {
    
    // Task 1
    // Parameter: Fungsi, Nama Task, Ukuran Stack, Parameter fungsi, Prioritas, Handle Task
    xTaskCreate(TaskReadSensor, "ReadSensor", 128, NULL, 1, NULL);
    
    // Task 2
    // Prioritas diset ke 2 (Lebih tinggi) agar langsung merespons saat data tiba
    xTaskCreate(TaskProcessData, "ProcessData", 128, NULL, 2, NULL);
  }
}

void loop() {
}

// TASK 1: PRODUCER (Pembaca Sensor)
void TaskReadSensor(void *pvParameters) {
  (void) pvParameters; // Menghindari warning dari compiler
  int sensorValue = 0;

  for (;;) { // Infinite loop 
    // Membaca nilai dari potensiometer (0 - 1023)
    sensorValue = analogRead(POT_PIN);
    
    Serial.print("Task 1 (Pengirim): Membaca nilai = ");
    Serial.println(sensorValue);

    // Mengirim data ke Queue secara sinkron
    // portMAX_DELAY artinya task akan menunggu (block) jika queue penuh
    xQueueSend(sensorQueue, &sensorValue, portMAX_DELAY);

    // Block/tidurkan task ini selama 1000 milidetik (1 detik)
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// TASK 2: CONSUMER (Pemroses Data)
void TaskProcessData(void *pvParameters) {
  (void) pvParameters;
  int receivedValue = 0;

  for (;;) {
    // xQueueReceive akan mem-BLOCK Task 2 sampai ada data masuk dari Task 1.
    // Inilah letak SINKRONISASINYA. Task 2 tidak akan membuang siklus CPU (tidak melakukan polling).
    if (xQueueReceive(sensorQueue, &receivedValue, portMAX_DELAY) == pdPASS) {
      
      Serial.print("Task 2 (Penerima): Mengeksekusi nilai = ");
      Serial.println(receivedValue);

      // Logika aktuator: Nyalakan LED jika nilai potensiometer lebih dari setengah (> 512)
      if (receivedValue > 512) {
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }
      
      Serial.println("----------------------------------");
    }
  }
}
