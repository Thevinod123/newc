# Verifiable Coding Labeler Instruction Guide

## Project: Verifiable Coding Problem Benchmark Scale Up

### Objective
Create programming problems in multiple languages that are difficult enough to stump at least one top-performing model.

---

## 🎯 Project Goal

The purpose of this project is to build a collection of challenging, well-defined programming problems designed to test and benchmark model reasoning across **12 programming languages**:

**Python, C, C#, C++, Java, JavaScript, Rust, Ruby, TypeScript, Go, PHP, and COBOL**

Each problem must:
- Be original and not a trivial variation of existing tasks.  
- Have a clear, verifiable solution that passes all reference tests.  
- Be difficult enough that at least one advanced model fails its test suite.  
- Follow the standard directory structure and submission process.

---

## ⚙ Environment Setup

You will use the **alignerr CLI** to claim, work on, validate, and submit problems.

### Prerequisites
- **Python 3.12+** - Required for the CLI and validation tools
- **uv** - Fast Python package manager for installing the CLI

### Installing uv

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

### Installing the alignerr CLI

You will receive a wheel file (`alignerr-*.whl`). Install it using uv:

```bash
uv tool install /path/to/alignerr-*.whl
```

Or if you have the wheel in your current directory:
```bash
uv tool install alignerr-*.whl
```

Verify the installation:
```bash
alignerr version
```

### Configuration

Set your authentication token (provided by your project admin):

```bash
export ALIGNERR_BEARER_TOKEN="your-token-here"
```

Add these to your `~/.bashrc`, `~/.zshrc`, or create a `.env` file in your working directory.

Verify your configuration:
```bash
alignerr config
```

---

## 🧱 Problem Review Workflow

📺 [Workflow Video 1](https://www.loom.com/share/b9d24de7f7704707abf974cb86d48138?sid=a802b05c-f387-40f5-9e12-0f6ae0fed5ba)  
📺 [Workflow Video 2](https://www.loom.com/share/1f955b81b56e497f96b1b93188b3cfac?sid=0d1d0196-fe65-47b3-9c69-6cf0e50efa15)

### Step 1: Claim a Task

Claim a task from your assigned project:

```bash
alignerr claim --project-id warrior-code-verifiers --folder Python
```

This reserves a task for you and stores it in your local state. The CLI remembers your current task.

### Step 2: Download the Problem

Download the problem files to your local machine:

```bash
alignerr download
```

This extracts the problem to `./work/<task-name>/` with all required files.

### Step 3: Create

Work on the problem in the `./work/<task-name>/` directory. Before making changes, validate the current state:

```bash
alignerr validate ./work/<task-name>/
```

Make your improvements and validate again to ensure everything passes.

---

## 📁 Required Files & Their Roles

| File | Description | Labeler Responsibility |
|------|--------------|------------------------|
| **metadata.json** | Contains metadata such as function name, language, dependencies, and configuration. | Verify that all fields are accurate, including file paths, function entry points, and test references. See detailed structure below. |
| **prompt.txt** | Main problem description and requirements. | Write a concise, clear, well-structured problem statement defining task, inputs, outputs, and constraints. Include examples. The first line must specify the language (e.g., "Use Python to solve this problem"). |
| **test.py** *(or equivalent)* | Contains test cases verifying solution correctness. | Write comprehensive tests covering normal, boundary, and edge cases. |
| **solution.py** *(or equivalent)* | Reference (ground truth) solution. | Implement a correct and efficient solution that fully passes all tests. |
| **run.sh** | Shell script to execute main program and test suite inside Docker. | Ensure this script references the correct files and runs successfully without modification. |
| **requirements.txt** | Lists extra dependencies (if needed). | Include only necessary libraries. Leave empty if not required. |

### metadata.json Detailed Structure

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

**Required Fields:**

- `benchmark` - Must be "code_verifiers"
- `name` - Unique problem identifier matching directory name
- `task_type` - Either "code-generation" or "code-editing"
- `programming_language` - Array of languages (e.g., ["Python"], ["Java"])
- `solution_file_path` - Array of solution files (e.g., ["solution.py"])
- `test_file_path` - Array of test files (e.g., ["test.py"])
- `execution_file_path` - Script to run tests (typically "run.sh")
- `tags` - Problem categories (e.g., ["Algorithm", "Recursion"])
- `timeout` - Max execution time in seconds (typically 10)
- `memory_limit` - Max memory (e.g., "256MB", "512MB")
- `num_test_cases` - Number of test cases (0 if not explicitly counted)

**Optional Fields:**

- `difficulty` - "Medium", "Hard", or "Expert" (auto-populated after model testing, can be omitted)
- `additional_files` - Array of extra files needed (e.g., ["requirements.txt", "helper.py"])

### Additional Files - What's Allowed

The `additional_files` field can reference:

**✅ Allowed:**
- `requirements.txt` - Python dependencies
- Helper source files (e.g., `helper.py`, `utils.java`)
- Configuration files (e.g., `config.json`)
- Text data files (e.g., `data.txt`, `input.csv`)

**❌ Not Allowed (Binary Files):**
- Compiled files: `.class`, `.jar`, `.war`, `.pyc`, `.pyo`, `.o`, `.so`, `.dll`, `.exe`
- Archives: `.zip`, `.tar`, `.gz`, `.rar`
- Media: `.jpg`, `.png`, `.gif`, `.pdf`, `.doc`
- Databases: `.db`, `.sqlite`

**All files must be text-based and UTF-8 readable. Binary files will cause validation to fail immediately and PRs will be blocked.**

---

## 🧰 Execution Environment (Docker and `run.sh`)

All problems execute inside a base **Docker image** for consistency.

Each problem’s `run.sh` should:
- Execute the main program and test cases.  
- Install dependencies from `requirements.txt` (if any).  
- Handle compilation or execution steps for the language.  
- Exit with code **0** on success and **1** on failure.

- Difficulty is "Medium" if 1 out of 3 models is stumped, difficulty is "Hard" if 2 out of 3 models are stumped, and difficulty is "Expert" if all 3 models are stumped.
- Labelers can **leave out** difficulty key from metadata.json since it will be populated after the model validation check is successfully completed.

**Labeler Checklist:**
- Dependencies are declared properly.  
- All references in `run.sh` are correct.  
- Code executes successfully inside the base image.

---

## 🌍 Language Coverage

12 supported languages:
> Python, C, C#, C++, Java, JavaScript, Rust, Ruby, TypeScript, Go, PHP, COBOL

---

## 🧠 Difficulty Calibration

Each problem must be challenging enough that **at least one high-performing model fails** its test suite.

Guidelines:
1. Start at **medium-hard** baseline difficulty.  
2. Include reasoning, recursion, or multi-step logic.  
3. To increase difficulty:
   - Add interdependent constraints.  
   - Introduce multi-phase logic (e.g., recursion + optimization).  
   - Include tricky but fair edge cases.

**Target:** solvable by experts, but **fails the HEIMDALL model** at least once.

---

## ✅ Validation and Submission

### Step 4: Validate Your Changes

After completing your improvements:

```bash
alignerr validate ./work/<task-name>/
```

- Fix any errors and rerun until clean
- Validation must **pass before submission**
- Checks structure, syntax, metadata, and test coverage

### Step 5: Submit Your Work

Submit the completed problem:

```bash
alignerr submit-work
```

This will:
- Package your work directory into a ZIP file
- Upload it to cloud storage
- Create a pull request for review
- Trigger automated evaluation

**Optional: Wait for Results**

```bash
alignerr submit-work --wait
```

This automatically waits for evaluation results after submission.

### Step 6: Check Evaluation Results

View the automated evaluation results:

```bash
alignerr poll-results
```

Or wait for results to become available:

```bash
alignerr poll-results --wait
```

The evaluation includes:
- **Diversity Analysis**: Checks if the problem is unique
- **Execution Results**: Verifies all tests pass
- **Model Testing**: Tests problem against multiple AI models
- **Requirements Verification**: Ensures submission guidelines are met

Results are saved to `./results/<task-name>/` with detailed reports.

### Understanding Evaluation Status

- **PASSED**: All checks completed successfully - task automatically finalized
- **DEFERRED**: Some issues need manual review - wait for feedback
- **FAILED**: Critical issues found - fix and resubmit

### Step 7: Next Steps Based on Results

**If evaluation PASSED:**
- Task is automatically finalized when using `--wait`
- You can immediately claim a new task

**If evaluation FAILED:**
- Make fixes in `./work/<task-name>/`
- Run `alignerr validate ./work/<task-name>/` to check locally
- Run `alignerr submit-work --wait` again (unlimited resubmissions)

**If evaluation DEFERRED:**
- Wait for manual review and feedback from admin
- Check back later or watch for notifications

**To abandon a task:**
```bash
alignerr abandon
```

---

## 📊 State Management

The CLI tracks your current task automatically:
- **Current task** is stored in `.alignerr/state.yaml`
- You can only work on one task at a time
- Use `alignerr config` to see your current task
- State persists across CLI sessions

When you claim a task, the CLI remembers:
- Task ID
- Project ID
- Work directory location
- Claim timestamp
- Submission status

---

## 🏁 Pre-Submission Checklist

Before running `alignerr submit-work`:
- ✅ Problem follows required structure
- ✅ Problem statement is clear and complete
- ✅ Unit tests cover all edge cases
- ✅ Reference solution passes all tests
- ✅ Local validation passes: `alignerr validate ./work/<task-name>/`
- ✅ All required files are present and correctly formatted

After submission, automated evaluation will verify:
- ✅ Problem is unique (diversity check)
- ✅ All tests execute successfully
- ✅ At least one strong model fails one or more tests
- ✅ All requirements are met

---

## 🔄 Complete Workflow Summary

```bash
# 1. Claim a task
alignerr claim --project-id <project-id> --folder <language>

# 2. Download the problem
alignerr download

# 3. Work on the problem in ./work/<task-name>/

# 4. Validate locally (checks structure, tests, requirements)
alignerr validate ./work/<task-name>/

# 5. Submit your work and wait for evaluation
alignerr submit-work --wait

# If evaluation PASSES, task is automatically finalized
# If FAILED, fix issues and resubmit
# If DEFERRED, wait for manual review

# Check results without submitting
alignerr poll-results

# Abandon task if needed
alignerr abandon
```

---

## 💡 Tips and Best Practices

- **Always validate locally first** - Run `alignerr validate ./work/<task-name>/` before every submission to catch issues early
- **Work on one task at a time** - The CLI enforces this through state management
- **Use `--wait` flags** - Save time by automatically waiting for results and auto-finalizing on pass
- **Check the results folder** - HTML reports in `./results/<task-name>/` provide detailed feedback
- **Resubmit freely** - Unlimited resubmissions until evaluation passes
- **Tasks auto-finalize** - When evaluation passes with `--wait`, task is automatically completed
- **Ask for help** - If evaluation is deferred or unclear, reach out to your project admin

---
