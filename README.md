# EMPEROR-J

**Champion of the IranOpen 2D Simulated Football League**  
**RoboCup Soccer Simulation 2D Team**

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![RoboCup](https://img.shields.io/badge/RoboCup-Soccer%202D-green.svg)](https://www.robocup.org/)

A competitive multi-agent team for the **RoboCup Soccer Simulation 2D League**.  
Built with modern `librcsc` and C++, featuring advanced agent coordination, formation strategies, and real-time decision-making.

Compatible with **rcssserver-16** and later.

---

## 🏆 Achievements

- Champion of the **IranOpen 2D Simulated Football League**
- Participated and ranked in **RoboCup 2023** (2D Soccer Simulation)

---

## Features

- Advanced multi-agent coordination and teamwork
- Dynamic formation and strategy management
- Optimized for competitive performance
- Clean and maintainable C++ codebase based on librcsc

---

## Quick Start

### Dependencies

- Boost 1.38 or later → [boost.org](https://www.boost.org/)
- Latest [librcsc](https://github.com/helios-base/librcsc)

### Installation (Ubuntu 16.04+)

```bash
sudo apt update
sudo apt install build-essential libboost-all-dev
```

Install librcsc, then from the project root:

```bash
./bootstrap
./configure
make
```

### Running the Team

```bash
cd src
./start.sh
```

---

## Configuration

If librcsc is installed in a non-standard location:

```bash
./configure --with-librcsc=/path/to/librcsc
```

You may also need to set the `LD_LIBRARY_PATH` environment variable.

See `./configure --help` for more options.

---

## References

- Erfan Fathi, Soroush Mazloum, and Taha Shahsavand: *EMPEROR-J Team*

---

## Contact

For questions or collaboration: **EmperorTmRCSS@gmail.com**

---

### Acknowledgments

Part of the RoboCup community.  
- Official RoboCup Soccer Simulator: [rcsoccersim](https://github.com/rcsoccersim)  
- RoboCup Official Website: [robocup.org](https://www.robocup.org)
