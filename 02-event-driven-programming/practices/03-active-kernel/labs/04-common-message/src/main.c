#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef struct
{
    uint16_t x,
    y;
    uint8_t health;
} player_status_t;
typedef struct
{
    uint8_t len;
    uint8_t data[24];
} message_t;
static bool receive(const message_t *m, player_status_t *out)
{
    if (m->len != sizeof (*out))
    {
        return false;
    }
    memcpy(out, m->data, sizeof (*out));
    return true;
}

int main(void)
{
    player_status_t src =
    {
        10U,
        20U,
        99U
    },
    dst =
    {
        0
    };
    message_t m =
    {
        .len = sizeof(src)
    };
    memcpy(m.data, &src, sizeof(src));
    assert(receive(&m, &dst));
    assert(dst.health == 99U);
    m.len--;
    assert(!receive(&m, &dst));
    puts("common message PASS");
    return 0;
}
