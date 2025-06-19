#!/bin/bash

# Compile your parser
gcc -Wall -Wextra -Werror -o argo argo.c

run_test() {
	input="$1"
	expected="$2"

	output=$(echo -n "$input" | ./argo /dev/stdin | cat -e)

	if [ "$output" = "$expected" ]; then
		echo "✅ PASS: $input"
	else
		echo "❌ FAIL: $input"
		echo "Expected: $expected"
		echo "Got:      $output"
	fi
}

# ---------------------
# ✔️ Test Cases
# ---------------------

run_test "1" "1$"
run_test "\"bonjour\"" "\"bonjour\"$"
run_test "\"escape!\\\"\"" "\"escape!\\\"\"$"
run_test "{\"tomatoes\":42,\"potatoes\":234}" "{\"tomatoes\":42,\"potatoes\":234}$"
run_test "{\"recursion\":{\"recursion\":{\"recursion\":\"recursion\"}}}" "{\"recursion\":{\"recursion\":{\"recursion\":\"recursion\"}}}$"
run_test "\"unfinished string" "unexpected end of input$"
run_test "\"unfinished string 2\\\"" "unexpected end of input$"
run_test "{\"no value?\":}" "unexpected token '}'$"
run_test "{:12}" "unexpected token ':'$"
