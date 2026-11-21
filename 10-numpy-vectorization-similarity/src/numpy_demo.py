import numpy as np

def validate_2d_matrix(arr) -> np.ndarray:
    if not isinstance(arr, np.ndarray):
        arr = np.array(arr)
    if arr.ndim != 2 or arr.size == 0:
        raise ValueError("Input array must be 2D and non-empty.")
    return arr.astype(np.float64)

def l2_normalize_rows(matrix: np.ndarray) -> np.ndarray:
    norms = np.linalg.norm(matrix, axis=1, keepdims=True)
    # Replace zero norms with 1 to avoid division by zero and NaN/Inf
    norms[norms == 0] = 1.0
    return matrix / norms

def cosine_similarity_matrix(matrix: np.ndarray) -> np.ndarray:
    matrix = validate_2d_matrix(matrix)
    normalized_matrix = l2_normalize_rows(matrix)
    return np.dot(normalized_matrix, normalized_matrix.T)

def summarize_matrix(matrix: np.ndarray) -> dict[str, float | list[float]]:
    matrix = validate_2d_matrix(matrix)
    summary = {
        'shape': matrix.shape,
        'mean': np.mean(matrix),
        'std': np.std(matrix),
        'min': np.min(matrix),
        'max': np.max(matrix)
    }
    return summary
