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

### 2026-11-06
- Obbiettibo: Implementare timer
- Fatto: Implementata inizializzazione timer con inizializzazione pulsante + 
         gestione IRQ del timer (impostato TIM11)
- Problema: a. Inizialmente il timer non funzionava.
            b. La gestione della IRQ messa in un file .c separato non funzionava.
- Soluzione: a. Analizzanto il registro cnt 
                ho visto che questo non contava. Avevo fatto un piccolo errore di
                logica, avevo scritto 'TIM11->CR1 &= ~TIM_CR1_CEN;' invece di 
                'TIM11->CR1 |= TIM_CR1_CEN;'.
              b. Inserendo la chiamata nel cmake/files.cmake ora funziona.
- Note: Ho provato vari tempi di filtro. 200ms sono quelli che vanno meglio.
- Prossimo passo: Iniziare a scrivere la libreria del DHT22. Logica del pulsante
                  da continuare quando sarà stata implementata la logica del DHT22.

#### N.B. La libreria del pulsante va migliorata per permettere di avere più pulsanti e per permettere l'inserimento del pin del pulsante

### 2026-11-08
- Obbiettivo: Iniziare a scrivere la libreria per il DHT22
- Fatto: Scritta funzione per scrivere la modalità del pin e funzione per
         l'inizializzazione.
- Problema: 
- Soluzione: 
- Prossimo passo: Sistemare funzione per inizializzazione e continuare scrittura 
                  libreria

### 2026-11-09
- Obbiettivo: Continuazione scrittura libreria per il DHT22
- Fatto: Impostato timer 10 per tempi richiesti a dht22
- Problema: 
- Soluzione: 
- Prossimo passo: Continuare con la libreria
