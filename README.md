# Rc-Car-With-Pid
# Penjelasan literatur :
  https://blog.indobot.co.id/pid-controller-untuk-arduino/
  https://microcontrollerslab.com/pid-controller-implementation-using-arduino/
  https://www.project.semesin.com/2018/03/13/sistem-kontrol-pid-dengan-arduino/
# Penjelasan youtube :
  https://www.youtube.com/watch?v=4Y7zG48uHRo
  https://youtube.com/shorts/gEkkCLAwXrQ?si=wZKT3bZutA0o7bko
# Penjelasan :
  Konsep PID (Proportional-Integral-Derivative)
  PID adalah metode kontrol yang menggunakan tiga parameter untuk menstabilkan sistem:
   1. Proportional (P): Mengoreksi kesalahan saat ini. Semakin besar kesalahan, semakin besar koreksi yang diberikan.
      Dihitung dengan Kp * error.
   2. Integral (I): Mengoreksi kesalahan akumulatif dari waktu ke waktu untuk menghilangkan offset. Dihitung dengan Ki * integral.
   3. Derivative (D): Mengoreksi berdasarkan laju perubahan kesalahan, membantu meredam respons yang terlalu cepat. Dihitung dengan Kd * derivative.
# Penerapan PID pada RC car
  1. Konfigurasi Hardware
      - Motor: Menggunakan AF_DCMotor untuk menggerakkan roda.
      - Servo: Menggunakan servo untuk mengarahkan roda depan.
      - Sensor Ultrasonik: Menggunakan sensor untuk mendeteksi jarak ke dinding kiri dan kanan lintasan. 
  2. Baca Jarak Sensor
      - Menggunakan sensor ultrasonik untuk mengukur jarak ke dinding kiri (Echo2) dan kanan (Echo3).
  3. Hitung Kesalahan
      - Kesalahan (error) adalah selisih antara jarak kiri dan kanan. Jika mobil lebih dekat ke dinding kanan,
        error akan positif; jika lebih dekat ke kiri, error akan negatif.
  4.  Kalkulasi PID
      - Proportional: Memberikan koreksi berdasarkan besar kesalahan saat ini.
      - Integral: Menambah kesalahan dari waktu ke waktu untuk menghilangkan kesalahan tetap.
      - Derivative: Mengurangi koreksi jika kesalahan berubah terlalu cepat untuk menghindari osilasi.
  5. Sesuaikan Posisi Servo
     - Menggunakan hasil kalkulasi PID untuk mengatur sudut servo, memastikan mobil tetap di tengah lintasan.
  6. Sesuaikan Kecepatan Motor
     - Kecepatan motor diatur berdasarkan besarnya kesalahan, memastikan mobil bergerak dengan kecepatan yang stabil dan sesuai.
