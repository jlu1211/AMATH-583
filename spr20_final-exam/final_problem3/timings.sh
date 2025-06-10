#!/usr/bin/env bash
# timings.sh — output strong‐scaling data as CSV

input="input_data.png"
output_dir="."

# Get the sequential baseline once
seq_time=$(./xcpp-png-gs "$input" "$output_dir/seq.png" 1 \
  | grep 'Sequential grayscale time' \
  | awk '{print $4}')

if [ -z "$seq_time" ]; then
    echo "Error: Failed to get sequential timing" >&2
    exit 1
fi

# Print CSV header
echo "Threads,Seq_time_ms,Thr_time_ms,Speedup"

# Loop over thread counts
for nt in 1 2 3 4; do
  # Grab the threaded time
  thr_time=$(./xcpp-png-gs "$input" "$output_dir/thread.png" "$nt" \
    | grep 'Threaded grayscale time' \
    | awk '{print $5}')
  
  if [ -z "$thr_time" ]; then
      echo "Error: Failed to get threaded timing for $nt threads" >&2
      continue
  fi
  
  # Compute speedup
  speedup=$(awk "BEGIN {printf \"%.2f\", $seq_time/$thr_time}")
  
  # Emit CSV line
  echo "${nt},${seq_time},${thr_time},${speedup}"
done
