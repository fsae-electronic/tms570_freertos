# FreeRTOS Tasks

This document describes every FreeRTOS task in the firmware. All tasks are defined in `app/App.cpp` and use **static allocation** (no heap for TCBs or stacks).

---

## Synchronization Objects

Before describing individual tasks, it helps to understand the shared communication infrastructure.

### Event Group — `xSysEvents`

A 32-bit FreeRTOS event group. Tasks block on `xEventGroupWaitBits()` and ISRs post to it with `xEventGroupSetBitsFromISR()`. This is the primary mechanism for waking tasks.

| Bit | Constant | Source | Consumers |
|---|---|---|---|
| 0 | `EVT_CAN_SENSORS` | CAN ISR — MB1 | (informational; data read periodically) |
| 1 | `EVT_CAN_SPEED` | CAN ISR — MB2 | (informational) |
| 2 | `EVT_CAN_MOTOR1` | CAN ISR — MB3 | (informational) |
| 3 | `EVT_CAN_MOTOR2` | CAN ISR — MB4 | (informational) |
| 4 | `EVT_CAN_DRIVERS_STATUS` | CAN ISR — MB5 | (informational) |
| 5 | `EVT_CAN_DYNAMIC` | CAN ISR — MB6 | (informational) |
| 10 | `EVT_SCI_RX_READY` | SCI ISR (Rx) | `vRFDataRxTask` |
| 11 | `EVT_SCI_TX_READY` | SCI ISR (Tx) | (unused) |
| 12 | `EVT_SCI_CMD` | `vRFDataRxTask` | `vCmdTask` |
| 13 | `EVT_SCI_TELEMETRY_ACK` | `vRFDataRxTask` | `vTelemetryTask` |
| 14 | `EVT_TELEMETRY_NACK` | `xTxTimeoutTimer` callback | `vTelemetryTask` |
| 15 | `EVT_NEW_DATA` | `vDataUpdateTask` | `vRulesTask` |
| 16 | `EVT_DATA_APPROVED` | `vRulesTask` | `vDriversFSMTask` |
| 20 | `CMD_READY` | (reserved) | `vRFDataRxTask` |
| 21 | `TELEMETRY_READY` | (reserved) | `vRFDataRxTask` |
| 25 | `EVT_PERIODIC_DATA_UPDATE` | RTI ISR (60 Hz) | `vDataUpdateTask` |
| 26 | `EVT_PERIODIC_TELEMETRY` | RTI ISR (50 Hz) | `vTelemetryTask` |
| 27 | `EVT_PERIODIC_UI_INPUT` | RTI ISR (unused) | (unused) |
| 28 | `EVT_PERIODIC_DISPLAY_UPDATE` | RTI ISR (30 Hz) | `vDisplayTask` |

### Mutex — `xDataMutex`

Binary semaphore protecting the shared `AppData_t` local-copy struct. `vDataUpdateTask` holds this briefly while copying global data into the local snapshot. Other tasks take it before reading from the snapshot.

### Software Timer — `xTxTimeoutTimer`

One-shot timer, 5 ms period. Started by `vTelemetryTask` after each transmission. If no ACK arrives before it fires, the callback sets `EVT_TELEMETRY_NACK`, triggering a retransmission.

---

## Task Summary Table

| Task name | Priority | Stack | Trigger | Status |
|---|---|---|---|---|
| `vInitTask` | 7 (MAX) | 128 B | Runs once at scheduler start | Deletes itself |
| `vDataUpdateTask` | 6 | 512 B | 60 Hz RTI periodic | Active |
| `vRulesTask` | 5 | 512 B | `EVT_NEW_DATA` | Placeholder |
| `vRFDataRxTask` | 4 | 512 B | `EVT_SCI_RX_READY` | Placeholder |
| `vDriversFSMTask` | 3 | 1024 B | `EVT_DATA_APPROVED` | Placeholder |
| `vCmdTask` | 3 | 512 B | `EVT_SCI_CMD` | Placeholder |
| `vTelemetryTask` | 2 | 512 B | 50 Hz RTI periodic + ACK/NACK | Active |
| `vDisplayTask` | 1 | 768 B | 30 Hz RTI periodic | Active |
| `vUIInputTask` | 1 | 256 B | — | **Disabled** (commented out) |

---

## Tasks in Detail

---

### `vInitTask` — Initialization

**Priority:** 7 (maximum)  
**Stack:** 128 bytes  
**Trigger:** Runs immediately when the scheduler starts (highest priority)

**What it does:**

Starts the RTI counter that drives all periodic events. This cannot be done before `vTaskStartScheduler()` because the RTI counter (once started) will immediately fire its compare interrupts, and the FreeRTOS port needs the scheduler to be running before any task can be woken from an ISR.

After starting the counter the task **deletes itself** — it has no further role.

```
rtiStartCounter(rtiCOUNTER_BLOCK0)  →  self-delete
```

**Why it exists:**  
Certain hardware must be started only after the FreeRTOS scheduler is live (interrupts re-enabled). Running at maximum priority guarantees this task executes before any other application task.

---

### `vDataUpdateTask` — CAN Data Ingestion (60 Hz)

**Priority:** 6  
**Stack:** 512 bytes  
**Trigger:** `EVT_PERIODIC_DATA_UPDATE` (RTI Compare 2, 60 Hz)

**What it does:**

1. Waits for the 60 Hz RTI tick.
2. Calls `update_data()` — reads all pending CAN mailboxes into the global data structs (`sensors_data`, `speed_data`, `dynamic_data`, `motor1_data`, `motor2_data`, `drivers_status_data`, `buttons_data`, `on_board_status_data`).
3. Calls `update_dashboard_data()` — converts the raw CAN data into floating-point dashboard values and appends them to the circular history buffers used by the graph page.
4. Calls `ui_handle_touch()` — reads the current touch position from the FT81x and updates the dashboard state (page swipes, button presses, calibration).
5. Takes `xDataMutex` and copies all global structs into the `AppData_t` snapshot used by lower-priority tasks.
6. Sets `EVT_NEW_DATA` to wake `vRulesTask`.

**Inputs:** Global CAN data structs (populated by `can.c` on CAN interrupt)  
**Outputs:** Dashboard data buffers, `AppData_t` snapshot, `EVT_NEW_DATA` event

**Notes:**  
The copy into `AppData_t` prevents lower-priority tasks from blocking `update_data()`. The data update runs at 60 Hz regardless of whether new CAN frames have arrived (the CAN mailbox read returns the last received frame if no new one is pending).

---

### `vRulesTask` — Compliance / Safety Checker

**Priority:** 5  
**Stack:** 512 bytes  
**Trigger:** `EVT_NEW_DATA`

**What it does (design intent):**

1. Takes `xDataMutex` and copies the safety-relevant sensor fields — TPS, front brake pressure, rear brake pressure — from the `AppData_t` snapshot.
2. Runs compliance checks on those values (e.g., simultaneous throttle and brake application, out-of-range sensor readings, etc.).
3. If data passes all checks, sets `EVT_DATA_APPROVED` to unlock `vDriversFSMTask`.
4. If a violation is detected, sets an alarm event instead.

**Current state:** The compliance check body is a `TODO` placeholder. The task currently passes `EVT_DATA_APPROVED` unconditionally after reading the data.

**Inputs:** `AppData_t` snapshot  
**Outputs:** `EVT_DATA_APPROVED` (pass) or alarm events (fail)

**Notes:**  
This task is the safety gate between sensing and actuation. `vDriversFSMTask` is not allowed to command motor output until this task approves the current data sample.

---

### `vDriversFSMTask` — Motor Driver State Machine

**Priority:** 3  
**Stack:** 1024 bytes (largest task stack — motor control logic is expected to grow)  
**Trigger:** `EVT_DATA_APPROVED`

**What it does (design intent):**

1. Takes `xDataMutex` and copies control-relevant fields: drive enable flag, motor 1/2 voltage/current, and the current drive state from the motor controllers.
2. Runs the motor driver FSM:
   - **OFF**: drive not enabled, no motor commands.
   - **ON**: drive enabled, compute and transmit motor torque/current setpoints via CAN mailboxes 3 and 4 (`MOTOR1_MAILBOX`, `MOTOR2_MAILBOX`).
   - **FAULT**: motor controller reported a fault, disable drive, signal alarm.

**Current state:** Placeholder — FSM logic body is not yet implemented.

**Inputs:** `AppData_t` snapshot, `EVT_DATA_APPROVED`  
**Outputs:** CAN Tx to motor controllers

**Notes:**  
The 1024-byte stack (double the other tasks) reflects the expectation that this will become the most computationally intensive task as torque control algorithms are added.

---

### `vRFDataRxTask` — RF Serial Receive and Dispatch

**Priority:** 4  
**Stack:** 512 bytes  
**Trigger:** `EVT_SCI_RX_READY` (set by SCI ISR on each received byte)

**What it does:**

1. Calls `sciReceive()` to start a 1-byte DMA/interrupt-driven reception.
2. Blocks on `EVT_SCI_RX_READY`.
3. When a byte arrives, inspects the data ID byte to determine the packet type:
   - `CMD_ID (0x01)` → sets `EVT_SCI_CMD` to wake `vCmdTask`.
   - `TELEMETRY_ACK_ID (0x7E)` → sets `EVT_SCI_TELEMETRY_ACK` to wake `vTelemetryTask`.
4. Loops back to start the next reception immediately.

**Current state:** The packet identification logic (reading the ID from the receive buffer) is a `TODO` placeholder. The dispatch switch is wired but the actual buffer parsing is not yet implemented.

**Inputs:** XBee RF module serial data (SCI Rx interrupt)  
**Outputs:** `EVT_SCI_CMD`, `EVT_SCI_TELEMETRY_ACK`

**Notes:**  
Only 1 byte is requested per `sciReceive()` call. Full packet framing (COBS decode + MessagePack unpack) is expected to be implemented here once the RF protocol is finalized.

---

### `vCmdTask` — Remote Command Processor

**Priority:** 3  
**Stack:** 512 bytes  
**Trigger:** `EVT_SCI_CMD` (set by `vRFDataRxTask`)

**What it does (design intent):**

Receives a 16-byte command packet dispatched by `vRFDataRxTask`, validates it, and executes the commanded action (e.g., enable/disable telemetry, change drive mode, request calibration).

**Current state:** Placeholder — command processing body is not yet implemented.

**Inputs:** `EVT_SCI_CMD`, 16-byte `xRxBuff`  
**Outputs:** (TBD — depends on command type)

---

### `vTelemetryTask` — RF Telemetry Transmitter (50 Hz)

**Priority:** 2  
**Stack:** 512 bytes  
**Trigger:** `EVT_PERIODIC_TELEMETRY` (RTI Compare 3, 50 Hz), `EVT_SCI_TELEMETRY_ACK`, `EVT_TELEMETRY_NACK`

**What it does:**

1. Waits for any of: the 50 Hz tick, an ACK from the ground station, or a transmit timeout (NACK).
2. On a 50 Hz tick (or NACK retransmit):
   a. Takes `xDataMutex` and copies telemetry fields — RPM, battery voltage/current, front-left and front-right wheel speeds, motor fault codes — from the `AppData_t` snapshot.
   b. Packs the data into `xTxBuff` (MessagePack format — currently placeholder, buffer is zero-filled).
   c. Transmits the buffer via `sciSend()` to the XBee.
   d. Starts `xTxTimeoutTimer` (5 ms). If no ACK arrives within 5 ms, the timer fires `EVT_TELEMETRY_NACK` and this task retransmits.
3. On ACK (`EVT_SCI_TELEMETRY_ACK`): stops the timeout timer, considers the packet delivered.

**Current state:** Data collection and transmission are wired; MessagePack serialization is a `TODO` placeholder (buffer is sent as zeros).

**Inputs:** `AppData_t` snapshot, RTI 50 Hz tick  
**Outputs:** SCI serial Tx (XBee)

**Notes:**  
The task does **not** clear `EVT_PERIODIC_TELEMETRY` on entry (`pdFALSE` for the clear-on-exit parameter). This means if the task is busy when the next 50 Hz tick arrives, the bit stays set and the task will re-run immediately after finishing — trading strict period accuracy for guaranteed eventual transmission.

---

### `vDisplayTask` — Touchscreen Dashboard Renderer (30 Hz)

**Priority:** 1  
**Stack:** 768 bytes  
**Trigger:** `EVT_PERIODIC_DISPLAY_UPDATE` (RTI Compare 1, 30 Hz)

**What it does:**

Waits for the 30 Hz RTI tick, then calls `update_dashboard_draw()`. This function renders the current page to the FT81x display using its SPI interface. Four pages are available:

- **RACE** — main driving display: RPM bar, motor voltages/currents/temps, TPS, brake pressures, wheel speeds, drive state and fault indicators.
- **TELEMETRY** — battery voltage/current, all four wheel speeds, RPM.
- **GRAPH** — scrolling history graph of any one of 15 selectable metrics.
- **DEBUG** — raw sensor values and calibration controls.

The page is changed by swiping the touchscreen (handled in `vDataUpdateTask` via `ui_handle_touch()`).

**Inputs:** `dashboard_data` global (populated by `vDataUpdateTask`)  
**Outputs:** FT81x SPI commands (display update)

**Notes:**  
Display rendering at 30 Hz is the lowest-priority task in the system (priority 1, same as the disabled `vUIInputTask`). This ensures that all safety-critical and control tasks run to completion before the display is updated.

---

### `vUIInputTask` — Touch Input Processor (Disabled)

**Priority:** 1  
**Stack:** 256 bytes  
**Trigger:** N/A  
**Status:** **Disabled** — creation is commented out in `vCreateTasks()`

**Design intent:**

Was intended to handle touch input processing in its own task. Touch handling was moved into `vDataUpdateTask` (via `ui_handle_touch()`) to keep input processing synchronized with the data update cycle. This task remains in the code as a placeholder in case the touch handling needs to be decoupled again.

---

## Scheduling Diagram

```
RTI Compare 1 (30 Hz)  ──────────────────────► EVT_PERIODIC_DISPLAY_UPDATE ──► vDisplayTask (P1)
RTI Compare 2 (60 Hz)  ──────────────────────► EVT_PERIODIC_DATA_UPDATE ──────► vDataUpdateTask (P6)
                                                     │
                                                     └─► EVT_NEW_DATA ──────────► vRulesTask (P5)
                                                                                       │
                                                                                       └─► EVT_DATA_APPROVED ──► vDriversFSMTask (P3)
RTI Compare 3 (50 Hz)  ──────────────────────► EVT_PERIODIC_TELEMETRY ─────────► vTelemetryTask (P2)
SCI Rx ISR             ──────────────────────► EVT_SCI_RX_READY ────────────────► vRFDataRxTask (P4)
                                                     │
                                          ┌──────────┴──────────────┐
                                          ▼                         ▼
                                    EVT_SCI_CMD              EVT_SCI_TELEMETRY_ACK
                                          │                         │
                                          ▼                         ▼
                                    vCmdTask (P3)           vTelemetryTask (P2)
```

Higher priority number = runs first when multiple tasks are ready.
