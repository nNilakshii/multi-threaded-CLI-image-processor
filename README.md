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

You can verify the pipeline quickly:

```bash
./image_processor -i assets/sample_2x2.bmp -o sample_gray.bmp -t 4
```

Example output:

```
Input file:  assets/sample_2x2.bmp
Output file: sample_gray.bmp
Threads:     4
Single-thread: 0.016 ms
4-thread:      0.012 ms
Speedup:      x1.333
```

### Tests

```bash
make test
```

Runs lightweight CLI parsing checks.

### Roadmap

1. Harden error handling and add additional unit tests (BMP I/O, pixel math).
2. Collect real benchmark data across different resolutions and hardware.
3. Add README benchmarks, sample assets, and optional demo media.