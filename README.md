# rtecli-tui

An attempt at a terminal user interface (tui) wrapping Netronome's `rtecli`. Intended to easily monitor and interact with registers and counters values. Built with [FTXUI](https://github.com/ArthurSonzogni/FTXUI) and [nlohmann/json](https://github.com/nlohmann/json). 

> [!WARNING]
> This is an ongoing work quickly putted together as a proof of concept/experiment! Bugs and flaws are expected! 

---

## Usage 

```
make configure
make
make run REMOTEHOST=<target host>
```

---

## Known issues:
- Program hangs if host/rte service is down

## TODO:

- [ ] Improve layout
- [x] Allow to clear registers/counters
- [ ] Improve error handling
    - [ ] Response timeout
- [ ] Support Registers count > 1
- [ ] Support Meters
- [x] Support Multicast Groups
- [x] Support Port Information
- [ ] Allow to adjust update interval **?**
- [ ] Handle integer registers **?** (We can't determine if a register is either an unsigned or an integer, so this would require some type of manual input) 
