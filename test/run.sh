#!/bin/bash

set -e

testdir=(
    ./target/main
    ./target/test
)

tests_ignore=(
    main_server.out
    main_client.out

    test_network_server.out
    test_network_client.out
)

run_tests() {
    local dir="$1"
    for test in "$dir"/*; do
        filename=$(basename "$test")
        if [[ " ${tests_ignore[@]} " =~ " ${filename} " ]]; then
            echo -e "\033[1;33mSkipping: ${filename}\033[0m"
            continue
        fi
        echo -e "\033[1;34mRunning: ${test}\033[0m"
        "${test}" || exit 1
    done
}

for dir in "${testdir[@]}"; do
    if [ -d "$dir" ]; then
        run_tests "$dir"
        echo -e "\033[1;32mAll tests passed in: ${dir}\033[0m"
    else
        echo -e "\033[1;31mDirectory not found: ${dir}\033[0m"
    fi
done
