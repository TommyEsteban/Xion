<h1 style="display:flex;align-items:center;gap:12px"> 
  <img src="assets/icons/xion-logo.svg" width="64" alt="Xion logo" style="vertical-align:middle;"/>
  Xion — Viola–Jones Object Detection Framework
</h1>

A compact implementation of the Viola–Jones object detection framework (AdaBoost + cascade), written in C with optional CUDA acceleration. This repository contains the original project sources and tests written before the era of generative AI.

<p>
<strong>Quick highlights</strong>
</p>

- <img src="assets/icons/section-requirements.svg" width="18" style="vertical-align:middle;margin-right:8px;" alt="language"/> Language: C/C99 with optional CUDA (.cu) components
- <img src="assets/icons/section-build.svg" width="18" style="vertical-align:middle;margin-right:8px;" alt="build"/> Build system: CMake (>= 3.0.2)
- <img src="assets/icons/section-deps.svg" width="18" style="vertical-align:middle;margin-right:8px;" alt="deps"/> Image support: libjpeg
- <img src="assets/icons/section-tests.svg" width="18" style="vertical-align:middle;margin-right:8px;" alt="binaries"/> Binaries: `bin/xion` (runtime) and `bin/xionTest` (tests)

<h2><img src="assets/icons/section-status.svg" width="20" style="vertical-align:middle;margin-right:8px;" alt="status"/> Status</h2>

This is an archival/educational codebase implementing the classic Viola–Jones pipeline: dataset generation, integral images, feature generation, AdaBoost training, and cascade construction. It predates common generative AI tools and reflects human-authored, academic-style code.

<h2><img src="assets/icons/section-requirements.svg" width="20" style="vertical-align:middle;margin-right:8px;" alt="requirements"/> Requirements</h2>

- C compiler supporting GNU99
- CMake >= 3.0.2
- libjpeg development headers (e.g. `libjpeg-dev`)
- (Optional) CUDA toolkit for GPU acceleration

On Debian/Ubuntu:

```sh
sudo apt-get install build-essential cmake libjpeg-dev
# install NVIDIA CUDA separately if you want GPU support
```

<h2><img src="assets/icons/section-build.svg" width="20" style="vertical-align:middle;margin-right:8px;" alt="build"/> Build</h2>

From the repository root:

```sh
mkdir -p build && cd build
cmake ..
make -j
```

Binaries are placed under `bin/` by the CMake configuration.

<h2><img src="assets/icons/section-tests.svg" width="20" style="vertical-align:middle;margin-right:8px;" alt="tests"/> Tests</h2>

This project registers tests with CTest. Run them with:

```sh
ctest --output-on-failure
# or run specific test binaries:
bin/xionTest dataset
bin/xionTest preprocessing
```

<h2><img src="assets/icons/section-usage.svg" width="20" style="vertical-align:middle;margin-right:8px;" alt="usage"/> Basic usage</h2>

Run the main binary `bin/xion`. The CLI supports these top-level modes:

- `data` — dataset generation
- `training adaboost` / `training cascade` — training modes
- `test adaboost` / `test cascade` — evaluation modes

Common flags include `-size`, `-tposdata`, `-tnegdata`, `-vposdata`, `-vnegdata`, `-cl`, `-ff`, `-features`, and `-p` (GPU parallelization).

Example (AdaBoost training):

```sh
bin/xion training adaboost -tposdata path/to/pos.dat -tnegdata path/to/neg.dat -size 24 -features 100 -ff features -cl classifiers.cl
```

<h2><img src="assets/icons/section-layout.svg" width="20" style="vertical-align:middle;margin-right:8px;" alt="layout"/> Project layout (selected)</h2>

- [include](include/) — public headers and configuration templates
- [modules](modules/) — implementation modules (dataset, features, frameworks, preprocessing, CUDA helpers)
- [data/testing](data/testing) — small test datasets
- [test](test/) — unit/integration tests
- [CMakeLists.txt](CMakeLists.txt#L1-L40) — build
- [modules/frameworks/main.c](modules/frameworks/main.c#L1-L120) — program entry and CLI parsing

<h2><img src="assets/icons/section-deps.svg" width="20" style="vertical-align:middle;margin-right:8px;" alt="deps"/> Dependencies & notes discovered in the code</h2>

- CMake searches for `JPEG` and `CUDA` packages (see `CMakeLists.txt`).
- Tests are added with `add_test` for `dataset` and `preprocessing`.
- Binaries and libraries are configured to output to `bin/` and `lib/` respectively.

<h2><img src="assets/icons/book.svg" width="20" style="vertical-align:middle;margin-right:8px;" alt="contrib"/> Running locally</h2>

Anyone can run this locally with CTest:

```sh
ctest --output-on-failure
```

<h2><img src="assets/icons/xion-logo.svg" width="20" style="vertical-align:middle;margin-right:8px;" alt="history"/> Attribution & History</h2>

This repository appears to be a personal or academic implementation of the Viola–Jones framework, and it predates the modern generative AI era. The code and tests are hand-authored and reflect classic image-detection pipeline techniques from the time when real-time object and face detection was emerging as a practical computer vision milestone.

The original Viola–Jones algorithm was introduced by Paul Viola and Michael Jones in 2001, and it was especially useful for fast face detection in webcam systems, image search, and early surveillance or robotics applications. This project follows that same spirit: classical feature generation, AdaBoost-based classifier training, and cascade evaluation for speed.

Today, this matters because the same ideas still influence modern real-time vision systems: efficient feature extraction, cascaded classifiers, and lightweight detection pipelines remain useful in mobile, embedded, and low-latency applications.

---

For more background, see the Viola–Jones algorithm on [Wikipedia](https://en.wikipedia.org/wiki/Viola%E2%80%93Jones_object_detection_framework).
