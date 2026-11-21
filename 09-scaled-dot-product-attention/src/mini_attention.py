import math
from typing import List, Tuple

def softmax_row(scores: List[float]) -> List[float]:
    if not scores:
        raise ValueError("Scores cannot be empty")
    
    max_score = max(scores)
    exp_scores = [math.exp(score - max_score) for score in scores]
    total = sum(exp_scores)
    return [score / total for score in exp_scores]

def scaled_dot_product_attention(q: List[List[float]], k: List[List[float]], v: List[List[float]]) -> Tuple[List[List[float]], List[List[float]]]:
    if not q or not k or not v:
        raise ValueError("Input matrices cannot be empty")
    
    d_k = len(q[0])
    seq_q = len(q)
    seq_k = len(k)
    seq_v = len(v)
    
    # Check dimension consistency
    if len(k[0]) != d_k:
        raise ValueError("Dimension mismatch: q[0] length must match k[0] length")
    if len(v[0]) != d_k:
        raise ValueError("Dimension mismatch: v[0] length must match k[0] length")
    if seq_k != seq_v:
        raise ValueError("Dimension mismatch: k row count must match v row count")
    
    # Calculate (q @ k^T)
    scores = [[sum(qi * ki for qi, ki in zip(q_row, k_col)) for k_col in zip(*k)] for q_row in q]
    
    # Scale by 1.0 / math.sqrt(d_k)
    scaled_scores = [[score / math.sqrt(d_k) for score in row] for row in scores]
    
    # Apply softmax_row per query row
    weights = [softmax_row(row) for row in scaled_scores]
    
    # Calculate (weights @ v)
    output = [[sum(w * vi for w, vi in zip(weight_row, v_col)) for v_col in zip(*v)] for weight_row in weights]
    
    return output, weights
