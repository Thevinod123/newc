#!/bin/bash
set -euo pipefail

# Configuration
SOLUTION_FILE="solution.c"
TEST_FILE="test.c"
TIMEOUT=30
MEMORY_LIMIT="512MB"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Logging function
log() { echo -e "${GREEN}[INFO]${NC} $1"; }
error() { echo -e "${RED}[ERROR]${NC} $1"; exit 1; }
warn() { echo -e "${YELLOW}[WARN]${NC} $1"; }

# Check if files exist
if [[ ! -f "$SOLUTION_FILE" ]]; then
    error "Solution file $SOLUTION_FILE not found"
fi

if [[ ! -f "$TEST_FILE" ]]; then
    error "Test file $TEST_FILE not found"
fi

log "Compiling solution and tests..."
gcc -std=gnu11 -O2 "$SOLUTION_FILE" "$TEST_FILE" -o run_tests -lm

log "Running tests..."
set +e
timeout "$TIMEOUT" ./run_tests
TEST_EXIT_CODE=$?
set -e

if [[ $TEST_EXIT_CODE -eq 0 ]]; then
    echo exit code 0
    exit 0
else
    echo exit code 1
    exit 1
fi

