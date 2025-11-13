# Multi-Threaded CLI Image Processor

Command-line tool written in C++ that converts BMP images to grayscale using a data-parallel, multi-threaded processing model. The project evolves over three phases:

- **Phase 1 – Core:** Build a single-threaded grayscale converter to validate BMP parsing and file I/O.
- **Phase 2 – Performance:** Parallelize the filter using `std::thread`, partitioning the image into horizontal chunks to leverage all CPU cores and benchmark the speedup.
- **Phase 3 – Demo:** Polish the repository with benchmarks, sample inputs, before/after visuals, and documentation suitable for recruiters.

### Current Capabilities

- Parses 24-bit BMP headers, loads pixel data, and writes valid BMP output.
- Applies the grayscale (luminosity) filter in single-thread or multi-thread modes.
- Benchmarks single vs. multi-thread execution using `<chrono>` and reports speedup.
- Provides CLI argument parsing with optional thread-count override.
- Includes a tiny `assets/sample_2x2.bmp` for quick manual testing.

### Getting Started

```bash
make            # build the image_processor binary
./image_processor -i input.bmp -o output.bmp -t 8
```

The `-t/--threads` flag is optional. When omitted, the application falls back to `std::thread::hardware_concurrency()`.

> macOS prerequisite: make sure the Xcode Command Line Tools are installed (`xcode-select --install`) so the standard C++ headers are available to the compiler.

verify the pipeline quickly with below command:

```bash
./image_processor -i assets/sample_2x2.bmp -o sample_gray.bmp -t 4
```

Example output:

```
Input file:  assets/sample_2x2.bmp
Output file: sample_gray.bmp
Threads:     4
Single-thread: 0.000 ms
4-thread:      0.119 ms
Speedup:      x0.001
```

```
sample output exp:
Single-thread: 0.000 ms: time to apply grayscale filter using single thread. Image is only four pixels, so this finishes in less than timer’s precision (rounded to 0.000 ms).

4-thread: 0.119 ms: time using four threads, including splitting work & creating those threads; on  tiny image, coordination overhead dominates.

Speedup: x0.001: ratio single_ms / multi_ms. Cause single-thread version was instantaneous while  multi-thread run had overhead, it reports a speedup much less than 1 (ie. parallel run was slower). On a large image, this value rises above 1, cause parallel advantage.

```

### Tests

```bash
make test
```

Runs lightweight CLI parsing checks.