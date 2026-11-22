import torch
import torch.nn as nn
import torch.nn.functional as F


def get_device() -> torch.device:
    """Selects Apple MPS if available, otherwise falls back to CPU."""
    if torch.backends.mps.is_available():
        return torch.device("mps")
    return torch.device("cpu")


def pairwise_cosine_similarity(matrix: torch.Tensor) -> torch.Tensor:
    """Computes an (N, N) cosine similarity matrix for a non-empty 2D tensor of shape (N, D)."""
    if matrix.ndim != 2 or matrix.numel() <= 0:
        raise ValueError("Input must be a non-empty 2D tensor.")

    x = matrix.to(dtype=torch.float32)
    normalized = F.normalize(x, p=2, dim=1)
    return normalized @ normalized.T


def compute_scalar_gradient(x: torch.Tensor) -> torch.Tensor:
    """Computes d(sum(x^2)) / dx = 2x using PyTorch autograd on a leaf tensor."""
    x_req = x.detach().to(dtype=torch.float32).clone().requires_grad_(True)
    loss = (x_req ** 2).sum()
    loss.backward()
    return x_req.grad


class TinyProjection(nn.Module):
    """Minimal neural network module with a single linear projection layer."""

    def __init__(self, in_features: int, out_features: int) -> None:
        super().__init__()
        self.linear = nn.Linear(in_features, out_features)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.linear(x)
