#!/usr/bin/env bash

set -eo pipefail

test_dir="./tests"
answer_dir="./answers"

temp_result="real_ans.ans"

truncate -s 0 "$temp_result"

current_time_ms() {
    echo $(($(date +%s%N) / 1000000))
}

for test_file in "$test_dir"/test*.txt; do
    base_name=$(basename "$test_file" .txt)
    answer_file="$answer_dir/${base_name/test/ans}.txt"

    if [ ! -f "$answer_file" ]; then
        echo "Missing answer file for $test_file"
        exit 1
    fi

    start_time=$(current_time_ms)
    ./../build/triag < "$test_file" -l > "$temp_result"
    end_time=$(current_time_ms)
    elapsed_time=$((end_time - start_time))

    if diff -q "$answer_file" "$temp_result" > /dev/null; then
        echo "$base_name passed in ${elapsed_time} ms"
    else
        echo "$base_name failed"
        echo "Differences:"
        diff "$answer_file" "$temp_result"
    fi
done

rm -f "$temp_result"
