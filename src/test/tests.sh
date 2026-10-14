#!/bin/bash
# Test Suite for Codexion

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

# 1. Basic functional test (FIFO)
run_test "Basic FIFO" "3 1000 100 100 100 1 50 fifo" 0

# 2. Basic functional test (EDF)
run_test "Basic EDF" "3 1000 100 100 100 1 50 edf" 0

# 3. Single Coder (Edge Case)
run_test "Single Coder" "1 1000 100 100 100 1 50 fifo" 0

# 4. Invalid Inputs (Negative/Non-int/Scheduler)
run_test "Invalid Num Coder" "-1 1000 100 100 100 1 50 fifo" 1
run_test "Invalid Burnout" "3 abc 100 100 100 1 50 fifo" 1
run_test "Invalid Scheduler" "3 1000 100 100 100 1 50 random" 1

# 5. High Contention / Burnout Scenario
# 5 coders, very short burnout time relative to compile/debug/refactor
run_test "High Contention Burnout" "5 200 100 100 100 10 10 fifo" 0

# 6. Tight Timeline (Should finish by compilation count)
run_test "Completion by Count" "2 5000 10 10 10 2 10 fifo" 0

echo "All tests completed."
