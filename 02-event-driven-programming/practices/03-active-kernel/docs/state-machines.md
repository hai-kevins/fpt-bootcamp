# State Machines

## Application FSM

```text
BOOT --SYSTEM_START--> IDLE
IDLE --BUTTON_PRESSED--> ACTIVE
ACTIVE --BUTTON_PRESSED--> IDLE
IDLE/ACTIVE --APP_ERROR--> ERROR
ERROR --APP_RESET--> IDLE
```

## LED state

```text
OFF --LED_ON--> ON
ON --LED_OFF--> OFF
OFF/ON --LED_BLINK_START--> BLINKING
BLINKING --LED_BLINK_TICK--> toggle physical LED
BLINKING --LED_BLINK_STOP--> OFF
```

Every state transition must cancel timers that no longer belong to the old state.
