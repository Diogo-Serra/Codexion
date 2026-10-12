#!/bin/bash
# Comprehensive Test Suite for Codexion

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

run_test() {
    local name=$1
    local args=$2
    local expected_exit=$3
    echo -n "Running $name... "
    output=$(./codexion $args 2>&1)
    exit_code=$?
    if [ $exit_code -eq $expected_exit ]; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL${NC} (Expected $expected_exit, got $exit_code)"
        echo "Output: $output"
        return 1
    fi
}

echo "--- Functional Tests ---"
run_test "Basic FIFO" "3 1000 100 100 100 1 50 fifo" 0
run_test "Basic EDF" "3 1000 100 100 100 1 50 edf" 0
run_test "Single Coder" "1 1000 100 100 100 1 50 fifo" 0

echo -e "\n--- Edge Cases ---"
run_test "High Contention Burnout" "5 200 100 100 100 10 10 fifo" 0
run_test "Completion by Count" "2 5000 10 10 10 2 10 fifo" 0
run_test "Tight Burnout Window" "3 150 100 10 10 1 50 fifo" 0

echo -e "\n--- Validation Tests ---"
run_test "Invalid Num Coder" "-1 1000 100 100 100 1 50 fifo" 1
run_test "Invalid Burnout" "3 abc 100 100 100 1 50 fifo" 1
run_test "Invalid Scheduler" "3 1000 100 100 100 1 50 random" 1
run_test "Missing Args" "3 1000 100" 1

echo -e "\nAll tests completed."
