#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 8u

typedef struct
{
    uint8_t data[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} RingBuffer;

/* Initialise the ring buffer. */
void ringbuf_init(RingBuffer *buffer)
{
    buffer->head = 0u;
    buffer->tail = 0u;
    buffer->count = 0u;
}

/* Return 1 if the buffer is empty; otherwise return 0. */
uint8_t ringbuf_is_empty(const RingBuffer *buffer)
{
    return (uint8_t)(buffer->count == 0u);
}

/* Return 1 if the buffer is full; otherwise return 0. */
uint8_t ringbuf_is_full(const RingBuffer *buffer)
{
    return (uint8_t)(buffer->count == BUFFER_SIZE);
}

/* Return the number of bytes stored in the buffer. */
uint8_t ringbuf_count(const RingBuffer *buffer)
{
    return buffer->count;
}

/* Write one byte. Return 0 if the buffer is full. */
uint8_t ringbuf_write(RingBuffer *buffer, uint8_t value)
{
    if (ringbuf_is_full(buffer))
    {
        return 0u;
    }

    buffer->data[buffer->head] = value;

    /*
     * BONUS:
     * Bitwise AND replaces modulo for wrap-around.
     * It can be faster on MCUs without a hardware divider.
     * This works because BUFFER_SIZE is a power of 2.
     */
    buffer->head =
        (uint8_t)((buffer->head + 1u) & (BUFFER_SIZE - 1u));

    buffer->count++;

    return 1u;
}

/* Read one byte. Return 0 if the buffer is empty. */
uint8_t ringbuf_read(RingBuffer *buffer, uint8_t *value)
{
    if (ringbuf_is_empty(buffer))
    {
        return 0u;
    }

    *value = buffer->data[buffer->tail];

    buffer->tail =
        (uint8_t)((buffer->tail + 1u) & (BUFFER_SIZE - 1u));

    buffer->count--;

    return 1u;
}

int main(void)
{
    RingBuffer buffer;
    uint8_t value;
    uint8_t i;

    const uint8_t initial_data[BUFFER_SIZE] =
    {
        0x41u, 0x42u, 0x43u, 0x44u,
        0x45u, 0x46u, 0x47u, 0x48u
    };

    const uint8_t new_data[3] =
    {
        0x49u, 0x4Au, 0x4Bu
    };

    /* Step 1: Initialise and write 8 bytes. */
    ringbuf_init(&buffer);

    printf("=== STEP 1: WRITE 8 BYTES ===\n");

    for (i = 0u; i < BUFFER_SIZE; i++)
    {
        if (ringbuf_write(&buffer, initial_data[i]))
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)",
                   (unsigned int)initial_data[i],
                   (unsigned int)ringbuf_count(&buffer));

            if (ringbuf_is_full(&buffer))
            {
                printf(" FULL");
            }

            printf("\n");
        }
    }

    /* Step 2: Attempt to write to a full buffer. */
    printf("\n=== STEP 2: WRITE WHEN FULL ===\n");

    if (!ringbuf_write(&buffer, 0x99u))
    {
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");
    }

    /* Step 3: Read 3 bytes. */
    printf("\n=== STEP 3: READ 3 BYTES ===\n");

    for (i = 0u; i < 3u; i++)
    {
        if (ringbuf_read(&buffer, &value))
        {
            printf("[READ]    -> 0x%02X (count=%u)\n",
                   (unsigned int)value,
                   (unsigned int)ringbuf_count(&buffer));
        }
    }

    /* Step 4: Write 3 new bytes into the freed slots. */
    printf("\n=== STEP 4: WRITE 3 NEW BYTES ===\n");

    for (i = 0u; i < 3u; i++)
    {
        if (ringbuf_write(&buffer, new_data[i]))
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)",
                   (unsigned int)new_data[i],
                   (unsigned int)ringbuf_count(&buffer));

            if (ringbuf_is_full(&buffer))
            {
                printf(" FULL");
            }

            printf("\n");
        }
    }

    /* Step 5: Read all 8 remaining bytes. */
    printf("\n=== STEP 5: READ REMAINING BYTES ===\n");

    while (!ringbuf_is_empty(&buffer))
    {
        if (ringbuf_read(&buffer, &value))
        {
            printf("[READ]    -> 0x%02X (count=%u)\n",
                   (unsigned int)value,
                   (unsigned int)ringbuf_count(&buffer));
        }
    }

    printf("Buffer empty: %s\n",
           ringbuf_is_empty(&buffer) ? "YES" : "NO");

    /* Step 6: Attempt to read from an empty buffer. */
    printf("\n=== STEP 6: READ WHEN EMPTY ===\n");

    if (!ringbuf_read(&buffer, &value))
    {
        printf("[READ] (empty) -> FAIL (buffer empty)\n");
    }

    return 0;
}