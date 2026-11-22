import os
import numpy as np
from numpy_demo import validate_2d_matrix, l2_normalize_rows, cosine_similarity_matrix, summarize_matrix

def test_validate_2d_matrix():
    # Test normal 2D matrix
    matrix = np.array([[1, 2], [3, 4]])
    assert validate_2d_matrix(matrix).shape == (2, 2)

    # Test 1D array should raise ValueError
    try:
        validate_2d_matrix(np.array([1, 2, 3]))
    except ValueError as e:
        assert str(e) == "Input array must be 2D and non-empty."

    # Test empty array should raise ValueError
    try:
        validate_2d_matrix(np.array([]))
    except ValueError as e:
        assert str(e) == "Input array must be 2D and non-empty."

def test_l2_normalize_rows():
    # Test normal 2D matrix
    matrix = np.array([[1, 2], [3, 4]])
    normalized = l2_normalize_rows(matrix)
    np.testing.assert_allclose(np.linalg.norm(normalized, axis=1), np.ones(2), rtol=1e-6)

    # Test matrix with a zero row
    matrix_with_zero_row = np.array([[0, 0], [3, 4]])
    normalized = l2_normalize_rows(matrix_with_zero_row)
    assert np.allclose(normalized[0], np.zeros(2), atol=1e-6)
    np.testing.assert_allclose(np.linalg.norm(normalized[1], axis=0), 1.0, rtol=1e-6)

def test_cosine_similarity_matrix():
    # Test normal 2D matrix
    matrix = np.array([[1, 2], [3, 4]])
    similarity = cosine_similarity_matrix(matrix)
    assert np.allclose(similarity[0, 0], 1.0, rtol=1e-6)
    assert np.allclose(similarity[1, 1], 1.0, rtol=1e-6)

def test_summarize_matrix():
    # Test normal 2D matrix
    matrix = np.array([[1, 2], [3, 4]])
    summary = summarize_matrix(matrix)
    assert summary['shape'] == (2, 2)
    assert summary['mean'] == np.mean(matrix)
    assert summary['std'] == np.std(matrix)
    assert summary['min'] == np.min(matrix)
    assert summary['max'] == np.max(matrix)

if __name__ == "__main__":
    # Ensure src/ directory exists
    os.makedirs(os.path.dirname(__file__), exist_ok=True)

    # Run tests
    test_validate_2d_matrix()
    test_l2_normalize_rows()
    test_cosine_similarity_matrix()
    test_summarize_matrix()

    print("All tests passed successfully.")
