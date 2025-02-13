#!/usr/bin/env bash

set -eo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
test_dir="$script_dir/tests"
answer_dir="$script_dir/answers"
project_root="$(dirname "$script_dir")"

current_time_ms() {
    echo $(($(date +%s%N) / 1000000))
}

# Initializing the binary path
triag_bin=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        -b)
            triag_bin="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            rm -f "$temp_result"
            exit 1
            ;;
    esac
done

[ -z "$triag_bin" ] && triag_bin="${TRIAG_BIN}"

# Automatic binary search, if it is not specified
if [ -z "$triag_bin" ]; then
    if [ -f "$project_root/build/triag" ]; then
        triag_bin="$project_root/build/triag"
    elif [ -f "./triag" ]; then
        triag_bin="./triag"
    else
        echo "ERROR: 'triag' binary not found. Options:"
        echo "1. Use -b to specify path"
        echo "2. Set TRIAG_BIN environment variable"
        rm -f "$temp_result"
        exit 1
    fi
fi

if [ ! -f "$triag_bin" ]; then
    echo "ERROR: 'triag' binary not found at: $triag_bin"
    rm -f "$temp_result"
    exit 1
fi

temp_result="$script_dir/real_ans.ans"
truncate -s 0 "$temp_result"

for test_file in "$test_dir"/test*.txt; do
    base_name=$(basename "$test_file" .txt)
    answer_file="$answer_dir/${base_name/test/ans}.txt"

    [ ! -f "$answer_file" ] && { echo "Missing answer for $test_file"; rm -f "$temp_result"; exit 1; }

    start_time=$(current_time_ms)
    "$triag_bin" < "$test_file" -l > "$temp_result"
    elapsed_time=$(( $(current_time_ms) - start_time ))

    if diff -q "$answer_file" "$temp_result" > /dev/null; then
        echo "$base_name passed in ${elapsed_time} ms"
    else
        echo "$base_name failed"
        diff --color=always "$answer_file" "$temp_result"
        rm -f "$temp_result"
        exit 1
    fi
done

rm -f "$temp_result"
