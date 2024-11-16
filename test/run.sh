#!/bin/bash

set -e

tests=(
    ./target/main/main_pool.out
    ./target/main/main_data_buffer.out
    ./target/main/main_memento.out
    ./target/main/main_observer.out
    ./target/main/main_singleton.out
    ./target/main/main_state_machine.out
    ./target/main/main_thread_safe_iostream.out
    ./target/main/main_thread_safe_queue.out
    ./target/main/main_thread.out
    ./target/main/main_worker_pool.out
    ./target/main/main_persistent_worker.out
    ./target/main/main_vector_2.out
    ./target/main/main_vector_3.out
    ./target/main/main_random_2D_coordinate_generator.out
    ./target/main/main_perlin_2D.out

    ./target/test/test_pool.out
    ./target/test/test_data_buffer.out
    ./target/test/test_argparse.out
    ./target/test/test_dual_key_map.out
    ./target/test/test_coordinate_compressor.out
    ./target/test/test_pair_hash.out
    ./target/test/test_observable_value.out
    ./target/test/test_angle.out
    ./target/test/test_timer.out
    ./target/test/test_chronometer.out
    ./target/test/test_cron.out
    ./target/test/test_select.out
)

for test in "${tests[@]}"; do
    echo -e "\033[1;34mRunning: ${test}\033[0m"
    "${test}" || exit 1
done
