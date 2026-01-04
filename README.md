
# Assessing Large Language Models in Verifying Concurrent Programs

## Overview

This repository contains an experimental framework for evaluating the ability of **large language models (LLMs)** to analyze and reason about **concurrent programs**, particularly under **relaxed memory models** such as **Total Store Order (TSO)** and **Partial Store Order (PSO)**.

The project consists of multiple Python scripts, each corresponding to a different LLM. Every model is queried with a **fixed set of concurrency-related questions** to assess its strengths and limitations in identifying concurrency bugs, understanding program behavior, and reasoning about relaxed memory semantics.

The evaluation focuses on common verification tasks such as:
- Data race detection
- Assertion violation analysis
- Deadlock detection
- Program comprehension
- Reasoning under relaxed memory models

## Query Set

Query 1:
"Can you identify a potential data race or race condition in the following program?"

Query 2:
"Does the assert condition always hold in the following program? Or can there be a case where the assertion fails?"

Query 3:
"Can you summarize what the following program does?"

Query 4:
"How do relaxed memory models such as total store order and partial store order affect the following program's behaviour?"

Query 5:
"Can you suggest any edits or optimizations to improve the concurrency handling in the following program?"

Query 6:
"Can you identify a potential deadlock in the following program?"

Query 7:
"Can you identify a potential data race in the following program?"


## How to Run

### Prerequisites

- Python 3.9 or later
- API keys or local endpoints for the evaluated LLMs (as applicable)

---

### Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/ridhij93/LLM-Conc-RMM.git
   cd LLM-Conc-RMM

2. Run
   ```bash
    python model_variants.py

## Citation

If you use this repository in academic work, please cite the following paper:

```bibtex
@inproceedings{jain2026assessing,
  title     = {Assessing Large Language Models in Verifying Concurrent Programs},
  author    = {Ridhi Jain and Rahul Purandare},
  booktitle = {Proceedings of the IEEE International Conference on Software Analysis, Evolution and Reengineering (SANER)},
  year      = {2026},
  note      = {Accepted}
}

