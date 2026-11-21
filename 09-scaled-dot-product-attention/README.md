# 09 – Scaled Dot-Product Attention

Minimal implementation of row-wise softmax and scaled dot-product attention in pure Python (standard library only), verified with a standalone test runner (`src/main.py`).

## Structure

- `src/mini_attention.py` – `softmax_row` (with max-subtraction for numerical stability) and `scaled_dot_product_attention` with dimension validation.
- `src/main.py` – Self-contained test suite verifying softmax edge cases/stability, attention output shapes and row sums, and `ValueError` handling.

## Run Tests

Run `python3 src/main.py` from the lab folder.
