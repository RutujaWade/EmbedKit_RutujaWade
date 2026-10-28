# Ring Buffer Implementation in C

**Name:** Rutuja Wade  
**Project:** Embedded Developer Fresher Assessment  
**Language:** C (C99)  
**Buffer Size:** 8 Bytes  

## 1. Project Description

This project implements an 8-byte ring buffer using C. A ring buffer is a circular data structure that follows the **FIFO (First In, First Out)** principle. It allows data to be written, read, and stored temporarily while reusing memory locations.

## 2. Code Explanation

- **`ringbuf_init()`** – Initializes the buffer by setting `head`, `tail`, and `count` to zero.
- **`ringbuf_is_empty()`** – Checks whether the buffer contains no data.
- **`ringbuf_is_full()`** – Checks whether the buffer has reached its maximum capacity.
- **`ringbuf_count()`** – Returns the number of bytes currently stored.
- **`ringbuf_write()`** – Adds a byte to the buffer if space is available. It returns failure if the buffer is full.
- **`ringbuf_read()`** – Reads a byte from the buffer if data is available. It returns failure if the buffer is empty.
- **`main()`** – Demonstrates writing eight bytes, testing overflow, reading three bytes, writing new bytes, reading the remaining data, and testing underflow.

## 3. Bitwise AND for Wrap-Around

The program uses bitwise AND (`&`) instead of modulo (`%`) to update the buffer indexes.

```c
index = (index + 1u) & (BUFFER_SIZE - 1u);
```

Since the buffer size is 8, this operation wraps the index back to zero after reaching index 7. This method works because 8 is a power of two.

## 4. Test Cases

1. Write eight bytes (`0x41` to `0x48`).
2. Attempt to write `0x99` when the buffer is full.
3. Read the first three bytes.
4. Write three new bytes (`0x49`, `0x4A`, `0x4B`).
5. Read all remaining bytes in FIFO order.
6. Attempt to read from an empty buffer.

## 5. Requirements

- GCC compiler
- C99 support

## 6. Compile and Run

**Compile:**

```bash
gcc -Wall -Wextra -std=c99 ringbuf.c -o ringbuf.exe
```

**Run on Windows:**

```cmd
ringbuf.exe
```

## 7. Conclusion

This project demonstrates FIFO operation, buffer status checking, overflow and underflow handling, and memory reuse using a circular buffer. It also demonstrates efficient index wrap-around using bitwise AND.