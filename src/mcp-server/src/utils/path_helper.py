from pathlib import Path

def get_project_root():
    """
    Automatically retrieve the project root path.

    Starts from the current file's directory and moves upward 
    until a directory containing 'pyproject.toml' is found.
    If none is found, returns the parent directory of the current file as a fallback.
    """
    current = Path(__file__).resolve()
    for parent in current.parents:
        if (parent / "pyproject.toml").exists():
            return parent
    # Fallback: return the parent.parent.parent directory of the current file
    return current.parent.parent.parent

