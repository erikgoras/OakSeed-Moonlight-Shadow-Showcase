import torch

try:
    from src.torch_demo import (
        TinyProjection,
        compute_scalar_gradient,
        get_device,
        pairwise_cosine_similarity,
    )
except ModuleNotFoundError:
    from torch_demo import (
        TinyProjection,
        compute_scalar_gradient,
        get_device,
        pairwise_cosine_similarity,
    )


def test_device_selection() -> None:
    device = get_device()
    assert device.type in ["cpu", "mps"], f"Expected device to be 'cpu' or 'mps', got {device.type}"
    print(f"Device selected: {device}")


def test_tiny_projection() -> None:
    device = get_device()
    model = TinyProjection(in_features=5, out_features=3).to(device)
    input_tensor = torch.randn(1, 5, device=device)
    output_tensor = model(input_tensor)
    assert output_tensor.shape == (1, 3), f"Expected output shape (1, 3), got {output_tensor.shape}"
    assert output_tensor.device.type == device.type, (
        f"Expected output device type {device.type}, got {output_tensor.device}"
    )
    print("TinyProjection test passed.")


def test_pairwise_cosine_similarity() -> None:
    orthogonal_vectors = torch.tensor([[1.0, 0.0], [0.0, 1.0]])
    parallel_vectors = torch.tensor([[1.0, 1.0], [2.0, 2.0]])

    cos_sim_orthogonal = pairwise_cosine_similarity(orthogonal_vectors)
    cos_sim_parallel = pairwise_cosine_similarity(parallel_vectors)

    torch.testing.assert_close(
        cos_sim_orthogonal,
        torch.tensor([[1.0, 0.0], [0.0, 1.0]]),
        atol=1e-5,
        rtol=1e-5,
    )
    torch.testing.assert_close(
        cos_sim_parallel,
        torch.ones(2, 2),
        atol=1e-5,
        rtol=1e-5,
    )
    print("Pairwise cosine similarity test passed.")


def test_autograd_gradient() -> None:
    x = torch.tensor([1.0, 2.0, 3.0])
    expected_gradient = torch.tensor([2.0, 4.0, 6.0])
    gradient = compute_scalar_gradient(x)
    assert gradient is not None, "Expected gradient tensor, got None"
    torch.testing.assert_close(gradient, expected_gradient, atol=1e-5, rtol=1e-5)
    print("Autograd gradient test passed.")


def test_error_handling() -> None:
    try:
        pairwise_cosine_similarity(torch.tensor([]))
        assert False, "Expected ValueError for empty tensor"
    except ValueError as e:
        assert str(e) == "Input must be a non-empty 2D tensor.", f"Unexpected error message: {e}"

    try:
        pairwise_cosine_similarity(torch.tensor([1.0, 2.0]))
        assert False, "Expected ValueError for 1D tensor"
    except ValueError as e:
        assert str(e) == "Input must be a non-empty 2D tensor.", f"Unexpected error message: {e}"

    print("Error handling test passed.")


if __name__ == "__main__":
    test_device_selection()
    test_tiny_projection()
    test_pairwise_cosine_similarity()
    test_autograd_gradient()
    test_error_handling()
    print("All tests passed.")
