#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint8_t state;
    uint16_t sig;
    uint8_t next;
} row_t;

enum
{
    IDLE,
    RUNNING,
    PAUSED,
    ERROR
};

enum
{
    START = 1,
    PAUSE,
    RESUME,
    STOP,
    FAIL,
    RESET
};

static const row_t rows[] =
{
    {
        IDLE, START, RUNNING
    },
    {
        RUNNING,
        PAUSE,
        PAUSED
    },
    {
        PAUSED,
        RESUME,
        RUNNING
    },
    {
        RUNNING,
        STOP,
        IDLE
    },
    {
        PAUSED,
        STOP,
        IDLE
    },
    {
        IDLE,
        FAIL,
        ERROR
    },
    {
        RUNNING,
        FAIL,
        ERROR
    },
    {
        PAUSED,
        FAIL,
        ERROR
    },
    {
        ERROR,
        RESET,
        IDLE
    }
};
static uint8_t state = IDLE;

static int dispatch(uint16_t sig)
{
    for (unsigned i = 0; i < sizeof(rows) / sizeof(rows[0]); i++)
    {
        if (rows[i].state == state && rows[i].sig == sig)
        {
            state = rows[i].next;
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    assert(dispatch(START));
    assert(dispatch(PAUSE));
    assert(dispatch(RESUME));
    assert(dispatch(FAIL));
    assert(dispatch(RESET));
    assert(state == IDLE);
    puts("TSM PASS");
    return 0;
}
