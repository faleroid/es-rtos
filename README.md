# ⏱️ FreeRTOS Sensor Monitoring System (Producer-Consumer)

[![Wokwi Simulation](https://img.shields.io/badge/Wokwi-Simulasi_Project-blue?style=for-the-badge&logo=wokwi)](https://wokwi.com/projects/463060527444257793)

## 📌 Deskripsi Proyek
Proyek ini merupakan implementasi sistem operasi waktu nyata (**Real-Time Operating System / RTOS**) menggunakan pustaka FreeRTOS pada Arduino Uno. Sistem mendemonstrasikan arsitektur *Producer-Consumer* yang berjalan secara **sinkron** tanpa menggunakan fungsi `loop()` bawaan, melainkan sepenuhnya dikendalikan oleh *RTOS Scheduler*.

## ⚙️ Arsitektur Sinkronisasi (Task & Queue)
Sistem ini menggunakan mekanisme **Queue (Antrean)** sebagai jembatan komunikasi antar-Task untuk mencegah *Race Condition* dan membuang siklus CPU.

1. **Task 1: Producer (`TaskReadSensor`)**
   - **Fungsi:** Membaca nilai analog dari potensiometer (Pin A0).
   - **Aksi:** Mengirimkan nilai tersebut ke dalam `sensorQueue`.
   - **State:** Memasuki *Blocked State* (tidur) selama 1 detik setelah pengiriman sukses.
2. **Task 2: Consumer (`TaskProcessData`)**
   - **Fungsi:** Mengontrol aktuator (LED di Pin 8) berdasarkan data sensor.
   - **Aksi:** Secara *default* berada dalam *Blocked State* (menunggu data dari antrean). Sistem akan langsung terbangun seketika (*context switch*) saat Task 1 memasukkan data ke antrean.

## 🛠️ Kebutuhan Perangkat Keras
- 1x Arduino Uno
- 1x Potensiometer (Sensor Analog)
- 1x LED & Resistor 220Ω (Aktuator)

## 🚀 Cara Menjalankan
1. Buka tautan simulasi Wokwi.
2. Klik tombol **Start Simulation**.
3. Buka **Serial Monitor** untuk melihat log eksekusi sinkronisasi antara Pengirim (Task 1) dan Penerima (Task 2).
4. Putar *knob* potensiometer; LED akan menyala jika nilai melewati ambang batas (> 512).

---
*Dikembangkan oleh Naufal Satrio Putra (H1D024079) - 2026*
