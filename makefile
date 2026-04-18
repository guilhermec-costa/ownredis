.PHONY: fmt

fmt:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i