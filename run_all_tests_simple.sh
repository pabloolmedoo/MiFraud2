#!/usr/bin/env bash
set -u

EXE="./Fraud2"
TEST_DIR="tests"
TMP_DIR=".test_tmp_simple"

if [[ ! -x "$EXE" ]]; then
  echo "ERROR: no encuentro el ejecutable $EXE"
  echo "Compila antes con:"
  echo "g++ -std=c++17 -Iinclude -o Fraud2 src/main.cpp src/Location.cpp src/VectorLocation.cpp src/VectorInt.cpp src/Clustering.cpp src/ArrayClustering.cpp"
  exit 1
fi

if [[ ! -d "$TEST_DIR" ]]; then
  echo "ERROR: no encuentro la carpeta $TEST_DIR"
  exit 1
fi

mkdir -p "$TMP_DIR"

total=0
passed=0
failed=0

normalize() {
  sed 's/[[:space:]]\+$//' "$1" | awk 'NF {p=1} {lines[NR]=$0} END {last=NR; while(last>0 && lines[last]=="") last--; for(i=1;i<=last;i++) print lines[i]}'
}

for testfile in "$TEST_DIR"/*.test; do
  [[ -f "$testfile" ]] || continue

  name=$(basename "$testfile")
  call=$(awk '/^%%%CALL/{sub(/^%%%CALL[[:space:]]*/, ""); print; exit}' "$testfile")

  expected_file="$TMP_DIR/$name.expected"
  actual_file="$TMP_DIR/$name.actual"
  expected_norm="$TMP_DIR/$name.expected.norm"
  actual_norm="$TMP_DIR/$name.actual.norm"
  diff_file="$TMP_DIR/$name.diff"

  awk '
    /^%%%OUTPUT/ {out=1; next}
    /^%%%/ && out==1 {out=0}
    out==1 {print}
  ' "$testfile" > "$expected_file"

  total=$((total+1))

  if [[ -z "$call" ]]; then
    $EXE > "$actual_file" 2>&1
  else
    # shellcheck disable=SC2086
    $EXE $call > "$actual_file" 2>&1
  fi

  normalize "$expected_file" > "$expected_norm"
  normalize "$actual_file" > "$actual_norm"

  if diff -u "$expected_norm" "$actual_norm" > "$diff_file"; then
    echo "PASS $name"
    passed=$((passed+1))
  else
    echo "FAIL $name"
    echo "  Comando: $EXE $call"
    echo "  Diferencias:"
    sed 's/^/    /' "$diff_file" | head -80
    failed=$((failed+1))
  fi
done

echo
echo "=============================="
echo "RESULTADO: $passed/$total tests pasados"
echo "Fallidos: $failed"
echo "=============================="

if [[ "$failed" -ne 0 ]]; then
  echo
  echo "Los archivos de salida y diff están en: $TMP_DIR"
  exit 1
fi

exit 0
