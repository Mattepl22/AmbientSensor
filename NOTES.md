# NOTES – Nodo sensori STM32

## Setup
- Scheda: <STM32F411CEU6>
- MCU: <STM32F411CEU6> → macro `STM32F411xx`
- Toolchain: STM32CubeCLT 1.22.0, CMake
- CMSIS: dal pacchetto STM32Cube_FW_F4_V<1.28.3>
  - Device: `Drivers/CMSIS/Device/ST/STM32F4xx/Include` (solo stm32f4xx.h, stm32f407xx.h, system_stm32f4xx.h)
  - Core Arm: `Drivers/CMSIS/Include` (solo cmsis_compiler.h, cmsis_gcc.h, cmsis_version.h, core_cm4.h, mpu_armv7.h)

## Diario

### 2026-11-04
- Obiettivo: creare il progetto e compilare un main vuoto con gli header CMSIS
- Fatto: installato pacchetto F4 da CubeMX, copiati gli header, aggiornato CMakeLists (include + macro chip)
- Prossimo passo: driver GPIO per il pulsante

- Obbiettivo: iniziare libreria pulsante.
- Fatto: Funzione di inizializzazione del pulsante.
- Fatto: Funzione di lettura del pulsante.
- Problema: Il semihosting non funziona.
- Soluzione: Inserire in .vscode/launch.json l'abilitazione per il semihosting.
- Prossimo passo: Configurazione timer per filtro alla pressione del pulsante