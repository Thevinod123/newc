# Alignerr CLI - Problem Review Guide

## 🚀 Quick Start

Set up your environment and start reviewing problems.

## Setup & Installation

### Prerequisites
- **Python 3.12+** - Required for the CLI and validation tools

### Install uv (Python Package Manager)

[uv](https://github.com/astral-sh/uv) is a fast Python package manager. It's required to install the alignerr CLI.

**macOS and Linux:**
```bash
curl -LsSf https://astral.sh/uv/install.sh | sh
```

Or with wget:
```bash
wget -qO- https://astral.sh/uv/install.sh | sh
```

**Windows:**
```powershell
powershell -ExecutionPolicy ByPass -c "irm https://astral.sh/uv/install.ps1 | iex"
```

### Install alignerr CLI

You will receive a wheel file (`alignerr-*.whl`) from your project administrator. Install it:

```bash
uv tool install /path/to/alignerr-*.whl
```

Verify installation:
```bash
alignerr version
```

### Configuration

Set your authentication credentials (provided by your admin):

```bash
export ALIGNERR_BEARER_TOKEN="your-token-here"
```

Add these to your shell profile (`~/.bashrc`, `~/.zshrc`) or create a `.env` file in your working directory.

Verify your setup:
```bash
alignerr config
```

---

## 🔄 Complete Workflow

### 1. Claim a Task

Claim a problem from your assigned project:

```bash
alignerr claim --project-id warrior-code-verifiers --folder Java
```

Available Folders:
- C
- C#
- C++
- COBOL
- Go
- Java
- JavaScript
- PHP
- Python
- Ruby
- Rust
- TypeScript

The CLI will reserve a task for you and remember it.

### 2. Download the Problem

Download the problem files:

```bash
alignerr download
```

This extracts files to `./work/<task-name>/`.

### 3. Create

Work on the problem in `./work/<task-name>/`. Validate your changes:

```bash
alignerr validate --problem-dir ./work/<task-name>/
```

### 4. Submit Your Work

Package and submit:

```bash
alignerr submit-work
```

Or submit and wait for results:

```bash
alignerr submit-work --wait
```

### 5. Check Evaluation Results

View automated evaluation:

```bash
alignerr poll-results
```

Or wait for results:

```bash
alignerr poll-results --wait
```

Results include:
- **Diversity Analysis** - Problem uniqueness
- **Execution Results** - Test pass/fail status
- **Model Testing** - AI model performance
- **Requirements Verification** - Guideline compliance

### 6. Next Steps

**If evaluation PASSED:**
- Task automatically finalized when using `--wait`
- Claim a new task immediately

**If evaluation FAILED:**
- Fix issues in `./work/<task-name>/`
- Run `alignerr validate ./work/<task-name>/`
- Resubmit with `alignerr submit-work --wait`

**If evaluation DEFERRED:**
- Wait for manual review feedback

**If abandoning:**
```bash
alignerr abandon
```

---

## 📁 Problem Structure

### Directory Layout

Each problem submission must follow this structure:

```
languages/<Language>/<YYYY-MM-DD-username-problem-name>/
├── run.sh              # Execution script (required)
├── metadata.json       # Problem metadata (required)
├── prompt.txt          # Problem description (required)
├── test.<ext>          # Test cases (required)
├── solution.<ext>      # Reference solution (required)
└── [additional files]  # Optional: requirements.txt, helper files, etc.
```

### Required Files

**run.sh** - Execution script that runs tests inside Docker
- Must be executable
- Must exit with code 0 on success, non-zero on failure
- Should handle dependency installation and compilation

**metadata.json** - Problem metadata (see detailed structure below)

**prompt.txt** - Problem description and requirements

**test.<ext>** - Test cases that verify solution correctness
- Extension matches the language (e.g., test.py, test.java, test.cpp)

**solution.<ext>** - Reference solution implementation
- Extension matches the language
- Must pass all tests

### metadata.json Structure

All fields and their purposes:

```json
{
  "benchmark": "code_verifiers",
  "name": "problem-name-unique-id",
  "task_type": "code-generation",
  "programming_language": ["Python"],
  "solution_file_path": ["solution.py"],
  "test_file_path": ["test.py"],
  "execution_file_path": "run.sh",
  "difficulty": "Medium",
  "tags": ["Algorithm", "Data Structure"],
  "timeout": 10,
  "memory_limit": "256MB",
  "num_test_cases": 5,
  "additional_files": ["requirements.txt"]
}
```

**Field Descriptions:**

- `name` (string, required): Unique problem identifier
- `task_type` (string, required): Either "code-generation" or "code-editing"
- `programming_language` (array, required): List of programming languages (e.g., ["Python"], ["Java"])
- `solution_file_path` (array, required): List of solution files (e.g., ["solution.py"])
- `test_file_path` (array, required): List of test files (e.g., ["test.py"])
- `execution_file_path` (string, required): Script to run tests (typically "run.sh")
- `difficulty` (string, optional): "Medium", "Hard", or "Expert" (auto-populated after model testing)
- `tags` (array, required): Problem categories (e.g., ["Algorithm", "Recursion", "Dynamic Programming"])
- `timeout` (integer, required): Max execution time in seconds (typically 10)
- `memory_limit` (string, required): Max memory (e.g., "256MB", "512MB")
- `num_test_cases` (integer, required): Number of test cases (0 if not explicitly counted)
- `additional_files` (array, optional): List of extra files needed (e.g., ["requirements.txt", "helper.py"])

### Additional Files

The `additional_files` field in metadata.json can reference:

**Allowed:**
- `requirements.txt` - Python dependencies
- Helper source files (e.g., `helper.py`, `utils.java`)
- Configuration files (e.g., `config.json`, `.env`)
- Text data files (e.g., `data.txt`, `input.csv`)

**Not Allowed:**
- Binary files (`.class`, `.jar`, `.pyc`, `.pyo`, `.o`, `.so`, `.dll`, `.exe`)
- Compiled artifacts
- Archive files (`.zip`, `.tar`, `.gz`, `.rar`)
- Image files (`.jpg`, `.png`, `.gif`)
- Binary data files

All files must be text-based and UTF-8 readable. Binary files will cause validation to fail immediately.

### File Restrictions

**Blocked file types:**
- Java: `.class`, `.jar`, `.war`
- Python: `.pyc`, `.pyo`
- C/C++: `.o`, `.so`, `.dll`, `.exe`
- Archives: `.zip`, `.tar`, `.gz`, `.rar`
- Media: `.jpg`, `.png`, `.gif`, `.pdf`, `.doc`
- Databases: `.db`, `.sqlite`

Submit only source code and text configuration files.

---

## 📊 State Management

The CLI tracks your current task automatically:
- State stored in `.alignerr/state.yaml`
- One task at a time
- Persists across sessions
- Use `alignerr config` to check status

---

## 💡 Tips

- **Always validate locally first** - Run `alignerr validate ./work/<task-name>/` before submitting
- **Use `--wait` flags** - Automatically wait for results and auto-finalize on pass
- **Check results folder** - `./results/<task-name>/` has detailed HTML reports
- **Resubmit freely** - No limit on resubmissions, iterate until it passes
- **Tasks auto-finalize** - When evaluation passes, task is automatically completed

---

## 📋 Evaluation Status

- **PASSED** - All checks passed, task automatically finalized
- **DEFERRED** - Manual review needed, wait for feedback
- **FAILED** - Fix issues and resubmit

---

## 🎥 Video Tutorials

📺 **[How to use](https://www.loom.com/share/55a28e72c5214513951d8a5396940dab)**  

For detailed instructions, see **INSTRUCTIONS.md**.
