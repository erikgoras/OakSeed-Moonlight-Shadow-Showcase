import sys
from pathlib import Path
import math

# Add parent directory to path for imports
sys.path.insert(0, str(Path(__file__).resolve().parent))
from mini_attention import softmax_row, scaled_dot_product_attention

def test_softmax():
    # Test empty input
    try:
        softmax_row([])
        assert False, "Expected ValueError for empty scores"
    except ValueError as e:
        assert "cannot be empty" in str(e), f"Wrong error message: {e}"
    
    # Test basic case
    scores = [3.0, 1.0]
    probs = softmax_row(scores)
    assert math.isclose(sum(probs), 1.0, abs_tol=1e-7), "Probabilities must sum to 1.0"
    
    # Test stability with large values
    large_scores = [100.0, 99.0]
    probs_large = softmax_row(large_scores)
    assert math.isclose(sum(probs_large), 1.0, abs_tol=1e-7), "Large scores must still sum to 1.0"
    
    # Test negative values
    neg_scores = [-5.0, -10.0]
    probs_neg = softmax_row(neg_scores)
    assert math.isclose(sum(probs_neg), 1.0, abs_tol=1e-7), "Negative scores must sum to 1.0"

def test_attention():
    # Test known case: q=k=v=[1][2]
    q = [[1.0, 2.0]]
    k = [[1.0, 2.0]]
    v = [[3.0, 4.0]]
    
    out, weights = scaled_dot_product_attention(q, k, v)
    
    # Verify output shape is (seq_q, d_k)
    assert len(out) == len(q), f"Output length mismatch: {len(out)} vs {len(q)}"
    assert len(weights) == len(q), f"Weights length mismatch: {len(weights)} vs {len(q)}"
    
    # Verify row sums to 1.0
    for w in weights:
        assert math.isclose(sum(w), 1.0, abs_tol=1e-7), "Weight rows must sum to 1.0"

def test_value_errors():
    q = [[1.0, 2.0]]  # d_k=2, seq_q=1
    
    # Empty matrices
    try:
        scaled_dot_product_attention([], [], [])
        assert False, "Expected ValueError for empty q"
    except ValueError as e:
        pass
    
    # k dimension mismatch (k[0] != d_k)
    try:
        scaled_dot_product_attention([[1.0, 2.0]], [[1.0]], [[3.0]])
        assert False, "Expected ValueError for k dimension mismatch"
    except ValueError as e:
        pass
    
    # v dimension mismatch (v[0] != d_k)
    try:
        scaled_dot_product_attention([[1.0, 2.0]], [[1.0, 2.0]], [[3.0]])
        assert False, "Expected ValueError for v dimension mismatch"
    except ValueError as e:
        pass
    
    # k-v row count mismatch
    try:
        scaled_dot_product_attention([[1.0, 2.0]] * 2, [[1.0, 2.0]], [[3.0]])
        assert False, "Expected ValueError for k-v row count mismatch"
    except ValueError as e:
        pass

if __name__ == "__main__":
    try:
        test_softmax()
        print("OK: softmax_row tests passed")
        
        test_attention()
        print("OK: attention tests passed")
        
        test_value_errors()
        print("OK: value error tests passed")
        
        print("OK: mini_attention tests passed")
    except AssertionError as e:
        print(f"FAILED: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"ERROR: {e}")
        sys.exit(1)
