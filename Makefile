EXE=rtecli-tui
REMOTEHOST=<host>

.PHONY: build
build:
	@make -j$(nproc) -C build/

.PHONY: run
run: build
	@./build/$(EXE) $(REMOTEHOST)

.PHONY: configure
configure:
	@mkdir -p build/
	@cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build/

.PHONY: clean
clean:
	@cmake --build build/ --target clean
