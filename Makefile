CC ?= gcc
CFLAGS ?= -O3 -Wall -Wextra
OMPFLAGS ?= -fopenmp

PATTERN_DIR := test/pattern-files
TARGET_DIR := test/files-to-match
REFERENCE_SOURCE := reference/busca-cadeia.c
SOURCE_PROGRAMS := $(patsubst src/%.c,%,$(wildcard src/*.c))

RUNALL_GOAL := $(firstword $(filter runall-%,$(MAKECMDGOALS)))
RUN_GOAL := $(firstword $(filter run-%,$(MAKECMDGOALS)))
ifeq ($(RUNALL_GOAL),)
ifeq ($(RUN_GOAL),)
PROGRAM := $(firstword $(filter-out all clean run-% runall-%,$(MAKECMDGOALS)))
else
PROGRAM := $(patsubst run-%,%,$(RUN_GOAL))
endif
else
PROGRAM := $(patsubst runall-%,%,$(RUNALL_GOAL))
endif
INPUT_SUFFIX := $(firstword $(filter-out all clean $(PROGRAM) $(RUN_GOAL) $(RUNALL_GOAL),$(MAKECMDGOALS)))
ifeq ($(INPUT_SUFFIX),)
PATTERN_FILE := $(PATTERN_DIR)/arquivo1
TARGET_FILE := $(TARGET_DIR)/arquivo2-0
else
HYPHEN_PATTERN := $(PATTERN_DIR)/arquivo1-$(INPUT_SUFFIX)
HYPHEN_TARGET := $(TARGET_DIR)/arquivo2-$(INPUT_SUFFIX)
PLAIN_PATTERN := $(PATTERN_DIR)/arquivo1$(INPUT_SUFFIX)
PLAIN_TARGET := $(TARGET_DIR)/arquivo2$(INPUT_SUFFIX)
ifneq ($(and $(wildcard $(HYPHEN_PATTERN)),$(wildcard $(HYPHEN_TARGET))),)
PATTERN_FILE := $(HYPHEN_PATTERN)
TARGET_FILE := $(HYPHEN_TARGET)
else ifneq ($(and $(wildcard $(PLAIN_PATTERN)),$(wildcard $(PLAIN_TARGET))),)
PATTERN_FILE := $(PLAIN_PATTERN)
TARGET_FILE := $(PLAIN_TARGET)
else
PATTERN_FILE := $(HYPHEN_PATTERN)
TARGET_FILE := $(HYPHEN_TARGET)
endif
endif

.PHONY: all clean $(INPUT_SUFFIX)

all:
	@echo "Use 'make <program> [suffix]', 'make run-<program> [suffix]', or 'make runall-<program>'."
	@echo "Programs in src/ are built by filename; e.g. 'make optimized_sequential grp-t1'."

run-%: %
	@echo "./$* \"$(PATTERN_FILE)\" \"$(TARGET_FILE)\""
	@./$* "$(PATTERN_FILE)" "$(TARGET_FILE)"

runall-%: %
	@set -e; count=0; \
	for pattern in "$(PATTERN_DIR)"/arquivo1*; do \
		[ -f "$$pattern" ] || continue; \
		name=$${pattern##*/}; \
		case "$$name" in \
			arquivo1) target="$(TARGET_DIR)/arquivo2-0" ;; \
			arquivo1-*) suffix=$${name#arquivo1-}; target="$(TARGET_DIR)/arquivo2-$$suffix"; \
				if [ ! -f "$$target" ]; then target="$(TARGET_DIR)/arquivo2$$suffix"; fi ;; \
			arquivo1*) suffix=$${name#arquivo1}; target="$(TARGET_DIR)/arquivo2$$suffix" ;; \
		esac; \
		if [ ! -f "$$target" ]; then echo "Missing target file for $$pattern: $$target"; exit 1; fi; \
		echo "./$* \"$$pattern\" \"$$target\""; \
		./$* "$$pattern" "$$target"; \
		count=$$((count + 1)); \
	done; \
	if [ "$$count" -eq 0 ]; then echo "No pattern files found in $(PATTERN_DIR)"; exit 1; fi

$(PROGRAM): $(if $(filter busca_cadeia,$(PROGRAM)),$(REFERENCE_SOURCE),src/$(PROGRAM).c)
	@set -e; source="$(if $(filter busca_cadeia,$@),$(REFERENCE_SOURCE),src/$@.c)"; \
	if [ ! -x "$@" ] || [ "$$source" -nt "$@" ]; then \
		if [ -z "$(RUNALL_GOAL)" ]; then \
			if [ ! -f "$(PATTERN_FILE)" ]; then echo "Pattern file not found: $(PATTERN_FILE)"; exit 1; fi; \
			if [ ! -f "$(TARGET_FILE)" ]; then echo "Target file not found: $(TARGET_FILE)"; exit 1; fi; \
		fi; \
		if grep -Eq '^[[:space:]]*#[[:space:]]*include[[:space:]]*[<"]omp\.h[>"]|^[[:space:]]*#[[:space:]]*pragma[[:space:]]+omp' "$$source"; then \
			echo "Compiling $$source with OpenMP"; \
			$(CC) $(CFLAGS) $(OMPFLAGS) -o "$@" "$$source"; \
		else \
			echo "Compiling $$source as C"; \
			$(CC) $(CFLAGS) -o "$@" "$$source"; \
		fi; \
		if [ -z "$(RUN_GOAL)" ] && [ -z "$(RUNALL_GOAL)" ]; then ./$@ "$(PATTERN_FILE)" "$(TARGET_FILE)"; fi; \
	fi

ifneq ($(INPUT_SUFFIX),)
$(INPUT_SUFFIX):
	@:
endif

clean:
	rm -f busca_cadeia $(SOURCE_PROGRAMS)
